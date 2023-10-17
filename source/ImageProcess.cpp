#include "ImageProcess.h"
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <qimage.h>

ImageProcess::ImageProcess() {}
QImage ImageProcess::fourierTransform(const QImage &inputImage) {
  // 将QImage转换为cv::Mat,如果为彩色图像，需要转换为灰度图
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 将图像转换为浮点型
  mat.convertTo(mat, CV_32F);

  // 执行傅里叶变换
  cv::Mat planes[] = {cv::Mat_<float>(mat), cv::Mat::zeros(mat.size(), CV_32F)};
  cv::Mat complexImg;
  cv::merge(planes, 2, complexImg);
  cv::dft(complexImg, complexImg);

  // 分离频域图像的实部和虚部
  cv::split(complexImg, planes);
  cv::Mat mag;
  cv::magnitude(planes[0], planes[1], mag);

  // 对数缩放
  mag += cv::Scalar::all(1);
  cv::log(mag, mag);

  // 剪切和重分布图像频谱
  cv::normalize(mag, mag, 0, 255, cv::NORM_MINMAX);
  mag.convertTo(mag, CV_8U);

  // 转换为QImage
  cv::cvtColor(mag, mag, cv::COLOR_GRAY2RGB);
  QImage result = QImage((uchar *)mag.data, mag.cols, mag.rows, mag.step,
                         QImage::Format_RGB888)
                      .copy();

  return result;
}

QImage ImageProcess::displayHistogram(const QImage &inputImage) {
  // 将QImage转换为灰度图
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 直方图参数
  int histSize = 256;
  float range[] = {0, 256};
  const float *histRange = {range};

  // 计算直方图
  cv::Mat hist;
  // 具体实现为遍历图像的每个像素，将其值作为索引，将对应的直方图值加一
  myCalcHist(mat, hist, histSize);

  // 绘制直方图
  cv::Mat histImage(256, 256, CV_8UC3, cv::Scalar(0, 0, 0));
  double maxHeight = 0;
  cv::minMaxLoc(hist, 0, &maxHeight, 0, 0);
  for (int i = 0; i < 256; i++) {
    int binValue = hist.at<int>(i);
    int intensity = static_cast<int>(binValue * 256 / maxHeight);
    cv::line(histImage, cv::Point(i, 256), cv::Point(i, 256 - intensity),
             cv::Scalar(255, 255, 255));
  }

  // 转换为QImage
  QImage result = QImage((uchar *)histImage.data, histImage.cols,
                         histImage.rows, histImage.step, QImage::Format_RGB888)
                      .copy();
  return result;
}

QImage ImageProcess::histogramEqualization(const QImage &inputImage) {
  // 将QImage转换为灰度图
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 直方图均衡化，具体实现为计算累计直方图，然后将累计直方图归一化，最后将原图像的像素值映射到新的像素值
  cv::Mat dst;
  myEqualizeHist(mat, dst);
  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}

QImage ImageProcess::applyCLAHE(const QImage &inputImage, float clipLimit,
                                int tileGridSize) {
  // 将QImage转换为灰度图
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  //   //
  //   使用自定义的CLAHE算法，具体实现为将图像分割为tiles，对每个tile进行直方图均衡化（使用了对比度限制），最后使用双线性插值合并tiles
  //   cv::Mat dst;
  //   myCreateCLAHE(mat, dst, tileGridSize, clipLimit);

  // 使用OpenCV自带的CLAHE算法
  cv::Ptr<cv::CLAHE> clahe =
      cv::createCLAHE(clipLimit, cv::Size(tileGridSize, tileGridSize));
  cv::Mat dst;
  clahe->apply(mat, dst);

  // 转换为QImage
  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}

void ImageProcess::myCalcHist(const cv::Mat &src, cv::Mat &hist, int histSize) {
  hist = cv::Mat::zeros(1, histSize, CV_32SC1);
  for (int r = 0; r < src.rows; r++) {
    for (int c = 0; c < src.cols; c++) {
      hist.at<int>(src.at<uchar>(r, c))++;
    }
  }
}

void ImageProcess::myEqualizeHist(const cv::Mat &src, cv::Mat &dst) {
  cv::Mat hist;
  myCalcHist(src, hist, 256);

  // 计算累计直方图
  cv::Mat cdf = cv::Mat::zeros(1, 256, CV_32SC1);
  cdf.at<int>(0) = hist.at<int>(0);
  for (int i = 1; i < 256; i++) {
    cdf.at<int>(i) = cdf.at<int>(i - 1) + hist.at<int>(i);
  }

  // 归一化
  cdf = cdf * 255 / (src.rows * src.cols);

  // 映射
  dst = cv::Mat::zeros(src.size(), CV_8UC1);
  for (int r = 0; r < src.rows; r++) {
    for (int c = 0; c < src.cols; c++) {
      dst.at<uchar>(r, c) = cdf.at<int>(src.at<uchar>(r, c));
    }
  }
}

void ImageProcess::myCreateCLAHE(const cv::Mat &src, cv::Mat &dst, int tileSize,
                                 float clipLimit) {
  int rows = src.rows / tileSize;
  int cols = src.cols / tileSize;

  std::vector<cv::Mat> tiles;

  // 分割图像为tiles并进行直方图均衡化
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      cv::Rect region(j * tileSize, i * tileSize, tileSize, tileSize);
      cv::Mat tile = src(region);
      cv::Mat tileEqualized;
      myEqualizeHistWithClipLimit(tile, tileEqualized, clipLimit);
      tiles.push_back(tileEqualized);
    }
  }

  dst = cv::Mat::zeros(src.size(), src.type());
  // 对于每个像素位置
  for (int y = 0; y < src.rows; y++) {
    for (int x = 0; x < src.cols; x++) {
      // 计算当前像素所在的tile的坐标
      int tx = x / tileSize;
      int ty = y / tileSize;

      // 计算当前像素位置在tile中的相对位置
      float alpha = (x % tileSize) / (float)tileSize;
      float beta = (y % tileSize) / (float)tileSize;

      // 获取四个邻近的tile中的像素值
      uchar topLeft =
          tiles[ty * cols + tx].at<uchar>(y % tileSize, x % tileSize);
      uchar topRight =
          (tx + 1 < cols)
              ? tiles[ty * cols + tx + 1].at<uchar>(y % tileSize, x % tileSize)
              : topLeft;
      uchar bottomLeft =
          (ty + 1 < rows) ? tiles[(ty + 1) * cols + tx].at<uchar>(y % tileSize,
                                                                  x % tileSize)
                          : topLeft;
      uchar bottomRight = (tx + 1 < cols && ty + 1 < rows)
                              ? tiles[(ty + 1) * cols + tx + 1].at<uchar>(
                                    y % tileSize, x % tileSize)
                              : topLeft;

      // 在X轴上进行线性插值
      uchar top = alpha * topLeft + (1 - alpha) * topRight;
      uchar bottom = alpha * bottomLeft + (1 - alpha) * bottomRight;

      // 在Y轴上进行线性插值
      uchar interpolatedValue = beta * top + (1 - beta) * bottom;

      dst.at<uchar>(y, x) = interpolatedValue;
    }
  }
  //   // 初始化一个空的目标图像
  //   dst = cv::Mat::zeros(src.size(), src.type());

  //   // 遍历每一个tile
  //   for (int i = 0; i < rows; i++) {
  //     for (int j = 0; j < cols; j++) {
  //       // 计算当前tile在目标图像中的位置
  //       cv::Rect region(j * tileSize, i * tileSize, tileSize, tileSize);

  //       // 将tile复制到目标图像的适当位置
  //       tiles[i * cols + j].copyTo(dst(region));
  //     }
  //   }
}

void ImageProcess::myEqualizeHistWithClipLimit(const cv::Mat &src, cv::Mat &dst,
                                               float clipLimit) {
  cv::Mat hist;
  myCalcHist(src, hist, 256);
  double maxHeight = 0;
  cv::minMaxLoc(hist, 0, (double *)&maxHeight, 0, 0);

  int clipLimit_ = clipLimit * maxHeight;
  // 对比度限制
  int excess = 0;
  for (int i = 0; i < 256; i++) {
    if (hist.at<int>(i) > clipLimit_) {
      excess += hist.at<int>(i) - clipLimit_;
      hist.at<int>(i) = clipLimit_;
    }
  }

  int avg_increament = excess / 256;
  for (int i = 0; i < 256; i++) {
    hist.at<int>(i) += avg_increament;
  }

  // 计算累计直方图
  cv::Mat cdf = cv::Mat::zeros(1, 256, CV_32SC1);
  cdf.at<int>(0) = hist.at<int>(0);
  for (int i = 1; i < 256; i++) {
    cdf.at<int>(i) = cdf.at<int>(i - 1) + hist.at<int>(i);
  }

  // 归一化
  cdf = cdf * 255 / (src.rows * src.cols);

  // 映射
  dst = cv::Mat::zeros(src.size(), CV_8UC1);
  for (int r = 0; r < src.rows; r++) {
    for (int c = 0; c < src.cols; c++) {
      dst.at<uchar>(r, c) = cdf.at<int>(src.at<uchar>(r, c));
    }
  }
}

void ImageProcess::readImageToMat(const QImage &inputImage, cv::Mat &mat) {
  QImage image = inputImage.convertToFormat(QImage::Format_Grayscale8);
  cv::Mat temp(image.height(), image.width(), CV_8UC1, image.bits(),
               image.bytesPerLine());
  mat = temp.clone();
}

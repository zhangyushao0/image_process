#include "ImageProcess.h"
#include <QDebug>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <qimage.h>
#include <qsize.h>

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
  // 将低频信息移动到频谱的中心
  int cx = complexImg.cols / 2;
  int cy = complexImg.rows / 2;

  cv::Mat q0(complexImg, cv::Rect(0, 0, cx, cy));   // 左上角图像区域
  cv::Mat q1(complexImg, cv::Rect(cx, 0, cx, cy));  // 右上角图像区域
  cv::Mat q2(complexImg, cv::Rect(0, cy, cx, cy));  // 左下角图像区域
  cv::Mat q3(complexImg, cv::Rect(cx, cy, cx, cy)); // 右下角图像区域

  // 交换象限（左上与右下进行交换，右上与左下进行交换）
  cv::Mat tmp;
  q0.copyTo(tmp);
  q3.copyTo(q0);
  tmp.copyTo(q3);
  q1.copyTo(tmp);
  q2.copyTo(q1);
  tmp.copyTo(q2);
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
// 具体实现为遍历图像的每个像素，将其值作为索引，将对应的直方图值加一
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

      // 防止索引超出边界
      int next_tx = (tx + 1 < cols) ? tx + 1 : tx;
      int next_ty = (ty + 1 < rows) ? ty + 1 : ty;

      // 获取四个邻近的tile中的像素值
      uchar topLeft =
          tiles[ty * cols + tx].at<uchar>(y % tileSize, x % tileSize);
      uchar topRight =
          tiles[ty * cols + next_tx].at<uchar>(y % tileSize, x % tileSize);
      uchar bottomLeft =
          tiles[next_ty * cols + tx].at<uchar>(y % tileSize, x % tileSize);
      uchar bottomRight =
          tiles[next_ty * cols + next_tx].at<uchar>(y % tileSize, x % tileSize);

      // 在X轴上进行线性插值
      float top = (1 - alpha) * topLeft + alpha * topRight;
      float bottom = (1 - alpha) * bottomLeft + alpha * bottomRight;

      // 在Y轴上进行线性插值
      uchar interpolatedValue = (uchar)((1 - beta) * top + beta * bottom);

      dst.at<uchar>(y, x) = interpolatedValue;
    }
  }
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

QImage ImageProcess::addGaussianNoise(const QImage &inputImage, double mean,
                                      double sigma) {
  // 将QImage转换为灰度图 CV_8UC1
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 转换为CV_8SC1
  mat.convertTo(mat, CV_16SC1);

  // 添加高斯噪声
  cv::Mat noise = cv::Mat(mat.size(), CV_16SC1);
  cv::randn(noise, mean, sigma);
  cv::Mat noiseImage = mat + noise;

  // 转换回CV_8UC1
  noiseImage.convertTo(noiseImage, CV_8UC1);

  //  转换为QImage
  QImage result =
      QImage((uchar *)noiseImage.data, noiseImage.cols, noiseImage.rows,
             noiseImage.step, QImage::Format_Grayscale8)
          .copy();
  return result;
}
QImage ImageProcess::addSaltAndPepperNoise(const QImage &inputImage, double pa,
                                           double pb) {
  // 将QImage转换为灰度图 CV_8UC1
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 添加椒盐噪声
  cv::Mat noise = cv::Mat(mat.size(), CV_8UC1);
  cv::randu(noise, 0, 255);
  cv::Mat salt = noise > 255 * (1 - pa);
  cv::Mat pepper = noise < 255 * pb;
  mat.setTo(255, salt);
  mat.setTo(0, pepper);
  //  转换为QImage
  QImage result = QImage((uchar *)mat.data, mat.cols, mat.rows, mat.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}
QImage ImageProcess::medianBlurFilter(const QImage &inputImage,
                                      int kernelSize) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  int kernel_size = 3;
  cv::medianBlur(mat, dst, kernel_size);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}
QImage ImageProcess::meanBlurFilter(const QImage &inputImage, int kernelSize) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  cv::Size ksize(3, 3);
  cv::blur(mat, dst, ksize);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}
QImage ImageProcess::adaptiveMedianFilter(const QImage &inputImage,
                                          int maxWindowSize)

{
  auto start = std::chrono::high_resolution_clock::now();
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  myAdaptiveMedian(mat, dst, maxWindowSize);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  qDebug() << "adaptiveMedianFilter:" << duration.count() << "ms";
  return result;
}
QImage ImageProcess::nonLocalMeanFilter(const QImage &inputImage,
                                        int templateWindowSize,
                                        int searchWindowSize, double h) {
  auto start = std::chrono::high_resolution_clock::now();
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  // myNonLocalMeansDenoising(mat, dst, templateWindowSize, searchWindowSize, h,
  //                          5);
  cv::fastNlMeansDenoising(mat, dst, h, templateWindowSize, searchWindowSize);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  qDebug() << "nonLocalMeanFilter:" << duration.count() << "ms";
  return result;
}
void ImageProcess::myNonLocalMeansDenoising(const cv::Mat &input, cv::Mat &dst,
                                            int templateWindowSize,
                                            int searchWindowSize, double h,
                                            double sigma) {
  // 确保输入图像是灰度图
  cv::Mat gray;
  if (input.channels() == 3) {
    cv::cvtColor(input, gray, cv::COLOR_BGR2GRAY);
  } else {
    gray = input;
  }

  // 初始化目标图像
  dst = cv::Mat::zeros(gray.size(), gray.type());

  // 计算用于去噪的h的平方
  double h2 = h * h;

  // 遍历图像中的每个像素
  for (int i = 0; i < gray.rows; i++) {
    for (int j = 0; j < gray.cols; j++) {
      double weight_sum = 0.0;
      double pixel_value_sum = 0.0;

      // 遍历搜索窗口中的每个像素
      for (int k = -searchWindowSize / 2; k <= searchWindowSize / 2; k++) {
        for (int l = -searchWindowSize / 2; l <= searchWindowSize / 2; l++) {
          int x = i + l;
          int y = j + k;

          // 检查索引是否在图像边界内
          if (x >= 0 && x < gray.cols && y >= 0 && y < gray.rows) {
            double template_diff = 0.0;

            // 计算模板窗口内的加权欧氏距离
            for (int a = -templateWindowSize / 2; a <= templateWindowSize / 2;
                 a++) {
              for (int b = -templateWindowSize / 2; b <= templateWindowSize / 2;
                   b++) {
                int xa = x + a;
                int ya = y + b;

                // 检查模板索引是否在图像边界内
                if (xa >= 0 && xa < gray.rows && ya >= 0 && ya < gray.cols) {
                  double diff = gray.at<uchar>(x, y) - gray.at<uchar>(xa, ya);
                  template_diff += diff * diff;
                  // 计算权重
                  double weight = exp(-template_diff / h2);

                  // 累加权重和像素值
                  weight_sum += weight;
                  pixel_value_sum += weight * gray.at<uchar>(xa, ya);
                }
              }
            }
          }
        }
      }

      // 计算去噪后的像素值
      dst.at<uchar>(i, j) = cvRound(pixel_value_sum / weight_sum);
    }
  }
}

void ImageProcess::myAdaptiveMedian(const cv::Mat &input, cv::Mat &dst,
                                    int maxWindowSize) {
  dst = input.clone();
  int border = maxWindowSize / 2; // 计算边界宽度

  // 对图像进行扩展，使用反射边界进行填充
  cv::Mat paddedInput;
  cv::copyMakeBorder(input, paddedInput, border, border, border, border,
                     cv::BORDER_REFLECT);

  // 对扩展后的图像进行遍历
  for (int i = border; i < paddedInput.rows - border; ++i) {
    for (int j = border; j < paddedInput.cols - border; ++j) {
      int windowSize = 3; // 初始化窗口大小为3
      bool isModified = false;

      // 不断增大窗口，直到达到最大窗口大小或满足中值条件
      while (windowSize <= maxWindowSize && !isModified) {
        int halfSize = windowSize / 2;
        cv::Mat window =
            paddedInput(cv::Rect(j - halfSize, i - halfSize, windowSize,
                                 windowSize)); // 提取当前窗口

        double minVal, maxVal, medianVal;
        cv::minMaxLoc(window, &minVal, &maxVal); // 计算窗口内的最小、最大值
        medianVal = getMedian(window);           // 计算窗口内的中值

        uchar currentPixel = paddedInput.at<uchar>(i, j); // 获取当前像素值

        // 判断中值是否位于最小值和最大值之间
        if (minVal < medianVal && medianVal < maxVal) {
          // 判断当前像素是否位于最小值和最大值之间
          if (minVal < currentPixel && currentPixel < maxVal) {
            dst.at<uchar>(i - border, j - border) =
                currentPixel; // 使用原像素值
          } else {
            dst.at<uchar>(i - border, j - border) =
                static_cast<uchar>(medianVal); // 使用中值
          }
          isModified = true; // 标记已进行修改
        } else {
          windowSize += 2; // 增大窗口大小
        }
      }

      // 如果没有进行修改，则使用原像素值
      if (!isModified) {
        dst.at<uchar>(i - border, j - border) = paddedInput.at<uchar>(i, j);
      }
    }
  }
}

double ImageProcess::getMedian(cv::Mat &window) {
  int n = window.rows * window.cols / 2;
  std::vector<uchar> vec(window.begin<uchar>(), window.end<uchar>());
  std::nth_element(vec.begin(), vec.begin() + n, vec.end());
  return vec[n];
}

QImage
ImageProcess::enhanceImageUsingSecondDerivative(const QImage &inputImage) {
  // 将QImage转换为灰度图 CV_8UC1
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 定义拉普拉斯算子核
  cv::Mat kernel = (cv::Mat_<float>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);

  // 应用卷积操作
  cv::Mat laplacian;
  cv::filter2D(mat, laplacian, CV_16S, kernel);

  mat.convertTo(mat, CV_16S);
  cv::Mat sharpened = mat - laplacian;

  // 确保值在0-255之间
  sharpened.convertTo(sharpened, CV_8UC1, 1, 0);
  // 将增强后的图像转换为QImage
  QImage result =
      QImage((uchar *)sharpened.data, sharpened.cols, sharpened.rows,
             sharpened.step, QImage::Format_Grayscale8)
          .copy();
  return result;
}

QImage ImageProcess::unsharpMask(const QImage &inputImage, int kernelSize,
                                 double sigma, double k) {
  // 确保kernelSize是正数和奇数
  if (kernelSize % 2 == 0) {
    kernelSize += 1;
  }

  // 将QImage转换为灰度图 CV_8UC1
  cv::Mat originalMat;
  readImageToMat(inputImage, originalMat);

  // 创建高斯模糊图像
  cv::Mat blurredMat;
  cv::GaussianBlur(originalMat, blurredMat, cv::Size(kernelSize, kernelSize),
                   sigma);

  // 计算掩膜：原图像 - 模糊图像
  cv::Mat maskMat = originalMat - blurredMat;

  // 应用非锐化掩蔽：原图像 + k * 掩膜
  cv::Mat sharpMat;
  originalMat.convertTo(sharpMat, CV_16S); // 转换为16位有符号，以避免溢出
  maskMat.convertTo(maskMat, CV_16S);
  cv::addWeighted(sharpMat, 1.0, maskMat, k, 0, sharpMat);

  // 转换回8位图像
  sharpMat.convertTo(sharpMat, CV_8UC1);

  // 将cv::Mat转换回QImage
  QImage resultImage = QImage(sharpMat.data, sharpMat.cols, sharpMat.rows,
                              sharpMat.step, QImage::Format_Grayscale8)
                           .copy();

  return resultImage;
}

QImage ImageProcess::compressImageJPEG(const QImage &inputImage, int ratio) {
  // 将QImage转换为 cv::Mat
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 将图像转换为JPEG格式
  std::vector<uchar> buffer;
  std::vector<int> params;
  params.push_back(cv::IMWRITE_JPEG_QUALITY);
  params.push_back(ratio);
  cv::imencode(".jpg", mat, buffer, params);

  // 将压缩后的图像转换为QImage
  QImage result = QImage::fromData(buffer.data(), buffer.size());
  return result;
}
QImage convertJPEG2000ToQImage(const std::vector<uchar> &jpeg2000Data) {
  // 使用OpenCV解码JPEG2000数据
  cv::Mat decodedImage = cv::imdecode(jpeg2000Data, cv::IMREAD_UNCHANGED);

  // 将解码后的图像编码为BMP格式
  std::vector<uchar> bmpData;
  cv::imencode(".bmp", decodedImage, bmpData);

  // 使用BMP数据创建QImage
  QImage result =
      QImage::fromData(bmpData.data(), static_cast<int>(bmpData.size()));
  return result;
}

QImage ImageProcess::compressImageJPEG2000(const QImage &inputImage,
                                           int ratio) {
  // 将QImage转换为 cv::Mat
  cv::Mat mat;
  readImageToMat(inputImage, mat);
  ratio *= 10;
  // 将图像转换为JPEG2000格式
  std::vector<uchar> buffer;
  std::vector<int> params;
  params.push_back(cv::IMWRITE_JPEG2000_COMPRESSION_X1000);
  params.push_back(ratio);
  cv::imencode(".jp2", mat, buffer, params);

  // 将JPEG2000数据转换为QImage
  QImage result = convertJPEG2000ToQImage(buffer);
  return result;
}

QImage ImageProcess::erode(const QImage &inputImage, int kernelSize) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                              cv::Size(kernelSize, kernelSize));
  cv::erode(mat, dst, element);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}
QImage ImageProcess::dilate(const QImage &inputImage, int kernelSize) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  cv::Mat element = cv::getStructuringElement(cv::MORPH_CROSS,
                                              cv::Size(kernelSize, kernelSize));
  cv::dilate(mat, dst, element);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}

QImage ImageProcess::removeSmallComponents(const QImage &inputImage,
                                           int threshold) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  cv::Mat labelImage;
  std::map<int, int> map;
  int nLabels = myDetectConnectedComponents(mat, labelImage, map);
  for (int i = 1; i < nLabels + 1; i++) {
    if (map[i] < threshold) {
      for (int r = 0; r < mat.rows; r++) {
        for (int c = 0; c < mat.cols; c++) {
          if (labelImage.at<int>(r, c) == i) {
            mat.at<uchar>(r, c) = 0;
          }
        }
      }
    }
  }
  QImage result = QImage((uchar *)mat.data, mat.cols, mat.rows, mat.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}

int ImageProcess::myDetectConnectedComponents(const cv::Mat &input,
                                              cv::Mat &labels,
                                              std::map<int, int> &map) {
  cv::Mat mat = input.clone();

  labels = cv::Mat::zeros(mat.size(), CV_32S);
  int label = 1;
  // 遍历图像中的每个像素
  for (int r = 0; r < mat.rows; r++) {
    for (int c = 0; c < mat.cols; c++) {
      if (mat.at<uchar>(r, c) == 0) {
        continue;
      }
      // 如果当前像素没有被标记
      if (labels.at<int>(r, c) == 0) {
        // 使用深度优先搜索标记相邻的像素
        int num = 0;
        myDFS(mat, labels, r, c, num, label);
        map.insert(std::pair<int, int>(label, num));
        label++;
      }
    }
  }
  return label - 1;
}
void ImageProcess::myDFS(const cv::Mat &input, cv::Mat &labels, int r, int c,
                         int &num, int label) {
  // 检查索引是否在图像边界内
  if (r < 0 || r >= input.rows || c < 0 || c >= input.cols) {
    return;
  }

  // 检查像素是否已经被标记
  if (labels.at<int>(r, c) != 0) {
    return;
  }

  // 检查像素值是否为0
  if (input.at<uchar>(r, c) == 0) {
    return;
  }

  // 标记当前像素
  labels.at<int>(r, c) = label;
  num++;

  // 递归标记相邻的像素
  myDFS(input, labels, r - 1, c, num, label);
  myDFS(input, labels, r + 1, c, num, label);
  myDFS(input, labels, r, c - 1, num, label);
  myDFS(input, labels, r, c + 1, num, label);

  myDFS(input, labels, r - 1, c - 1, num, label);
  myDFS(input, labels, r - 1, c + 1, num, label);
  myDFS(input, labels, r + 1, c - 1, num, label);
  myDFS(input, labels, r + 1, c + 1, num, label);
}
QImage ImageProcess::globalThreshold(const QImage &inputImage, int threshold) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  cv::threshold(mat, dst, threshold, 255, cv::THRESH_BINARY);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}
QImage ImageProcess::otsuThreshold(const QImage &inputImage) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  // 计算直方图
  const int histSize = 256;
  std::vector<int> histogram(histSize, 0);
  for (int y = 0; y < mat.rows; y++) {
    for (int x = 0; x < mat.cols; x++) {
      histogram[mat.at<uchar>(y, x)]++;
    }
  }

  // 总像素数量
  int total = mat.rows * mat.cols;
  // 计算总灰度和
  float sum = 0;
  for (int t = 0; t < histSize; t++)
    sum += t * histogram[t];

  float sumB = 0;
  int wB = 0;
  int wF = 0;

  float varMax = 0;
  int threshold = 0;

  for (int t = 0; t < histSize; t++) {
    wB += histogram[t]; // 后景
    if (wB == 0)
      continue;

    wF = total - wB; // 前景
    if (wF == 0)
      break;

    sumB += (float)(t * histogram[t]);

    float mB = sumB / wB;         // 计算前景平均灰度
    float mF = (sum - sumB) / wF; // 计算后景平均灰度

    // 计算类间方差
    float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

    if (varBetween > varMax) {
      varMax = varBetween;
      threshold = t;
    }
  }

  cv::Mat thresholded;
  cv::threshold(mat, thresholded, threshold, 255, cv::THRESH_BINARY);

  QImage result =
      QImage((uchar *)thresholded.data, thresholded.cols, thresholded.rows,
             thresholded.step, QImage::Format_Grayscale8)
          .copy();
  return result;
}

QImage ImageProcess::cannyEdgeDetection(const QImage &inputImage,
                                        int threshold1, int threshold2) {
  cv::Mat mat;
  readImageToMat(inputImage, mat);

  cv::Mat dst;
  cv::Canny(mat, dst, threshold1, threshold2);

  QImage result = QImage((uchar *)dst.data, dst.cols, dst.rows, dst.step,
                         QImage::Format_Grayscale8)
                      .copy();
  return result;
}

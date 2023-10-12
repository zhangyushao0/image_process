#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include "ImageProvider.h"
#include <opencv2/opencv.hpp>

class ImageProcess {
private:
public:
  ImageProcess() {}
  QImage processFourierTransform(const QImage &inputImage) {
    // 将QImage转换为cv::Mat
    cv::Mat mat = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC4,
                          (uchar *)inputImage.bits());

    // 转换为灰度图
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2GRAY);

    // 将图像转换为浮点型
    mat.convertTo(mat, CV_32F);

    // 执行傅里叶变换
    cv::Mat planes[] = {cv::Mat_<float>(mat),
                        cv::Mat::zeros(mat.size(), CV_32F)};
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
  QImage displayHistogram(const QImage &inputImage) {
    cv::Mat mat = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC4,
                          (uchar *)inputImage.bits());
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2GRAY);

    int histSize = 256;
    float range[] = {0, 256};
    const float *histRange = {range};
    bool uniform = true, accumulate = false;
    cv::Mat hist;
    cv::calcHist(&mat, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform,
                 accumulate);

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));

    cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1,
                  cv::Mat());

    for (int i = 1; i < histSize; i++) {
      cv::line(
          histImage,
          cv::Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
          cv::Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
          cv::Scalar(255, 0, 0), 2, 8, 0);
    }

    QImage result =
        QImage((uchar *)histImage.data, histImage.cols, histImage.rows,
               histImage.step, QImage::Format_RGB888)
            .copy();
    return result;
  }
  QImage histogramEqualization(const QImage &inputImage) {
    cv::Mat mat = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC4,
                          (uchar *)inputImage.bits());
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2GRAY);

    cv::equalizeHist(mat, mat);
    QImage result = QImage((uchar *)mat.data, mat.cols, mat.rows, mat.step,
                           QImage::Format_Grayscale8)
                        .copy();
    return result;
  }
  QImage applyCLAHE(const QImage &inputImage, double clipLimit = 2.0,
                    int tileGridSize = 8) {
    cv::Mat mat = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC4,
                          (uchar *)inputImage.bits());
    cv::cvtColor(mat, mat, cv::COLOR_BGRA2GRAY);

    cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    clahe->setClipLimit(clipLimit);
    clahe->setTilesGridSize(cv::Size(tileGridSize, tileGridSize));
    clahe->apply(mat, mat);

    QImage result = QImage((uchar *)mat.data, mat.cols, mat.rows, mat.step,
                           QImage::Format_Grayscale8)
                        .copy();
    return result;
  }
};
#endif // IMAGEPROCESS_H

#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <QImage>
#include <chrono>
#include <opencv2/opencv.hpp>
#include <qimage.h>

class ImageProcess {
private:
public:
  ImageProcess();
  // 傅里叶变换
  QImage fourierTransform(const QImage &inputImage);
  // 直方图
  QImage displayHistogram(const QImage &inputImage);
  // 直方图均衡化
  QImage histogramEqualization(const QImage &inputImage);
  // 使用CLAHE算法进行直方图均衡化
  QImage applyCLAHE(const QImage &inputImage, float clipLimit = 2.0,
                    int tileGridSize = 8);
  QImage addGaussianNoise(const QImage &inputImage, double mean = 0,
                          double sigma = 50);
  QImage medianBlurFilter(const QImage &inputImage, int kernelSize = 3);
  QImage meanBlurFilter(const QImage &inputImage, int kernelSize = 3);
  QImage nonLocalMeanFilter(const QImage &inputImage,
                            int templateWindowSize = 7,
                            int searchWindowSize = 21, double h = 50.0);

private:
  // 计算直方图
  void myCalcHist(const cv::Mat &src, cv::Mat &hist, int histSize);
  // 直方图均衡化
  void myEqualizeHist(const cv::Mat &src, cv::Mat &dst);
  // 使用CLAHE算法进行直方图均衡化
  void myCreateCLAHE(const cv::Mat &src, cv::Mat &dst, int tileSize,
                     float clipLimit);
  // 使用带对比度限制的直方图均衡化
  void myEqualizeHistWithClipLimit(const cv::Mat &src, cv::Mat &dst,
                                   float clipLimit);
  // 将QImage转换为Mat
  void readImageToMat(const QImage &inputImage, cv::Mat &mat);
  void myNonLocalMeansDenoising(const cv::Mat &src, cv::Mat &dst,
                                int templateWindowSize, int searchWindowSize,
                                double h, double sigma);
};
#endif // IMAGEPROCESS_H

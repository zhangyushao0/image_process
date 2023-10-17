#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <QImage>
#include <opencv2/opencv.hpp>

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
};
#endif // IMAGEPROCESS_H

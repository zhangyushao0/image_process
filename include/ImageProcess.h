#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <QImage>
#include <opencv2/opencv.hpp>

class ImageProcess {
private:
public:
  ImageProcess();
  QImage fourierTransform(const QImage &inputImage);
  QImage displayHistogram(const QImage &inputImage);
  QImage histogramEqualization(const QImage &inputImage);
  QImage applyCLAHE(const QImage &inputImage, float clipLimit = 0.2,
                    int tileGridSize = 16);

private:
  void myCalcHist(const cv::Mat &src, cv::Mat &hist, int histSize);
  void myEqualizeHist(const cv::Mat &src, cv::Mat &dst);
  void myCreateCLAHE(const cv::Mat &src, cv::Mat &dst, int tileSize,
                     float clipLimit);
  void myEqualizeHistWithClipLimit(const cv::Mat &src, cv::Mat &dst,
                                   float clipLimit);
};
#endif // IMAGEPROCESS_H

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
  QImage applyCLAHE(const QImage &inputImage, int clipLimit = 2,
                    int tileGridSize = 8);

private:
  void myCalcHist(const cv::Mat &src, cv::Mat &hist, int histSize);
  void myEqualizeHist(const cv::Mat &src, cv::Mat &dst);
  void myCreateCLAHE(const cv::Mat &src, cv::Mat &dst, int tileSize,
                     int clipLimit);
  void myEqualizeHistWithClipLimit(const cv::Mat &src, cv::Mat &dst,
                                   int clipLimit);
};
#endif // IMAGEPROCESS_H

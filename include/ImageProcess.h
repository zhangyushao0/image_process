#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <opencv2/opencv.hpp>
#include "ImageProvider.h"

class ImageProcess
{
private:
public:
    ImageProcess() {}
    QImage processFourierTransform(const QImage &inputImage)
    {
        // 将QImage转换为cv::Mat
        cv::Mat mat = cv::Mat(inputImage.height(), inputImage.width(), CV_8UC4, (uchar *)inputImage.bits(), inputImage.bytesPerLine());

        // 转换为灰度图
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2GRAY);

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

        // 对数尺度(logarithmic scale)缩放
        mag += cv::Scalar::all(1);
        cv::log(mag, mag);

        // 剪切和重分布图像频谱
        cv::normalize(mag, mag, 0, 255, cv::NORM_MINMAX);
        mag.convertTo(mag, CV_8U);

        // 转换为QImage
        cv::cvtColor(mag, mag, cv::COLOR_GRAY2RGB);
        QImage result = QImage((uchar*)mag.data, mag.cols, mag.rows, mag.step, QImage::Format_RGB888).copy();

        return result;
    }
};
#endif // IMAGEPROCESS_H

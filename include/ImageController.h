#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H
#include "ImageProcess.h"
#include "ImageProvider.h"
#include <QImage>
#include <QObject>
#include <QUrl>
#include <qimage.h>
#include <qsize.h>
#include <qtmetamacros.h>
#include <qurl.h>

class ImageController : public QObject {
  Q_OBJECT
private:
  ImageProvider imageProvider;
  ImageProcess imageProcess;

public:
  explicit ImageController(QObject *parent = nullptr) : QObject(parent) {}
  ImageProvider *getImageProvider() { return &imageProvider; }
  Q_INVOKABLE void ft(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.fourierTransform(image),
                           url.toString() + "ft");
  }
  Q_INVOKABLE void displayHistogram(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.displayHistogram(image),
                           url.toString() + "displayHistogram");
  }
  Q_INVOKABLE void histogramEqualization(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.histogramEqualization(image),
                           url.toString() + "histogramEqualization");
  }
  Q_INVOKABLE void applyCLAHE(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.applyCLAHE(image),
                           url.toString() + "applyCLAHE");
  }
  Q_INVOKABLE void loadImage(const QUrl &url) {
    QSize size;
    QImage image(url.toLocalFile());
    imageProvider.setImage(image, url.toString());
  }
  Q_INVOKABLE void saveImage(QString url, QUrl path) {
    QSize size;
    QImage image = imageProvider.requestImage(url, nullptr, size);
    image.save(path.toLocalFile());
  }
  Q_INVOKABLE void createAllOneGrayImage(int width, int height) {
    QImage image(width, height, QImage::Format_Grayscale8);
    image.fill(Qt::white);
    imageProvider.setImage(image, "allOneGrayImage");
  }
  Q_INVOKABLE void addGaussianNoise(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.addGaussianNoise(image),
                           url.toString() + "addGaussianNoise");
  }
  Q_INVOKABLE void addSaltAndPepperNoise(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.addSaltAndPepperNoise(image),
                           url.toString() + "addSaltAndPepperNoise");
  }
  Q_INVOKABLE void medianBlurFilter(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.medianBlurFilter(image),
                           url.toString() + "medianBlurFilter");
  }
  Q_INVOKABLE void meanBlurFilter(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.meanBlurFilter(image),
                           url.toString() + "meanBlurFilter");
  }
  Q_INVOKABLE void adaptiveMedianFilter(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.adaptiveMedianFilter(image),
                           url.toString() + "adaptiveMedianFilter");
  }
  Q_INVOKABLE void nonLocalMeanFilter(const QUrl &url) {
    QSize size;
    QImage image = imageProvider.requestImage(url.toString(), nullptr, size);
    imageProvider.setImage(imageProcess.nonLocalMeanFilter(image),
                           url.toString() + "nonLocalMeanFilter");
  }
};
#endif // IMAGECONTROLLER_H

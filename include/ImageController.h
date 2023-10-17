#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H
#include "ImageProcess.h"
#include "ImageProvider.h"
#include <QImage>
#include <QObject>
#include <QUrl>
#include <qimage.h>
#include <qtmetamacros.h>

class ImageController : public QObject {
  Q_OBJECT
private:
  ImageProvider imageProvider;
  ImageProcess imageProcess;

public:
  explicit ImageController(QObject *parent = nullptr) : QObject(parent) {}
  ImageProvider *getImageProvider() { return &imageProvider; }
  Q_INVOKABLE void ft(const QUrl &url) {
    QImage image(url.toLocalFile());
    imageProvider.setImage(imageProcess.fourierTransform(image),
                           url.toString() + "ft");
  }
  Q_INVOKABLE void displayHistogram(const QUrl &url) {
    QImage image(url.toLocalFile());
    imageProvider.setImage(imageProcess.displayHistogram(image),
                           url.toString() + "displayHistogram");
  }
  Q_INVOKABLE void histogramEqualization(const QUrl &url) {
    QImage image(url.toLocalFile());
    imageProvider.setImage(imageProcess.histogramEqualization(image),
                           url.toString() + "histogramEqualization");
  }
  Q_INVOKABLE void applyCLAHE(const QUrl &url) {
    QImage image(url.toLocalFile());
    imageProvider.setImage(imageProcess.applyCLAHE(image),
                           url.toString() + "applyCLAHE");
  }
  Q_INVOKABLE void processAfter(QString id) {
    QSize size;
    QImage image = imageProvider.requestImage(id, &size, size);
    imageProvider.setImage(imageProcess.histogramEqualization(image),
                           id + "after");
  }
};
#endif // IMAGECONTROLLER_H

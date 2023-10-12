#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H
#include <QObject>
#include <QImage>
#include <QUrl>
#include "ImageProvider.h"
#include "ImageProcess.h"

class ImageController : public QObject
{
    Q_OBJECT
private:
    ImageProvider imageProvider;
    ImageProcess imageProcess;

public:
    explicit ImageController(QObject *parent = nullptr) : QObject(parent) {}
    ImageProvider *getImageProvider() { return &imageProvider; }
    Q_INVOKABLE void ft(const QUrl &url)
    {
        QImage image(url.toLocalFile());
        imageProvider.setImage(imageProcess.processFourierTransform(image),url);
    }
};
#endif // IMAGECONTROLLER_H

#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H
#include <QImage>
#include <QQuickImageProvider>
class ImageProvider : public QQuickImageProvider
{
private:
    QHash<QUrl,QImage>imageMap;

public:
    ImageProvider() : QQuickImageProvider(QQuickImageProvider::Image)
    {
    }
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) override
    {
        Q_UNUSED(size)
        Q_UNUSED(requestedSize)
        return imageMap.value(QUrl(id));
    }
    void setImage(const QImage image,const QUrl url)
    {
        imageMap.insert(url,image);
    }
};
#endif // IMAGEPROVIDER_H

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QUrl>
#include "ImageController.h"
int main(int argc, char *argv[])
{

    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    ImageController imageController;
    engine.rootContext()->setContextProperty("imageController", &imageController);
    engine.addImageProvider("imageProvider", imageController.getImageProvider());
    const QUrl url(u"qrc:/image_process/Main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreationFailed,
        &app, []()
        { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}

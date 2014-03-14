#include "qapplicationstartermodule.h"
#include <QApplication>
#include <QObject>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QSurfaceFormat>

QApplicationStarterModule::QApplicationStarterModule()
{
}

int QApplicationStarterModule::Start(int argc, char *argv[])
{
 //   QApplication app(argc, argv);

    // Register engine with QML.
    //qmlRegisterType<SpecialScriptProvider>("de.bulla.iocengine", 1, 0, "");

    int rc = 0;

//    QQmlEngine engine;
//    QQmlComponent *component = new QQmlComponent(&engine);

//    QObject::connect(&engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

//    component->loadUrl(QUrl("qml/3DProject/main.qml"));

//    if (!component->isReady() ) {
//        qWarning("%s", qPrintable(component->errorString()));
//        return -1;
//    }

//    QObject *topLevel = component->create();
//    QQuickWindow *window = qobject_cast<QQuickWindow *>(topLevel);

//    QSurfaceFormat surfaceFormat = window->requestedFormat();
//    window->setFormat(surfaceFormat);
//    window->show();


    started();
 //   rc = app.exec();

//    delete component;
    return rc;
}

void QApplicationStarterModule::PublishServices()
{

}

void QApplicationStarterModule::Activated()
{

}

void QApplicationStarterModule::Deactivated()
{

}

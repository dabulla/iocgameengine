#include "qtquickinputeventswindow.h"
#include "engine.h"
#include "qapplicationstartermodule.h"
#include <QApplication>

void addSceneModules(IocContext& engine)
{
    engine.Set<IRenderSurface>(new QtQuickInputEventsWindow())
            .alias<IScriptProvider>();
    QString name = "QmlSource";
    QString value = "qml/3DProject/main.qml";
    engine.Set<QString>(value, name, true);

    QString* test0 = engine.GetImmediate<QString>(new QString("defaultValu"), name);
    IRenderSurface* test1 = engine.GetImmediate<IRenderSurface>();
    IScriptProvider* test2 = engine.GetImmediate<IScriptProvider>();
    QtQuickInputEventsWindow* test3 = engine.GetImmediate<QtQuickInputEventsWindow>();
    //engine.Set<IStarter>(new QApplicationStarterModule);
}


int main(int argc, char *argv[])
{
    int rc = 0;
    QApplication app(argc, argv);
    IocContext engine;
    addSceneModules(engine);
    rc |= engine.Start(argc, argv);
    rc |= app.exec();
    return rc;
}



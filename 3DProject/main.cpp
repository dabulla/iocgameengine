#include "qtquickinputeventswindow.h"
#include "engine.h"
#include "qapplicationstartermodule.h"
#include <QApplication>

void addSceneModules(Engine& engine)
{
    engine.Set<IRenderSurface>(new QtQuickInputEventsWindow())
            .alias<IScriptProvider>();
    engine.Set<QString>(QString::fromUtf8("QmlSource"), QString::fromUtf8("qml/3DProject/main.qml"));
    //engine.Set<IStarter>(new QApplicationStarterModule);
}


int main(int argc, char *argv[])
{
    int rc = 0;
    QApplication app(argc, argv);
    Engine engine;
    addSceneModules(engine);
    rc |= engine.Start(argc, argv);
    rc |= app.exec();
    return rc;
}



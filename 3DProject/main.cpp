#include "qtquickinputeventswindow.h"
#include "engine.h"
#include "qapplicationstartermodule.h"
#include <QApplication>

void addSceneModules(Engine& engine)
{
    engine.Set<IRenderSurface>(new QtQuickInputEventsWindow())
            .alias<IScriptProvider>();
    QString name = "QmlSource";
    QString value = "qml/3DProject/main.qml";
    engine.Set<QString>(name, value, true);
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



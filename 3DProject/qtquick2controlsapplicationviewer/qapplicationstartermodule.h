#ifndef QAPPLICATIONSTARTERMODULE_H
#define QAPPLICATIONSTARTERMODULE_H

#include <engine.h>

class QApplicationStarterModule : public IStarting, public IEngineObject
{
public:
    QApplicationStarterModule();

    // IStarter interface
private:
    int Start(int argc, char *argv[]);

    // IEngineObject interface
public:
    void PublishServices();
    void Activated();
    void Deactivated();
};

#endif // QAPPLICATIONSTARTERMODULE_H

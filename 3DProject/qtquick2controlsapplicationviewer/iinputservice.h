#ifndef IINPUTSERVICE_H
#define IINPUTSERVICE_H

#include <qevent.h>
#include <boost/signals2.hpp>
#include "engine.h"

using namespace boost::signals2;

class IInputService
{
public:
    IInputService();

    boost::signals2::signal<void(QKeyEvent*)> keyPress;
    boost::signals2::signal<void(QKeyEvent*)> keyRelease;
    boost::signals2::signal<void(QMouseEvent*)> mousePress;
    boost::signals2::signal<void(QMouseEvent*)> mouseRelease;
    boost::signals2::signal<void(QMouseEvent*)> mouseDoubleClick;
    boost::signals2::signal<void(QMouseEvent*)> mouseMove;
    boost::signals2::signal<void(QWheelEvent*)> wheel;
};

#endif // IINPUTSERVICE_H

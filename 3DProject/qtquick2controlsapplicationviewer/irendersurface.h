#ifndef IRENDERSURFACE_H
#define IRENDERSURFACE_H

#include <QObject>
#include <QtGui/qopengl.h>
#include <QSurfaceFormat>
#include <boost/signals2.hpp>
#include "engine.h"

/**
 * @brief The IRenderSurface class Maybe a better name will be "GlContextProvider"
 */
class IRenderSurface
{
//    Q_OBJECT
public:
    IRenderSurface();
    // Get the surface renderformat
    virtual const QSurfaceFormat GetFormat() = 0;
    virtual const QOpenGLContext* GetOpenGlContext() const = 0;

    boost::signals2::signal<void(int, int)> resize;
    boost::signals2::signal<void()> render;
//signals:

//    void resize(int width, int height);
//    void update(float tickcount);
//    void render();
};

#endif // IRENDERSURFACE_H

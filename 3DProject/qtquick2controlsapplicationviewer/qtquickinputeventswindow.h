#ifndef QQUICKINPUTEVENTSWINDOW_H
#define QQUICKINPUTEVENTSWINDOW_H

#include <QQuickWindow>
#include <QQuickView>
#include "irendersurface.h"
#include "iscriptprovider.h"
#include "iinputservice.h"
#include "iupdater.h"
#include "iupdating.h"
#include "autowiredptr.h"

class QtQuickInputEventsWindow : public QQuickView, public IRenderSurface, public IScriptProvider, public IUpdater, public IInputService, public IEngineObject
{
    Q_OBJECT
private:
    AutowiredPtr<IUpdating> m_allUpdating;
    void setMainQmlFile(const QString &file);
    void addImportPath(const QString &path);
    void emitUpdate();
    void emitResize(int w, int h);
    void emitRender();
    IInputService *m_pGlobalInputService;
public:
    QtQuickInputEventsWindow();
    virtual ~QtQuickInputEventsWindow();


    void keyPressEvent( QKeyEvent* e );
    void keyReleaseEvent( QKeyEvent* e );
    void mousePressEvent( QMouseEvent* e );
    void mouseReleaseEvent( QMouseEvent* e );
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mouseMoveEvent( QMouseEvent* e );
    void wheelEvent( QWheelEvent* e );

    // IEngineObject interface
public:
    virtual void PublishServices();
    virtual void Activated();
    virtual void Deactivated();

    // IRenderSurface interface
public:
    virtual const QOpenGLContext *GetOpenGlContext() const;
    virtual const QSurfaceFormat GetFormat();

//signals:
//    //handle input before the window. Use DirectConnection to communicate back, if the input was handled by content
//    void keyPress( QKeyEvent* e, bool *pHandled );
//    void keyRelease( QKeyEvent* e, bool *pHandled );
//    void mousePress( QMouseEvent* e, bool *pHandled );
//    void mouseRelease( QMouseEvent* e, bool *pHandled );
//    void mouseDoubleClick(QMouseEvent* e, bool *pHandled );
//    void mouseMove( QMouseEvent* e, bool *pHandled );
//    void wheel( QWheelEvent* e, bool *pHandled );

//    //handle input after the window had it's chance to handle the input. E.g. if a button in the gui was clicked, the contentMousePress signal is not fired.
//    void contentKeyPress( QKeyEvent* e );
//    void contentKeyRelease( QKeyEvent* e );
//    void contentMousePress( QMouseEvent* e );
//    void contentMouseRelease( QMouseEvent* e );
//    void contentMouseDoubleClick(QMouseEvent *);
//    void contentMouseMove( QMouseEvent* e );
//    void contentWheel( QWheelEvent* e );

private:
    class QtQuickInputEventsWindowPrivate *d;
};

#endif // QQUICKINPUTEVENTSWINDOW_H

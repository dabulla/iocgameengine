#include "qtquickinputeventswindow.h"

#include <QCoreApplication>
#include <QDir>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickView>
#include <QOpenGLContext>
#include <QTime>
#include <QTimer>

#include "engine.h"

class QtQuickInputEventsWindowPrivate
{
    QString mainQmlFile;
    QQmlEngine engine;

    QTime m_time;

    QtQuickInputEventsWindowPrivate()
    {}

    ~QtQuickInputEventsWindowPrivate()
    {
    }

    static QString adjustPath(const QString &path);

    friend class QtQuickInputEventsWindow;
private slots:
    void resize()
    {

    }
};

QtQuickInputEventsWindow::QtQuickInputEventsWindow()
: QQuickView(), d(new QtQuickInputEventsWindowPrivate())
{
}

QtQuickInputEventsWindow::~QtQuickInputEventsWindow()
{
    delete d;
}

QString QtQuickInputEventsWindowPrivate::adjustPath(const QString &path)
{
#if defined(Q_OS_MAC)
    if (!QDir::isAbsolutePath(path))
        return QStringLiteral("%1/../Resources/%2")
                .arg(QCoreApplication::applicationDirPath(), path);
#elif defined(Q_OS_BLACKBERRY)
    if (!QDir::isAbsolutePath(path))
        return QStringLiteral("app/native/%1").arg(path);
#elif !defined(Q_OS_ANDROID)
    QString pathInInstallDir =
            QStringLiteral("%1/../%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
    pathInInstallDir =
            QStringLiteral("%1/%2").arg(QCoreApplication::applicationDirPath(), path);
    if (QFileInfo(pathInInstallDir).exists())
        return pathInInstallDir;
#endif
    return path;
}


void QtQuickInputEventsWindow::setMainQmlFile(const QString &file)
{
    d->mainQmlFile = QtQuickInputEventsWindowPrivate::adjustPath(file);

    QQmlComponent component(&d->engine);

    QObject::connect(&d->engine, SIGNAL(quit()), QCoreApplication::instance(), SLOT(quit()));

#ifdef Q_OS_ANDROID
    component.loadUrl(QUrl(QStringLiteral("assets:/")+d->mainQmlFile));
#else
    component.loadUrl(QUrl::fromLocalFile(d->mainQmlFile));
#endif

    if (!component.isReady())
        qWarning("%s", qPrintable(component.errorString()));

    //d->window = qobject_cast<QtQuickInputEventsWindow *>(component.create());
    //if (!d->window)
    //    qFatal("Error: Your root item has to be a Window.");

    d->engine.setIncubationController(incubationController());
}

void QtQuickInputEventsWindow::addImportPath(const QString &path)
{
    d->engine.addImportPath(QtQuickInputEventsWindowPrivate::adjustPath(path));
}

void QtQuickInputEventsWindow::emitUpdate()
{
    //TODO: tickcount
    IUpdater::update(0.05);
}

void QtQuickInputEventsWindow::emitResize(int width, int height)
{
    IRenderSurface::resize(width, height);
//    emit resize(width, height);
}

void QtQuickInputEventsWindow::emitRender()
{
    render();
//    emit render();
}

void QtQuickInputEventsWindow::PublishServices()
{
    QSurfaceFormat *format = new QSurfaceFormat;
    //QSurfaceFormat format;
    format->setVersion( 4, 2 ); //Set OpenGL 4.2
    //Enable AntiAliasing
    format->setSamples( 4 );
    //Make sure there is a depth and alpha buffer
    format->setDepthBufferSize( 24 );
    format->setAlphaBufferSize(8);
    // CompatibilityProfile is used so QtQuick works together with OpenGL 4.2
    // OpenGL 4.2 is used to use nvidia nsight in its current version (only 4.2 supported)
    // Qt makes certain OpenGL-Calls that nsight does not like. Even if the UI is completely disabled, nsight does not work yet.
    format->setProfile( QSurfaceFormat::CompatibilityProfile );
    GetEngine().Set<QSurfaceFormat>(format);

    if(this == &GetEngine().GetImmediate<IInputService>())
    {
        //Todo:call
        m_pGlobalInputService = GetEngine().Set<IInputService>(new IInputService, "GlobalInput");
    }
    else
    {
        m_pGlobalInputService = (IInputService*)0;
    }
//    GetEngine().Get<IInputService>([this](const QString &name, IInputService &obj)
//    {
//        GetEngine().Set<IInputService>(new IInputService, "GlobalInput");
//    });
}

void QtQuickInputEventsWindow::Activated()
{
    setTitle( GetEngine().GetString("title", "[add title]") );
    //Make the UI adopt to the Window on resize
    //setResizeMode(QQuickView::SizeRootObjectToView);
    // Tell Qt we will use OpenGL for this window
    setSurfaceType( OpenGLSurface );
    //Qt should not clear the screen before rendering (this clear our renderings)
    setClearBeforeRendering(false);
    //root object should be transparent. This affects the glClearColor (verification needed)
    setColor(QColor(Qt::transparent));

    setFormat( GetEngine().GetImmediate<QSurfaceFormat>() );

    QString qmlSource = GetEngine().GetImmediate<QString>("QmlSource");
    setSource(qmlSource);
    //d->window->openglContext()->makeCurrent(d->window);
    d->m_time.start();

    QQuickWindow::resize( GetEngine().GetInt("width", 1366), GetEngine().GetInt("height", 768) );


    connect( this, SIGNAL( widthChanged( int ) ), this, SLOT( emitResize() ) );
    connect( this, SIGNAL( heightChanged( int ) ), this, SLOT( emitResize() ) );

    // This timer drives the scene updates
    QTimer* timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( emitUpdate() ), Qt::DirectConnection );
    timer->start( 1 );

    //Use "beforeRendering" to render a scene under the ui.
    connect( this, SIGNAL( beforeRendering() ), this, SLOT( emitRender() ), Qt::DirectConnection );

    //Initialize all stuff that need OpenGL+QtQuickUI.
    connect(this, SIGNAL(sceneGraphInitialized()), this, SLOT(emitSceneGraphInitialized()), Qt::DirectConnection );
}

void QtQuickInputEventsWindow::Deactivated()
{

}

const QOpenGLContext *QtQuickInputEventsWindow::GetOpenGlContext() const
{
    return openglContext();
}
const QSurfaceFormat QtQuickInputEventsWindow::GetFormat()
{
    return QQuickWindow::format();
}

void QtQuickInputEventsWindow::keyPressEvent(QKeyEvent *e)
{
    if(m_pGlobalInputService)
    {
        m_pGlobalInputService->keyPress( e );
    }
    if(!e->isAccepted())
    {
        QQuickWindow::keyPressEvent(e);
        if(e->isAccepted())
        {
            keyPress( e );
        }
    }
}

void QtQuickInputEventsWindow::keyReleaseEvent(QKeyEvent *e)
{
    if(m_pGlobalInputService)
    {
        m_pGlobalInputService->keyRelease( e );
    }
    if(!e->isAccepted())
    {
        QQuickWindow::keyReleaseEvent(e);
        if(e->isAccepted())
        {
            keyRelease( e );
        }
    }
}

void QtQuickInputEventsWindow::mousePressEvent(QMouseEvent *e)
{
    if(m_pGlobalInputService)
    {
        m_pGlobalInputService->mousePress( e );
    }
    if(!e->isAccepted())
    {
        QQuickWindow::mousePressEvent(e);
        if(e->isAccepted())
        {
            mousePress( e );
        }
    }
}

void QtQuickInputEventsWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(m_pGlobalInputService)
    {
        m_pGlobalInputService->mouseRelease( e );
    }
    if(!e->isAccepted())
    {
        QQuickWindow::mouseReleaseEvent(e);
        if(e->isAccepted())
        {
            mouseRelease( e );
        }
    }
}

void QtQuickInputEventsWindow::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(m_pGlobalInputService)
    {
        m_pGlobalInputService->mouseDoubleClick( e );
    }
    if(!e->isAccepted())
    {
        QQuickWindow::mouseDoubleClickEvent(e);
        if(e->isAccepted())
        {
            mouseDoubleClick( e );
        }
    }
}

void QtQuickInputEventsWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(m_pGlobalInputService)
    {
        m_pGlobalInputService->mouseMove( e );
    }
    if(!e->isAccepted())
    {
        QQuickWindow::mouseMoveEvent(e);
        if(e->isAccepted())
        {
            mouseMove( e );
        }
    }
}

void QtQuickInputEventsWindow::wheelEvent(QWheelEvent *e)
{
    if(m_pGlobalInputService)
    {
        m_pGlobalInputService->wheel( e );
    }
    if(!e->isAccepted())
    {
        QQuickWindow::wheelEvent(e);
        if(e->isAccepted())
        {
            wheel( e );
        }
    }
}

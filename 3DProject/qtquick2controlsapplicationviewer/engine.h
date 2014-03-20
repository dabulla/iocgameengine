#ifndef ENGINE_H
#define ENGINE_H

#include <QString>
#include <QVector3D>
#include <QMatrix4x4>
#include <boost/shared_ptr.hpp>
#include <boost/signals2.hpp>
#include <functional>
#include "iioccontext.h"

#define IIocContext IocContext


#define STD_OBJ_NAME "__st"

enum CollectionChange
{
    COLLECTION_ADDED,
    COLLECTION_REMOVED, //<TODO
    COLLECTION_REPLACED //<TODO
};

#define listener_t_templated std::function<void(const QString &name, T &obj)>
#define listener_list_t_templated std::function<void(const QString &name, T *pObj, const CollectionChange change)>

class IRenderable;
class ICamera;
class IMaterial;
class IocContext;

typedef std::function<void(IIocContext*)> __AutowiredPtrContainer;
class IEngineObjectPrivate;
class IEngineObject {
public:
    IEngineObject();
    virtual ~IEngineObject();
    virtual void PublishServices() = 0;
    virtual void Activated() = 0;
    virtual void Deactivated() = 0;
    void __AutowiredPtr_add(__AutowiredPtrContainer fn);
protected:
    IIocContext& GetIocContext() const;
    QList<QString> GetNames() const;
private:
    IEngineObjectPrivate *d;
    friend class IIocContext;
};

struct Basic3DScene {
public:
    QList<IRenderable> renderables;
    ICamera *pCcamera;
};

class IStarting
{
public:
    virtual int Start(int argc, char *argv[]) = 0;
    boost::signals2::signal<void()> started;
};


template < class T >
class ModuleLazyChain;

class IocContext //: public IIocContext
{
private:
    class IocContextPrivate *d;
public:
    IocContext();
    ~IocContext();
    void Update();
    void Render();
    int Start(int argc, char *argv[]);

    template < class T >
    T* GetImmediateOrRegisterDefault(T *defaultValue, const QString &name, const bool bExternal);
    template < class T >
    T *GetImmediateOrRegisterDefault(T& defaultValue, const QString &name);
    template < class T >
    T *GetImmediate(const QString &name);
    template < class T >
    void Get(listener_t_templated loaded, const QString &name);
    template < class T >
    QList<T*> GetAllImmediate();
    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > RegisterBean(SrcT *ptr, const QString &name, bool bDeleteOnRemove, const bool bUseCopy);
    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > RegisterBean(SrcT &obj, const QString &name, bool bUseCopy);

    //const reference: updates when new objects are added. GetAll<IRenderable>() must only be called once.
    template < class T >
    void GetAll(listener_list_t_templated loaded);
    const QString& GetString(const QString &name) const;
    const QString& GetString(const QString &name, const QString &valDefault);
    void SetString(const QString &name, QString value);
    const int& GetInt(const QString &name) const;
    const int& GetInt(const QString &name, const int &valDefault);
    void SetInt(const QString &name, int value);
    const float& GetFloat(const QString &name) const;
    const float& GetFloat(const QString &name, const float &valDefault);
    void SetFloat(const QString &name, float value);
    const QVector3D& GetVector3f(const QString &name) const;
    const QVector3D& GetVector3f(const QString &name, const QVector3D &valDefault);
    void SetVector3f(const QString &name, QVector3D value);
    const QMatrix4x4& GetMatrix4f(const QString &name) const;
    const QMatrix4x4& GetMatrix4f(const QString &name, const QMatrix4x4 &valDefault);
    void SetMatrix4f(const QString &name, QMatrix4x4 value);
};

template < class T >
class ModuleLazyChain
{
private:
    T* m_pRef;
    IIocContext *m_pEngine;
    ModuleLazyChain(T *ptr, IIocContext *engine);
    friend class IIocContext;
public:
    ModuleLazyChain< T > alias(const QString &name);

//    template < class NewT >
//    ModuleLazyChain< NewT > alias(const QString &name = STD_OBJ_NAME );
    operator T*() {return m_pRef;}
};
class IocChildContext
{
    IocChildContext(const QString conextName);
    template < class T >
    T* GetImmediateOrRegisterDefault(T *defaultValue, const QString &name, const bool bExternal);
    template < class T >
    T *GetImmediateOrRegisterDefault(T& defaultValue, const QString &name);
    template < class T >
    T *GetImmediate(const QString &name);
    template < class T >
    void Get(listener_t_templated loaded, const QString &name);
    template < class T >
    QList<T*> GetAllImmediate();
    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > RegisterBean(SrcT *ptr, const QString &name, bool bDeleteOnRemove);
    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > RegisterBean(SrcT &obj, const QString &name, bool bUseCopy);
};
#include "engine_impl.h"

#endif // ENGINE_H

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
protected:
    IIocContext& GetIocContext() const;
    QList<QString> GetNames() const;
private:
    void __AutowiredPtr_add(__AutowiredPtrContainer fn);
    IEngineObjectPrivate *d;
    friend class IIocContext;

    friend class AutowiredHelper;
};

class IStarting
{
public:
    virtual int Start(int argc, char *argv[]) = 0;
};


template < class T >
class ModuleLazyChain;

class IocContext //: public IIocContext
{
private:
    class IocContextPrivate *d;
public:
    IocContext(bool registerInterfacesOnParent);
    ~IocContext();
    /**
     * @brief Starts the IocContext. Resolves dependencies.
     *
     * First Beans should be registered and injected as needed. When IocContext starts, the Beans dependencies are resolved and their initialization is invoked.
     * Beans may also be registered after the context started. You should be aware, that the synchronous "GetImmediate" mehtods may do not return a bean during initialization.
     * @param argc cmd line args
     * @param argv
     * @return
     */
    int Start(int argc, char *argv[]);

    /**
     * @brief returns a bean or registers a default bean.
     *
     * This is menat to be used with Strings or other lightweight classes, where it is not a costly task to create a default bean.
     * TODO: Add a method that takes a function, which is invoked when the default bean is needed.
     * @param defaultValue fallback
     * @param name The bean's name
     * @param bExternal if the defaultBean is registered, it can join the IocContexts lifecycle (thus it is deleted by the framework when IocContext dies).
     * If bExternal is false, the client ist in charge of deleting defaultValue.
     */
    template < class T >
    T* GetImmediateOrRegisterDefault(T *defaultValue, const QString &name, const bool bExternal);
    /**
     * This Method takes a reference as it first argument. The reference is internally copied. The copy automatically joins the IocContexts lifecycle.
     * Is meant for easy use with literals.
     * TODO: Causes error with QString internal sharing. Maybe remove this method.
     */
    template < class T >
    T *GetImmediateOrRegisterDefault(T& defaultValue, const QString &name);
    /**
     * Asynchronous Getter for Beans by Name.
     */
    template < class T >
    void GetBeanByName(listener_t_templated loaded, const QString &name);
    /**
     * Asynchronous Getter for Beans by Type.
     */
    template < class T >
    void GetBeanByType(listener_list_t_templated loaded);
    /**
     * Synchronous Getter for Beans by Name. It's not safe that the dependencies have already been resolved.
     * The bean itself is maybe not already (ioc-)initialized and thus, it is not safe to call it's methods.
     * @return if a Beanwith this name is absolutely not known, null is returned.
     */
    template < class T >
    T *GetImmediate(const QString &name);

    /**
     * @brief returns all known beans of this type. Ths beans may have not been initialized. Can return an empty list.
     */
    template < class T >
    QList<T*> GetAllImmediate();
    /**
     * @brief Registers a Bean.
     *
     * A Bean can be any polymorphic Cpp class (+QString at the moment). If the class implements the IBean interface, it will receive special treatment.
     * If not, the object will be known under the given type and name. If any other Bean needs the object, it will be able to find it.
     * Configuration of a non-IBean-bean may be done by GetBeanBy* methods, where the beans to get are the dependent Beans.
     * TODO: add a method for non polymorphic types.
     * @param ptr bean
     * @param name name of the bean
     * @param bUseCopy should th
     * @param bDeleteOnRemove
     */
    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > RegisterBean(SrcT *ptr, const QString &name, bool bDeleteOnRemove, const bool bUseCopy);

    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > RegisterBean(SrcT &obj, const QString &name, bool bUseCopy);

    //const reference: updates when new objects are added. GetAll<IRenderable>() must only be called once.
    template < class T >
    void GetAll(listener_list_t_templated loaded);

    /**
     * @brief SetAsChildContext
     *
     * If a Bean is a IocContext, the parentContext will invoke this function.
     * The childcontext has to register all its beans as <parentContextName>.<BeanName>
     * The childContext will only store a map localName to globalName.
     * @param parent
     */
    void SetAsChildContext(IocContext *pParent, bool registerInterfacesOnParent);
    //deprecated
//    const QString& GetString(const QString &name) const;
//    const QString& GetString(const QString &name, const QString &valDefault);
//    void SetString(const QString &name, QString value);
//    const int& GetInt(const QString &name) const;
//    const int& GetInt(const QString &name, const int &valDefault);
//    void SetInt(const QString &name, int value);
//    const float& GetFloat(const QString &name) const;
//    const float& GetFloat(const QString &name, const float &valDefault);
//    void SetFloat(const QString &name, float value);
//    const QVector3D& GetVector3f(const QString &name) const;
//    const QVector3D& GetVector3f(const QString &name, const QVector3D &valDefault);
//    void SetVector3f(const QString &name, QVector3D value);
//    const QMatrix4x4& GetMatrix4f(const QString &name) const;
//    const QMatrix4x4& GetMatrix4f(const QString &name, const QMatrix4x4 &valDefault);
//    void SetMatrix4f(const QString &name, QMatrix4x4 value);
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

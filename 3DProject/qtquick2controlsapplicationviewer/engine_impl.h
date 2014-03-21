#ifndef ENGINE_IMPL_H
#define ENGINE_IMPL_H

//for code highlighting
#include <engine.h>
#include <boost/any.hpp>
#include <boost/type_traits/is_polymorphic.hpp>
#include "any_ptr.h"

//comma seperated list
#define AUTO_INTERFACES IEngineObject

#define rttilookup(T) (QString::fromUtf8(typeid(T).name()))
//T
//changed back to any ptr. change cast back. I think boost::any*() would not work
//Important: registerBean<My3DScene>.inject<ICamera>("advancedCameraManager")
//hierarchical IocContexts

typedef boost::signals2::signal<void(const QString &name, any_bean_ptr &obj)> listenerInternal_t;
typedef boost::signals2::signal<void(const QString &name, any_bean_ptr &obj, const CollectionChange change)> listenerInternal_list__t;

struct RootBeanInfo
{
    RootBeanInfo()
        :pLoaded(new listenerInternal_t), ptr()//(void *)0)
    {}
    RootBeanInfo(const RootBeanInfo& mod)
        : pLoaded(mod.pLoaded), ptr(mod.ptr)//, name(mod.name), type(mod.type)
    {}
    RootBeanInfo(QString name, QString type, listenerInternal_t &loaded, any_bean_ptr ptr)
        :name(name), type(type), pLoaded(&loaded), ptr(ptr)
    {}
    //QString name;
    //QString type;
    //For named beans
    boost::shared_ptr<listenerInternal_t> pLoaded;
    any_bean_ptr ptr;
};
struct ChildBeanInfo
{
    //boost::shared_ptr<listenerInternal_t> pLoaded;
    any_bean_ptr ptr;
};

typedef QHash<QString, ChildBeanInfo > QHashNameToChildBeanInfo;
typedef QHash<QString, QList<ChildBeanInfo> > QHashTypeToChildBeanInfo;
typedef QHash<QString, RootBeanInfo > QHashNameToRootBeanInfo;
typedef QHash<QString, QList<RootBeanInfo> > QHashTypeToRootBeanInfo;

class IocContextPrivate
{
    IocContextPrivate(bool registerInterfacesOnParent)
        :registerInterfacesOnParent(registerInterfacesOnParent)
    {}

    ~IocContextPrivate()
    {
        foreach(any_bean_ptr pDel, m_listInternalBeans)
        {
            //TODO delete pDel;
        }
    }
    template <typename T, typename SrcT>
    void RegisterByName(RootBeanInfo module, SrcT *ptr)
    {
        module.ptr = any_bean_ptr(dynamic_cast<T*>(ptr));
        m_ModulesByName.insert(module.name, module);
        m_ModulesByType[rttilookup(T)].append(module);
    }

    //variadic template recursion end
    template <typename SrcT>
    void RegisterByType(RootBeanInfo module, SrcT *ptr) {}

    //variadic template recursion
    template <typename T, typename... others, typename SrcT >
    void RegisterByType(RootBeanInfo module, SrcT *ptr)
    {
        module.ptr = any_bean_ptr(dynamic_cast<T*>(ptr));
        m_ModulesByType[rttilookup(T)].append(module);
        RegisterByType<others... >(module, ptr);
    }

    //Handle QString explicitly as non polymorphic type
    template <typename... some>
    void RegisterByTypeTry(RootBeanInfo module, QString *ptr)
    {}

    //variadic template recursion end
    template <typename SrcT>
    void _RegisterByTypeTry(RootBeanInfo module, SrcT *ptr) {}

    //variadic template recursion with explicit configuration of IBean
    template <IEngineObject, typename... others, typename SrcT >
    void _RegisterByTypeTry(RootBeanInfo module, SrcT *ptr)
    {
        IEngineObject *pObj = static_cast<IEngineObject>(ptr);
        pObj->__AddName(module.name);
        _RegisterByTypeTry<IEngineObject, others... , SrcT>(module, *ptr);
    }

    //variadic template recursion
    template <typename T, typename... others, typename SrcT >
    void _RegisterByTypeTry(RootBeanInfo module, SrcT *ptr)
    {
        //TODO: register non polymorphic
        module.ptr = any_bean_ptr(dynamic_cast<T*>(ptr));
        if(module.ptr)
        {
            //TODO: does this work on multi inheritance?
            void *voidPtr = module.ptr.toVoidPtr();
            bool alreadyContained = false;
            foreach(RootBeanInfo mod, m_ModulesByType[rttilookup(T)])
            {
                if(mod.ptr.toVoidPtr() == voidPtr)
                {
                    alreadyContained = true;
                    break;
                }
            }
            if(!alreadyContained)
            {
                m_ModulesByType[rttilookup(T)].append(module);
            }
        }
        _RegisterByTypeTry<others... >(module, ptr);
    }

    //variadic template recursion start
    template <typename... others, typename SrcT >
    void RegisterByTypeTry(RootBeanInfo module, SrcT *ptr)
    {
        // maybe some compilers can know this at compile time.
        // ATM e.g. GCC won't let us compile if we register
        // a non-polymorphic bean.
        if(boost::is_polymorphic<SrcT>())
        {
            _RegisterByTypeTry<others... >(module, ptr);
        }
    }

    unsigned int m_uniqueNameAppendix = 0;

//    QHash<QString, QString> m_Strings;
//    QHash<QString, int> m_Ints;
//    QHash<QString, float> m_Floats;
//    QHash<QString, QVector3D> m_Vectors;
//    QHash<QString, QMatrix4x4> m_Matrices;
    //QHashModules m_Classes;
    //Initialized once by root context. Holds all BeanInfos
    boost::shared_ptr< QHashNameToRootBeanInfo > m_pAllBeansByName;
    boost::shared_ptr< QHashTypeToRootBeanInfo > m_pAllBeansByType;
    QHashNameToChildBeanInfo m_ModulesByName;
    QHashTypeToChildBeanInfo m_ModulesByType;
    bool isChildCOntext;
   boost::shared_ptr< QHash<QString, boost::shared_ptr<listenerInternal_list__t> > > m_pAllBeanTypeCollectionListeners;

    QList<any_bean_ptr> m_listInternalBeansOfChildContext;
    friend class IocContext;
};
clean up from here
template < class T >
void IocContext::Get(listener_t_templated loaded, const QString &name)
{
    QHashNameToModule::Iterator iter(d->m_ModulesByName.find(name));

    if(iter == d->m_ModulesByName.end())
    if(iter == d->m_ModulesByName.end())
    {
        RootBeanInfo module;
        module.pLoaded->connect([loaded](const QString name, any_ptr ptr)
        {
            loaded(name, ptr.cast<T>());
        } );
        QString typeId = rttilookup(T);
        module.type = typeId;
        module.name = name;
        d->m_ModulesByName.insert(name, module);
        d->m_ModulesByType[typeId].append(module);
    }
    else
    {
        loaded(name, *cast_any_bean_ptr(T, iter->ptr));
    }
}

template < class T >
T* IocContext::GetImmediateOrRegisterDefault(T *defaultValue, const QString &name = STD_OBJ_NAME, const bool bExternal = false, const bool bUseCopy = false)
{
    //QHashNamedModules *pClasses = d->m_Classes[rttilookup(T)];
    //QHashNamedModules::Iterator iter(pClasses->find(name));
    if(name == STD_OBJ_NAME)
    {
        QString typeId = rttilookup(T);
        QList<RootBeanInfo> *pMods = &d->m_ModulesByType[typeId];
        if(pMods->size() == 0)
        {
            if(defaultValue != NULL)
            {
                RegisterBean<T>(defaultValue, name, bExternal, bUseCopy);
            }
            else
            {
                return defaultValue;
            }
        }
        else if(pMods->size() > 1)
        {
            throw QString("Bean not unique, there are more than one of type") + rttilookup(T);
        }
        return cast_any_bean_ptr(T, (*pMods)[0].ptr);
    }
    else
    {
        QHashNameToModule::Iterator iter(d->m_ModulesByName.find(name));
        if(iter == d->m_ModulesByName.end())
        {
            if(defaultValue != NULL)
            {
                RegisterBean<T>(defaultValue, name, bExternal, bUseCopy);
            }
            return defaultValue;
        }
        else
        {
            return cast_any_bean_ptr(T, iter->ptr);
        }
    }
}

template < class T >
T* IocContext::GetImmediateOrRegisterDefault(T &defaultValue, const QString &name = STD_OBJ_NAME)
{
    //copy defaultValue and use the copy in the system.
    return GetImmediateOrRegisterDefault(defaultValue, name, false);
}

template < class T >
T *IocContext::GetImmediate(const QString &name = STD_OBJ_NAME)
{
    return GetImmediateOrRegisterDefault<T>((T*)0, name);
}

template < class T >
QList<T*> IocContext::GetAllImmediate()
{
    QList<T*> ret;
    QString typeId = rttilookup(T);
    foreach(RootBeanInfo mod,d->m_ModulesByType[typeId] )
    {
        ret.append(cast_any_bean_ptr(T, mod.ptr));
    }
    return ret;
}

template < class T >
void IocContext::GetAll(listener_list_t_templated loaded)
{
    QString typeId = rttilookup(T);
    QHash<QString, boost::shared_ptr<listenerInternal_list__t> >::Iterator iter(d->m_ListListeners.find(typeId));
    if(iter == d->m_ListListeners.end())
    {
        d->m_ListListeners[typeId] = boost::shared_ptr<listenerInternal_list__t>(new listenerInternal_list__t);
        iter = d->m_ListListeners.find(typeId);
    }
    iter.value()->connect(
                [loaded](const QString name, any_bean_ptr &obj, const CollectionChange change)
                    {
                        loaded(name, cast_any_bean_ptr(T,obj), change);
                    });

    //QHashNamedModules* modulesOfType = &d->m_Classes[typeId];
    QList<RootBeanInfo> modulesOfType = d->m_ModulesByType[typeId];
    foreach(RootBeanInfo module, modulesOfType)
    {
        loaded(module.name, cast_any_bean_ptr(T, module.ptr), COLLECTION_ADDED);
    }
}

template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
ModuleLazyChain< SrcT > IocContext::RegisterBean(SrcT *ptrIn, const QString &name = STD_OBJ_NAME, bool bDeleteOnRemove = true, const bool bUseCopy = false)
{
    SrcT ptr;
    if(bUseCopy)
    {
        //copy the obj and care about lifcycle of obj
        ptr = new SrcT(*ptrIn);
        if(bDeleteOnRemove == false)
        {
            Q_ASSERT(0);//"Invalid arguments: When copying a bean into context lifecycle, it must also delete the bean.");
        }
    }
    else
    {
        // use the obj directly. Client guarantees that obj is not deleted for the time the context lives.
        ptr = ptrIn;
    }
    //QHashNamedModules* modulesOfType = &d->m_Classes[rttilookup(T)];
    //QHashNamedModules::Iterator iter = modulesOfType->find(name);
    QHashNameToModule::Iterator iter(d->m_ModulesByName.find(name));
    QString typeId = rttilookup(SrcT);
    RootBeanInfo *pMod;
    bool bDelete;
    if(iter == d->m_ModulesByName.end())
    {
        pMod = new RootBeanInfo();
        bDelete = true;
    }
    else
    {
        //Module exists. A listener may has been registered. The Bean must not exist altough.
        pMod = &iter.value();
        if(pMod->ptr)
        {
            QString errmsg = "NAMECLASH for name: ";
            errmsg += name;
            errmsg += "; type: ";
            errmsg += rttilookup(NamedInterface);
            errmsg += "(new type: ";
            errmsg += typeId;
            errmsg += ", old type: ";
            errmsg += iter->type;
            errmsg += ")";
            throw errmsg;
        }
        (*pMod->pLoaded)(name, pMod->ptr);
        bDelete = false;
    }
    if(name == STD_OBJ_NAME)
    {
        pMod->name = typeId + "_" + d->m_uniqueNameAppendix++;
    }
    else
    {
        pMod->name = name;
    }
    pMod->type = typeId;
    d->RegisterByName<NamedInterface>(*pMod, ptr);
    d->RegisterByType<AdditionalAnonymousInterfaces...>(*pMod, ptr);
    d->RegisterByTypeTry<AUTO_INTERFACES>(*pMod, ptr);
    if(bDeleteOnRemove)
    {
        d->m_listInternalBeans.append(any_bean_ptr(ptr));
    }
    if(bDelete)
    {
        delete pMod;
    }
    return ModuleLazyChain<SrcT>(ptr, this);
}

template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
ModuleLazyChain< SrcT > IocContext::RegisterBean(IocContext::SrcT obj, const QString &name = STD_OBJ_NAME, bool bUseCopy = true)
{
    return RegisterBean<NamedInterface, AdditionalAnonymousInterfaces...>(&obj, name, bUseCopy, bUseCopy);
}

template <class T>
ModuleLazyChain<T>::ModuleLazyChain(T *ref, IIocContext *engine)
{
    m_pRef = ref;
    m_pEngine = engine;
}

template <class T>
ModuleLazyChain<T> ModuleLazyChain<T>::alias(const QString& name)
{
    m_pEngine->Set<T>(m_pRef, name);
    return *this;
}

//introduces error where bean is twice in m_ModulesByType[IEngineObjkect]
//disbaled for now, use variadic template instead
//template <class T>
//template <class NewT>
//ModuleLazyChain< NewT > ModuleLazyChain<T>::alias(const QString& name)
//{
//    NewT *pNew = dynamic_cast<NewT*>(m_pRef);
//    Q_ASSERT(pNew);
//    return m_pEngine->Set<NewT>(pNew, name);
//}

#endif // ENGINE_IMPL_H

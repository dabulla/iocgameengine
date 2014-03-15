#ifndef ENGINE_IMPL_H
#define ENGINE_IMPL_H

//for code highlighting
#include <engine.h>

#define rttilookup(T) (QString::fromUtf8(typeid(T).name()))

class any_ptr {

    void* ptr_;
    void (*thr_)(void*);
    void (*del_)(void*);

    template <typename T>
    static void thrower(void* ptr) { throw static_cast<T*>(ptr); }
    template <typename T>
    static void deleter(void* ptr)
    {
        T* typed = static_cast<T*>(ptr);
        delete typed;
    }

public:
    templ but its not a templ
    template <typename T>
    any_ptr(T* ptr) : ptr_(ptr), thr_(&thrower<T>), del_(&deleter<T>) {}
    any_ptr() : ptr_(0), thr_(0), del_(0) {}

    template <typename U>
    U* cast() const {
        Hell I hope it can really cast...
        //todo: disable in final code. Detect wrong casts only while debugging because try/catch costs performance
        try { thr_(ptr_); }
        catch (U* ptr) { return static_cast<U*>(ptr); }
        catch (...) {}
        return 0;
    }
    void del() { del_(ptr_); }
};

typedef boost::signals2::signal<void(const QString &name, any_ptr &obj)> listenerInternal_t;

struct Module
{
    Module()
        :pLoaded(new listenerInternal_t), ptr((void *)0)
    {}
    Module(const Module& mod)
        :name(mod.name), type(mod.type), pLoaded(mod.pLoaded), ptr(mod.ptr)
    {}
    Module(QString name, QString type, listenerInternal_t &loaded, any_ptr ptr)
        :name(name), type(type), pLoaded(&loaded), ptr(ptr)
    {}
    QString name;
    QString type;
    boost::shared_ptr<listenerInternal_t> pLoaded;
    any_ptr ptr;
};

//typedef QHash<QString, Module > QHashNamedModules;
//typedef QHash<QString, QHashNamedModules > QHashModules;
typedef QHash<QString, Module > QHashNameToModule;
typedef QHash<QString, QList<Module> > QHashTypeToModules;

class EnginePrivate
{
    ~EnginePrivate()
    {
        foreach(any_ptr pDel, m_listInternalBeans)
        {
            pDel.del();
        }
    }

    QHash<QString, QString> m_Strings;
    QHash<QString, int> m_Ints;
    QHash<QString, float> m_Floats;
    QHash<QString, QVector3D> m_Vectors;
    QHash<QString, QMatrix4x4> m_Matrices;
    //QHashModules m_Classes;
    QHashNameToModule m_ModulesByName;
    QHashTypeToModules m_ModulesByType;
    QHash<QString, listenerInternal_t > m_ListListeners;

    QList<any_ptr> m_listInternalBeans;
    friend class Engine;
};


template < class T >
void Engine::Get(listener_t_templated loaded, const QString &name = STD_OBJ_NAME)
{
    //QHashNamedModules* modulesOfType = &d->m_Classes[rttilookup(T)];
    //QHashNamedModules::Iterator iter(modulesOfType->find(name));
    QHashNameToModule::Iterator iter(d->m_ModulesByName.find(name));

    if(iter == d->m_ModulesByName.end())
    {
        Module module;
        module.pLoaded->connect([loaded](const QString name, any_ptr ptr) -> void
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
        loaded(name, *(iter->ptr.cast<T>()));
    }
}

template < class T >
T* Engine::GetImmediate(T *defaultValue, const QString &name = STD_OBJ_NAME, const bool bExternal = false)
{
    //QHashNamedModules *pClasses = d->m_Classes[rttilookup(T)];
    //QHashNamedModules::Iterator iter(pClasses->find(name));
    QHashNameToModule::Iterator iter(d->m_ModulesByName.find(name));
    if(iter == d->m_ModulesByName.end())
    {
        if(defaultValue != NULL)
        {
            Set<T>(defaultValue, name, bExternal);
        }
        return defaultValue;
    }
    else
    {
        return iter->ptr.cast<T>();
    }
}

template < class T >
T* Engine::GetImmediate(T &defaultValue, const QString &name = STD_OBJ_NAME)
{
    //copy defaultValue and use the copy in the system.
    return GetImmediate(&defaultValue, name, false);
}

template < class T >
T *Engine::GetImmediate(const QString &name = STD_OBJ_NAME)
{
    return GetImmediate<T>((T*)0, name);
}

template < class T >
void Engine::GetAll(listener_list_t_templated loaded)
{
    QString typeId = rttilookup(T);
    d->m_ListListeners[typeId].connect(loaded);

    //QHashNamedModules* modulesOfType = &d->m_Classes[typeId];
    QList<Module> modulesOfType = d->m_ModulesByType[typeId];
    foreach(Module module, modulesOfType)
    {
        loaded(module.name, module.ptr.cast<T>());
    }
}

template < class T >
ModuleLazyChain< T > Engine::Set(T *ptr, const QString &name = STD_OBJ_NAME, bool bDeleteOnRemove = true)
{
    //QHashNamedModules* modulesOfType = &d->m_Classes[rttilookup(T)];
    //QHashNamedModules::Iterator iter = modulesOfType->find(name);
    QHashNameToModule::Iterator iter(d->m_ModulesByName.find(name));
    QString typeId = rttilookup(T);
    if(iter == d->m_ModulesByName.end())
    {
        Module module;
        module.ptr = any_ptr(ptr);
        module.name = name;
        module.type = typeId;
        d->m_ModulesByName.insert(name, module);
        d->m_ModulesByType[typeId].append(module);
    }
    else
    {
        iter->ptr = any_ptr(ptr);
        (*iter->pLoaded)(name, iter->ptr);
        iter->name = name;
    }
    if(bDeleteOnRemove)
    {
        d->m_listInternalBeans.append(ptr);
    }
    return ModuleLazyChain<T>(ptr, this);
}

template < class T >
ModuleLazyChain< T > Engine::Set(T &obj, const QString &name = STD_OBJ_NAME, bool bUseCopy = true)
{
    T *pObj;
    if(bUseCopy)
    {
        //copy the obj and care about lifcycle of obj
        pObj = new T(obj);
    }
    else
    {
        // use the obj directly. Client guarantees that obj is not deleted.
        pObj = &obj;
    }
    return Set<T>(&obj, name, bUseCopy);
}

template <class T>
ModuleLazyChain<T>::ModuleLazyChain(T *ref, Engine *engine)
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

template <class T>
template <class NewT>
ModuleLazyChain< NewT > ModuleLazyChain<T>::alias(const QString& name)
{
    NewT *pNew = dynamic_cast<NewT*>(m_pRef);
    Q_ASSERT(pNew);
    return m_pEngine->Set<NewT>(pNew, name);
}

#endif // ENGINE_IMPL_H

#include "engine.h"
#include "qtquickinputeventswindow.h"
#include <QHash>
#include <typeinfo>
#include "autowiredptr.h"

class IEngineObjectPrivate
{
private:
    IocContext *m_pEngine;
    QList<__AutowiredPtrContainer> m_engineReceivers;
    void SetEngine(IocContext *pe)
    {
        m_pEngine = pe;
        foreach(__AutowiredPtrContainer ptr, m_engineReceivers)
        {
            ptr(m_pEngine);
        }
    }
    friend class IEngineObject;
    friend class IocContext;
};

IEngineObject::IEngineObject()
    :d(new IEngineObjectPrivate)
{
}

IEngineObject::~IEngineObject()
{
    delete d;
}

void IEngineObject::__AutowiredPtr_add(__AutowiredPtrContainer fn)
{
    d->m_engineReceivers.append(fn);
}

IocContext &IEngineObject::GetIocContext() const
{
    return *d->m_pEngine;
}

IocContext::IocContext()
    : d(new IocContextPrivate())
{
}

IocContext::~IocContext()
{
    delete d;
}

int IocContext::Start(int argc, char *argv[])
{
    QList<IEngineObject*> engObjs;
//    QHashNamedModules map;
//    foreach( map, d->m_Classes )
//    {
        foreach(Module mod, d->m_ModulesByName )
        {
            IEngineObject *engObj = cast_any_bean_ptr(IEngineObject, mod.ptr);
            if(engObj == NULL) continue;
            engObj->d->SetEngine(this);
            engObj->PublishServices();
            engObjs.append(engObj);
        }
//    }
    // All Objects had chance to publish dependencies of other objs.
    foreach(IEngineObject* ptr, engObjs )
    {
        ptr->Activated();
    }
    IStarter *pStarter = GetImmediate<IStarter>();
    int rc;
    if(pStarter)
    {
        rc = pStarter->Start(argc, argv);
    }
    else
    {
        rc = 0;
    }
    return rc;
}

const QString& IocContext::GetString(const QString &name) const
{
    QHash<QString, QString>::Iterator i = d->m_Strings.find(name);
    Q_ASSERT(i != d->m_Strings.end());
    return i.value();
}

const QString& IocContext::GetString(const QString &name, const QString& valDefault)
{
    QHash<QString, QString>::Iterator i = d->m_Strings.find(name);
    if(i != d->m_Strings.end())
    {
        return i.value();
    }
    else
    {
        d->m_Strings[name] = valDefault;
        return valDefault;
    }
}

void IocContext::SetString(const QString &name, QString value)
{
    d->m_Strings[name] = value;
}

const int& IocContext::GetInt(const QString &name) const
{
    QHash<QString, int>::Iterator i = d->m_Ints.find(name);
    Q_ASSERT(i != d->m_Ints.end());
    return i.value();
}

const int& IocContext::GetInt(const QString &name, const int& valDefault)
{
    QHash<QString, int>::Iterator i = d->m_Ints.find(name);
    if( i != d->m_Ints.end() )
    {
        return i.value();
    }
    else
    {
        d->m_Ints[name] = valDefault;
        return valDefault;
    }
}

void IocContext::SetInt(const QString &name, int value)
{
    d->m_Ints[name] = value;
}

const float& IocContext::GetFloat(const QString &name) const
{
    QHash<QString, float>::Iterator i = d->m_Floats.find(name);
    Q_ASSERT(i != d->m_Floats.end());
    return i.value();
}

const float& IocContext::GetFloat(const QString &name, const float& valDefault)
{
    QHash<QString, float>::Iterator i = d->m_Floats.find(name);
    if( i != d->m_Floats.end() )
    {
        return i.value();
    }
    else
    {
        d->m_Floats[name] = valDefault;
        return valDefault;
    }
}

void IocContext::SetFloat(const QString &name, float value)
{
    d->m_Floats[name] = value;
}

const QVector3D& IocContext::GetVector3f(const QString &name) const
{
    QHash<QString, QVector3D>::Iterator i = d->m_Vectors.find(name);
    Q_ASSERT(i != d->m_Vectors.end());
    return i.value();
}

const QVector3D& IocContext::GetVector3f(const QString &name, const QVector3D& valDefault)
{
    QHash<QString, QVector3D>::Iterator i = d->m_Vectors.find(name);
    if( i != d->m_Vectors.end() )
    {
        return i.value();
    }
    else
    {
        d->m_Vectors[name] = valDefault;
        return valDefault;
    }
}

void IocContext::SetVector3f(const QString &name, QVector3D value)
{
    d->m_Vectors[name] = value;
}

const QMatrix4x4& IocContext::GetMatrix4f(const QString &name) const
{
    QHash<QString, QMatrix4x4>::Iterator i = d->m_Matrices.find(name);
    Q_ASSERT(i != d->m_Matrices.end());
    return i.value();
}

const QMatrix4x4& IocContext::GetMatrix4f(const QString &name, const QMatrix4x4 &valDefault)
{
    QHash<QString, QMatrix4x4>::Iterator i = d->m_Matrices.find(name);
    if( i != d->m_Matrices.end() )
    {
        return i.value();
    }
    else
    {
        d->m_Matrices[name] = valDefault;
        return valDefault;
    }
}

void IocContext::SetMatrix4f(const QString &name, QMatrix4x4 value)
{
    d->m_Matrices[name] = value;
}

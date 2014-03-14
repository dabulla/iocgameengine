#include "engine.h"
#include "qtquickinputeventswindow.h"
#include <QHash>
#include <typeinfo>


Engine::Engine()
    : d(new EnginePrivate())
{
}

Engine::~Engine()
{
    delete d;
}

int Engine::Start(int argc, char *argv[])
{
    QList<IEngineObject*> engObjs;
    QHashNamedModules map;
    foreach( map, d->m_Classes )
    {
        foreach(Module mod, map )
        {
            IEngineObject *engObj = mod.ptr.cast<IEngineObject>();
            if(engObj == NULL) continue;
            engObj->SetEngine(this);
            engObj->PublishServices();
            engObjs.append(engObj);
        }
    }
    // All Objects had chance to publish dependencies of other objs.
    foreach(IEngineObject* ptr, engObjs )
    {
        ptr->Activated();
    }
    return GetImmediate<IStarter>().Start(argc, argv);
}

const QString& Engine::GetString(const QString &name) const
{
    QHash<QString, QString>::Iterator i = d->m_Strings.find(name);
    Q_ASSERT(i != d->m_Strings.end());
    return i.value();
}

const QString& Engine::GetString(const QString &name, const QString& valDefault)
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

void Engine::SetString(const QString &name, QString value)
{
    d->m_Strings[name] = value;
}

const int& Engine::GetInt(const QString &name) const
{
    QHash<QString, int>::Iterator i = d->m_Ints.find(name);
    Q_ASSERT(i != d->m_Ints.end());
    return i.value();
}

const int& Engine::GetInt(const QString &name, const int& valDefault)
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

void Engine::SetInt(const QString &name, int value)
{
    d->m_Ints[name] = value;
}

const float& Engine::GetFloat(const QString &name) const
{
    QHash<QString, float>::Iterator i = d->m_Floats.find(name);
    Q_ASSERT(i != d->m_Floats.end());
    return i.value();
}

const float& Engine::GetFloat(const QString &name, const float& valDefault)
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

void Engine::SetFloat(const QString &name, float value)
{
    d->m_Floats[name] = value;
}

const QVector3D& Engine::GetVector3f(const QString &name) const
{
    QHash<QString, QVector3D>::Iterator i = d->m_Vectors.find(name);
    Q_ASSERT(i != d->m_Vectors.end());
    return i.value();
}

const QVector3D& Engine::GetVector3f(const QString &name, const QVector3D& valDefault)
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

void Engine::SetVector3f(const QString &name, QVector3D value)
{
    d->m_Vectors[name] = value;
}

const QMatrix4x4& Engine::GetMatrix4f(const QString &name) const
{
    QHash<QString, QMatrix4x4>::Iterator i = d->m_Matrices.find(name);
    Q_ASSERT(i != d->m_Matrices.end());
    return i.value();
}

const QMatrix4x4& Engine::GetMatrix4f(const QString &name, const QMatrix4x4 &valDefault)
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

void Engine::SetMatrix4f(const QString &name, QMatrix4x4 value)
{
    d->m_Matrices[name] = value;
}

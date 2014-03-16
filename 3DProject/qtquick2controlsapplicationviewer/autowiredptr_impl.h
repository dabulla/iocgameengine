#ifndef AUTOWIREDPTR_IMPL_H
#define AUTOWIREDPTR_IMPL_H

#include "autowiredptr.h"

template <class T>
class __AutowiredPtrPrivate
{
    const IocContext *m_pEngine;

    QList< std::pair<QString, T*> > m_allNamed;
    QList< T* > m_all;
    friend class AutowiredPtr<T>;
};

template <class T>
AutowiredPtr<T>::AutowiredPtr(IEngineObject *engobj)
    :d(new __AutowiredPtrPrivate<T>)
{
    engobj->__AutowiredPtr_add(
        [this](IocContext *pEng)
        {
            this->d->m_pEngine = pEng;
            pEng->GetAll<T>(
                [this](const QString name, T *pObj, const CollectionChange change)
                {
                    d->m_all.append(pObj);
                });
        });
}

template <class T>
AutowiredPtr<T>::~AutowiredPtr()
{
    delete d;
}

template <class T>
T *AutowiredPtr<T>::GetImmediate(QString name) const
{
    return d->m_pEngine->template GetImmediate<T>(name);
}

template <class T>
const QList<T*> & AutowiredPtr<T>::GetAllImmediate() const
{
    return d->m_all;
    //return d->m_pEngine->template GetAllImmediate<T>();
}

template <class T>
void AutowiredPtr<T>::Get(QString name, listener_t_templated loaded) const
{
    d->m_pEngine->template Get<T>(name, loaded);
}

template <class T>
void AutowiredPtr<T>::GetAll(listener_list_t_templated loaded) const
{
    d->m_pEngine->template GetAll<T>(loaded);
}

template <class T>
AutowiredPtr<T>::operator QList<T*>() const
{
    return d->m_all;
}

template <class T>
AutowiredPtr<T>::operator T *() const
{
    return this->template GetImmediate<T>();
}




#endif // AUTOWIREDPTR_IMPL_H

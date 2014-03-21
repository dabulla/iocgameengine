#ifndef AUTOWIREDPTR_IMPL_H
#define AUTOWIREDPTR_IMPL_H

#include "autowiredptr.h"

template <class T, const char *pszInternalName>
class __AutowiredPtrPrivate
{
    const IIocContext *m_pEngine;

    QList< std::pair<QString, T*> > m_allNamed;
    QList< T* > m_all;
    friend class AutowiredPtr<T>;
};

class AutowiredHelper
{
    static AutowiredPtr_add(IEngineObject *engobj, __AutowiredPtrContainer fn)
    {
        engobj->__AutowiredPtr_add(fn);
    }
};

//TODO: Implement syntax: "MainWndBean.ICamera.ViewMatrix" (Where "ViewMatrix" ist the internal name)
// LazyChain.inject("MainWnd.ICamera[2]", typeOrIntenalName);
// If this is evaluated before the bean exists... care also about lists...
// remove events. Use Interfaces + Lists.

template <class T, const char *pszInternalName >
AutowiredPtr<T, pszInternalName>::AutowiredPtr(IEngineObject *engobj)
    :d(new __AutowiredPtrPrivate<T, pszInternalName>)
{
    AutowiredHelper::__AutowiredPtr_add(
        [this](IIocContext *pEng)
        {
            this->d->m_pEngine = pEng;
            pEng->GetAll<T>(
                [this](const QString name, T *pObj, const CollectionChange change)
                {
                    d->m_all.append(pObj);
                });
        });
}

template <class T, const char *pszInternalName>
AutowiredPtr<T, pszInternalName>::~AutowiredPtr()
{
    delete d;
}

template <class T, const char *pszInternalName>
T *AutowiredPtr<T, pszInternalName>::GetImmediate(QString name) const
{
    return d->m_pEngine->template GetImmediate<T>(name);
}

template <class T, const char *pszInternalName>
const QList<T*> & AutowiredPtr<T, pszInternalName>::GetAllImmediate() const
{
    return d->m_all;
    //return d->m_pEngine->template GetAllImmediate<T>();
}

template <class T, const char *pszInternalName>
void AutowiredPtr<T, pszInternalName>::Get(QString name, listener_t_templated loaded) const
{
    d->m_pEngine->template Get<T>(name, loaded);
}

template <class T, const char *pszInternalName>
void AutowiredPtr<T, pszInternalName>::GetAll(listener_list_t_templated loaded) const
{
    d->m_pEngine->template GetAll<T>(loaded);
}

template <class T, const char *pszInternalName>
AutowiredPtr<T, pszInternalName>::operator QList<T*>() const
{
    return d->m_all;
}

template <class T, const char *pszInternalName>
AutowiredPtr<T, pszInternalName>::operator T *() const
{
    return this->template GetImmediate<T>();
}




#endif // AUTOWIREDPTR_IMPL_H

#include "autowiredptr.h"

template <class T, const char* szName>
class AutowiredPtrPrivate
{
    const IocContext *m_pEngine;

    const QList< std::pair<QString, T> > m_allNamed;
    const QList< T > m_all;
};

template <class T, const char* szName>
AutowiredPtr<T, szName>::AutowiredPtr(const IEngineObject *engobj)
    :d(new AutowiredPtrPrivate<T, szName>)
{
    engobj->__AutowiredPtr_add(
        [this](IocContext *pEng)
        {
            this->d->m_pEngine = pEng;
        });
}

template <class T, const char* szName>
AutowiredPtr<T, szName>::~AutowiredPtr()
{
    delete d;
}

template <class T, const char* szName>
T *AutowiredPtr<T, szName>::GetImmediate() const
{
    return d->m_pEngine->template GetImmediate<T>(szName);
}

template <class T, const char* szName>
void AutowiredPtr<T, szName>::Get(listener_t_templated loaded) const
{
    d->m_pEngine->template Get<T>(loaded);
}

template <class T, const char* szName>
void AutowiredPtr<T, szName>::GetAll(listener_list_t_templated loaded) const
{
    d->m_pEngine->template GetAll<T>(loaded);
}

template <class T, const char* szName>
AutowiredPtr<T, szName>::operator QList<T*>() const
{

}

template <class T, const char* szName>
AutowiredPtr<T, szName>::operator T *() const
{
    return this->template GetImmediate<T, szName>();
}

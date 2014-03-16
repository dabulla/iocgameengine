#ifndef AUTOWIREDPTR_H
#define AUTOWIREDPTR_H

#include "engine.h"
#include <QList>
#include <utility>
#include <boost/signals2.hpp>

template <class T>
class __AutowiredPtrPrivate;

template <class T>
class AutowiredPtr
{
public:
    AutowiredPtr(IEngineObject *engobj);
    virtual ~AutowiredPtr();

    void Get(QString name, listener_t_templated loaded) const;
    T *GetImmediate(QString name) const;
    void GetAll(listener_list_t_templated loaded) const;
    const QList<T*> & GetAllImmediate() const;

    operator T*() const;
    operator QList< T* >() const;

private:
    void init(const IocContext *pEng);
    __AutowiredPtrPrivate<T> *d;
    friend class IEngineObjectPrivate;
};

#include "autowiredptr_impl.h"

#endif // AUTOWIREDPTR_H

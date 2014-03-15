#ifndef AUTOWIREDPTR_H
#define AUTOWIREDPTR_H

#include "engine.h"
#include <QList>
#include <utility>
#include <boost/signals2.hpp>

template <class T, const char* szName>
class AutowiredPtrPrivate;

template <class T, const char* szName = STD_OBJ_NAME>
class AutowiredPtr
{
public:
    AutowiredPtr(const IEngineObject *engobj);
    virtual ~AutowiredPtr();

    T *GetImmediate() const;
    void Get(listener_t_templated loaded) const;
    void GetAll(listener_list_t_templated loaded) const;

    operator T*() const;
    operator QList< T* >() const;

private:
    void init(const Engine *pEng);
    AutowiredPtrPrivate<T, szName> *d;
    friend class IEngineObjectPrivate;
};

#endif // AUTOWIREDPTR_H

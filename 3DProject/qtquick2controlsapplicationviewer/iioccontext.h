#ifndef IIOCCONTEXT_H
#define IIOCCONTEXT_H
/*
IDEA: make (New)IocContext being only a delegate with a pointer to (Old)IocContext_Base.
(New)IocCOntext then changes name during registration to handle internal names.

#include <QList>

class IIocContext : public BaseIocContextAdapter
{
    IocC
public:
    IIocContext();

    template < class T >
    T* GetImmediateOrRegisterDefault(T *defaultValue, const QString &name = STD_OBJ_NAME, const bool bExternal = false)
    {
        return BaseIocContextAdapter.GetImmediateOrRegisterDefault(defaultValue, name, bExternal);
    }
    template < class T >
    T *GetImmediateOrRegisterDefault(T& defaultValue, const QString &name = STD_OBJ_NAME);
    template < class T >
    T *GetImmediate(const QString &name = STD_OBJ_NAME);
    template < class T >
    void Get(listener_t_templated loaded, const QString &name = STD_OBJ_NAME) = 0;
    template < class T >
    QList<T*> GetAllImmediate() = 0;
    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > Register(SrcT *ptr, const QString &name = STD_OBJ_NAME, bool bDeleteOnRemove = true) = 0;
    template < class NamedInterface, class... AdditionalAnonymousInterfaces, class SrcT >
    ModuleLazyChain< SrcT > Register(SrcT &obj, const QString &name = STD_OBJ_NAME, bool bUseCopy = true) = 0;
};
*/
#endif // IIOCCONTEXT_H

#ifndef ANY_PTR_H
#define ANY_PTR_H

// no real use beacuse interfaces are not hierarchical
class any_ptr {

    void* ptr_;
    void (*thr_)(void*);
    //void (*del_)(void*);
    QString realType;

    template <typename T>
    static void thrower(void* ptr) { throw static_cast<T*>(ptr); }
//    template <typename T>
//    static void deleter(void* ptr)
//    {
//        T* typed = static_cast<T*>(ptr);
//        delete typed;
//    }

public:
    template <typename T>
    any_ptr(T* ptr) : ptr_(ptr), thr_(&thrower<T>), realType(rttilookup(T))/*, del_(&deleter<T>)*/ {}
    any_ptr() : ptr_(0), thr_(0)/*, del_(0)*/ {}

    template <typename U>
    U* cast() const {
        //U* ret = dynamic_cast<U*>(ptr);
        //if(ret) return ret;
        try { thr_(ptr_); }
        catch (U* ptr) { return static_cast<U*>(ptr); }
        catch (...) {}
        return 0;
    }
    operator bool() const
    {
        return ptr_;
    }
    void* toVoidPtr() const { return ptr_; }

    //void del() { del_(ptr_); }
};
typedef any_ptr any_bean_ptr;
#define cast_any_bean_ptr(T, ptr) (ptr.cast<T>())


#endif // ANY_PTR_H

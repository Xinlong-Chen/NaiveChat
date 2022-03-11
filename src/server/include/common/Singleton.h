#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton {
public:
    Singleton& operator = (const Singleton&) = delete;
    Singleton& operator = (Singleton&&)      = delete;

    static T& get_instance() {
        if(!instance)
            instance = new T_Instance;
        return *instance;
    }

protected:
    Singleton() {}

private:
    struct T_Instance : public T {
        T_Instance() : T() {}
    };

    static inline T* instance = nullptr;
};

#endif
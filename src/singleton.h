// singleton.h
#pragma once


template <typename T> 
class Singlton {
public:

    Singleton(const Singleton& other) = delete;
    Singleton(Singleton&& other) = delete;
    Singleton& operator=(Singleton&& other) = delete;
    Singleton& operator=(Singleton& other) = delete;

    static T& getInstace(){
        static T instance;
        return instance;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;
};
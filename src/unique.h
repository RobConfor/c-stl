#pragma once

#include <utility> 


template <typename T>
class unique_ptr { 
public:
    unique_ptr(){
        _ptr = nullptr;
    }

    explicit unique_ptr(T* ptr){
        _ptr = ptr;
    }

    // enforce move only 
    unique_ptr(const unique_ptr& uni) = delete;
    unique_ptr& operator=(unique_ptr uni) = delete;
    unique_ptr& operator=(const unique_ptr& uni) = delete;

    unique_ptr& operator=(unique_ptr&& other){
        if(this == &other){
            return *this;
        }
        delete _ptr;
        _ptr = other._ptr;
        other._ptr = nullptr;
        return *this;
    }

    // this can never be true, or else we have invalidated our unqiueness
    bool operator==(const unique_ptr& other) const {
        return _ptr == other._ptr;
    }

    bool operator==(const T* other) const {
        return *_ptr == *other;
    }

    unique_ptr(unique_ptr&& other) noexcept {
        _ptr = other._ptr;
        other._ptr = nullptr;
    }
    
    T* get() const {
        return _ptr;
    }

    T& operator*() const {
        return *_ptr;
    }

    T* operator->() const {
        return _ptr;
    }

    T* release() {
        T* temp = _ptr;
        _ptr = nullptr;
        return temp;
    }

    void reset(){
        delete _ptr;
        _ptr = nullptr;
    }

    ~unique_ptr(){
        delete _ptr;
    }


private:
T* _ptr;

};


template <typename T>
unique_ptr<T> make_unique(T val){
    return unique_ptr<T>();
}

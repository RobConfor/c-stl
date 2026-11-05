#pragma once

#include "vector.h"
#include <utility>

template <typename T>
class stack {

    stack();

    explicit stack(size_t size){
        data = vector<T>(size);
        _size = size;
    }

    T top() const {
        return data.back();
    }

    void pop(){
        data.pop_back();
    }

    ~stack(){
        ~data();
    }

private:
// using vector for better CPU and cache locality
vector<T> data;
size_t _size;

};
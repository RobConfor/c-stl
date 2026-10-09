#pragma once

#include "vector.h"
#include <utility>

template <typename T>
class stack {

    stack();

    explicit stack(size_t size);

    // custom underlying data structure
    stack(size_t size);


    T top() const {
        return data.back();
    }

    void pop(){
        data.pop_back();
    }


private:
size_t capacity;
vector<T> data;

};
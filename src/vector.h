#pragma once

#include <memory>
#include <utility>
#include <stdexcept>

template <typename T>

class vector {
private:
size_t _size;
size_t _capacity;
T* _data;

void __destroy() noexcept {
    for(size_t i = 0; i < _size; ++i){
        _data[i].~T();
    }
    _size = 0;
}

void __deallocate() noexcept {
    if (_data) {
        delete[] _data;
        _data = nullptr;
        _capacity = 0;
    }
}

void __reallocate(size_t n) noexcept {
    T* temp = new T[n];

    for(size_t i = 0; i < _size; ++i){
        temp[i] = std::move(_data[i]); 
    }
    
    delete[] _data;

    _data = temp;
    _capacity = n;
}


public: 
//////////////// CONSTRUCTORS //////////////////
//////////////// CONSTRUCTORS //////////////////
//////////////// CONSTRUCTORS //////////////////

    vector() {
        _size = 0;
        _capacity = 2;
        _data = new T[_capacity];
    }

    explicit vector(size_t size){
        _data = new T[size];
        _size = 0;
        _capacity = size;
    }

    vector(size_t size, T* vals){

        _data = new T[size];

        // or we can just have data point to vals? not sure which is correct

        for(size_t i = 0; i < size; ++i){
            *(_data + i) = *(vals + i);
        }
        _size = size;
        _capacity = size;
    }

    // vector(std::initializer_list<T> list){
    //     _allocator = Allocator();
    //     _data = std::allocator_traits<Allocator>::allocate(_allocator, list.size());

    //     // or we can just have data point to vals? not sure which is correct
    //     std::copy(list.begin(), list.end(), _data);
    //     _size = list.size();
    //     _capacity = list.size();
    // }


    vector(size_t size, const T& val){
        _data = new T[size];
        _capacity = size;

        for(size_t i = 0; i < size; ++i){
            *(_data + i) = val;
        }
        _size = size;

    }

    ///////////////////////////////////////////////////////
    ///////////////////// ASSIGMENT ///////////////////////
    ///////////////////////////////////////////////////////


    // INDEXING AND ACCESS // 
    T& operator[](const size_t idx) noexcept {
        return *(_data + idx);
    }

    const T& operator[](const size_t idx) const noexcept{
        return *(_data + idx);
    }

    T& at(size_t index) {
        if (index >= _size) {
            throw std::out_of_range("vector::at index out of range");
        }
        return _data[index];
    }

    const T& at(size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("vector::at index out of range");
        }
        return _data[index];
    }


    // ALLOCATION ///
    void resize(size_t size){
        _data = new T[size];
    }

    void resize(size_t size, const T val){
        _data = new T[size];

        for(size_t i = 0; i < size; ++i){
            *(_data + i) = val;
        }

    }

    void reserve(size_t size) noexcept {
        _data = new T[size];
    }


    void push_back(T val){
        if(_size == _capacity){
            __reallocate(_capacity * 2);
        }
        *(_data + _size) = val;
        ++_size;
    }

    T back() const {
        return _data[_size - 1];
    }

    void pop_back() {
        if(_size == 0){
            // error; 
            return;
        }
        _size--;
    }
    
    size_t size(){
        return _size;
    }

    // destory
    ~vector(){
        // one of these?
        // __destroy();
        delete[] _data;
    }

};
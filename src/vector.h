#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

template <typename T>
class vector {
private:
    using allocator_type = std::allocator<T>;
    using allocator_traits = std::allocator_traits<allocator_type>;

    allocator_type _allocator;
    std::size_t _size = 0;
    std::size_t _capacity = 0;
    T *_data = nullptr;

    void destroy_elements() noexcept {
        while (_size != 0) {
            --_size;
            allocator_traits::destroy(_allocator, _data + _size);
        }
    }

    void reallocate(std::size_t new_capacity) {
        T *new_data = _allocator.allocate(new_capacity);
        std::size_t constructed = 0;
        try {
            for (; constructed < _size; ++constructed) {
                allocator_traits::construct(
                    _allocator,
                    new_data + constructed,
                    std::move_if_noexcept(_data[constructed]));
            }
        } catch (...) {
            while (constructed != 0) {
                --constructed;
                allocator_traits::destroy(_allocator, new_data + constructed);
            }
            _allocator.deallocate(new_data, new_capacity);
            throw;
        }

        destroy_elements();
        _allocator.deallocate(_data, _capacity);
        _data = new_data;
        _size = constructed;
        _capacity = new_capacity;
    }

public:
    vector() : _capacity(2), _data(_allocator.allocate(_capacity)) {}

    explicit vector(std::size_t size)
        : _size(size), _capacity(size), _data(size == 0 ? nullptr : _allocator.allocate(size)) {
        std::size_t constructed = 0;
        try {
            for (; constructed < _size; ++constructed) {
                allocator_traits::construct(_allocator, _data + constructed);
            }
        } catch (...) {
            while (constructed != 0) {
                --constructed;
                allocator_traits::destroy(_allocator, _data + constructed);
            }
            if (_data != nullptr) {
                _allocator.deallocate(_data, _capacity);
            }
            throw;
        }
    }

    vector(std::size_t size, T *values)
        : _size(size), _capacity(size), _data(size == 0 ? nullptr : _allocator.allocate(size)) {
        std::size_t constructed = 0;
        try {
            for (; constructed < _size; ++constructed) {
                allocator_traits::construct(_allocator, _data + constructed, values[constructed]);
            }
        } catch (...) {
            while (constructed != 0) {
                --constructed;
                allocator_traits::destroy(_allocator, _data + constructed);
            }
            if (_data != nullptr) {
                _allocator.deallocate(_data, _capacity);
            }
            throw;
        }
    }

    vector(std::size_t size, const T &value)
        : _size(size), _capacity(size), _data(size == 0 ? nullptr : _allocator.allocate(size)) {
        std::size_t constructed = 0;
        try {
            for (; constructed < _size; ++constructed) {
                allocator_traits::construct(_allocator, _data + constructed, value);
            }
        } catch (...) {
            while (constructed != 0) {
                --constructed;
                allocator_traits::destroy(_allocator, _data + constructed);
            }
            if (_data != nullptr) {
                _allocator.deallocate(_data, _capacity);
            }
            throw;
        }
    }

    vector(const vector &other)
        : _size(other._size),
          _capacity(other._capacity),
          _data(_capacity == 0 ? nullptr : _allocator.allocate(_capacity)) {
        std::size_t constructed = 0;
        try {
            for (; constructed < _size; ++constructed) {
                allocator_traits::construct(_allocator, _data + constructed,
                                            other._data[constructed]);
            }
        } catch (...) {
            while (constructed != 0) {
                --constructed;
                allocator_traits::destroy(_allocator, _data + constructed);
            }
            if (_data != nullptr) {
                _allocator.deallocate(_data, _capacity);
            }
            throw;
        }
    }

    vector(vector &&other) noexcept
        : _size(other._size), _capacity(other._capacity), _data(other._data) {
        other._size = 0;
        other._capacity = 0;
        other._data = nullptr;
    }

    vector &operator=(const vector &other) {
        if (this != &other) {
            vector copy(other);
            swap(copy);
        }
        return *this;
    }

    vector &operator=(vector &&other) noexcept {
        if (this != &other) {
            destroy_elements();
            if (_data != nullptr) {
                _allocator.deallocate(_data, _capacity);
            }
            _size = other._size;
            _capacity = other._capacity;
            _data = other._data;
            other._size = 0;
            other._capacity = 0;
            other._data = nullptr;
        }
        return *this;
    }

    T &operator[](std::size_t index) noexcept { return _data[index]; }
    const T &operator[](std::size_t index) const noexcept { return _data[index]; }

    T &at(std::size_t index) {
        if (index >= _size) {
            throw std::out_of_range("vector::at index out of range");
        }
        return _data[index];
    }

    const T &at(std::size_t index) const {
        if (index >= _size) {
            throw std::out_of_range("vector::at index out of range");
        }
        return _data[index];
    }

    void resize(std::size_t new_size) {
        if (new_size < _size) {
            while (_size > new_size) {
                --_size;
                allocator_traits::destroy(_allocator, _data + _size);
            }
        } else if (new_size > _size) {
            if (new_size > _capacity) {
                reserve(new_size);
            }
            std::size_t old_size = _size;
            try {
                for (; _size < new_size; ++_size) {
                    allocator_traits::construct(_allocator, _data + _size);
                }
            } catch (...) {
                while (_size > old_size) {
                    --_size;
                    allocator_traits::destroy(_allocator, _data + _size);
                }
                throw;
            }
        }
    }

    void resize(std::size_t new_size, const T &value) {
        if (new_size < _size) {
            resize(new_size);
            return;
        }
        if (new_size > _capacity) {
            reserve(new_size);
        }
        std::size_t old_size = _size;
        try {
            for (; _size < new_size; ++_size) {
                allocator_traits::construct(_allocator, _data + _size, value);
            }
        } catch (...) {
            while (_size > old_size) {
                --_size;
                allocator_traits::destroy(_allocator, _data + _size);
            }
            throw;
        }
    }

    void reserve(std::size_t new_capacity) {
        if (new_capacity > _capacity) {
            reallocate(new_capacity);
        }
    }

    void push_back(const T &value) {
        if (_size == _capacity) {
            reserve(_capacity == 0 ? 1 : _capacity * 2);
        }
        allocator_traits::construct(_allocator, _data + _size, value);
        ++_size;
    }

    void push_back(T &&value) {
        if (_size == _capacity) {
            reserve(_capacity == 0 ? 1 : _capacity * 2);
        }
        allocator_traits::construct(_allocator, _data + _size, std::move(value));
        ++_size;
    }

    const T &back() const { return _data[_size - 1]; }
    T &back() { return _data[_size - 1]; }

    void pop_back() {
        if (_size != 0) {
            --_size;
            allocator_traits::destroy(_allocator, _data + _size);
        }
    }

    std::size_t size() const { return _size; }
    std::size_t capacity() const { return _capacity; }

    ~vector() {
        destroy_elements();
        if (_data != nullptr) {
            _allocator.deallocate(_data, _capacity);
        }
    }

private:
    void swap(vector &other) noexcept {
        using std::swap;
        swap(_size, other._size);
        swap(_capacity, other._capacity);
        swap(_data, other._data);
    }
};

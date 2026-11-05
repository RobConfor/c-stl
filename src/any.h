#pragma once


template <typename T>
class any {
public:

    any();

    any operator=(any other);

    ~any();


    emplace();

    void reset(){
        delete data;
    }

    swap();


    has_value();

    type();

private:

// underlying data 
void* _data;

};
#include <iostream>
#include <string> 
#include "vector.h"

void testVector();
void testStack();
void testQueue();


int main(){

    testVector();

    testStack();

    testQueue();

    return 0;
}


void testVector(){
    vector<int> vec1;
    vector<int> vec2(10);
    vector<int> vec3(10, 1);
    vector<char> vec4(100, 'C');
    vector<std::string> vec6;
    vec6.push_back("hello");




    // vector<vector<std::string>> vec7;
    // vec7.resize(1);
    // vec7[0] = vec6;
}
#include <iostream>
#include <string> 
#include <utility>
#include <cassert>
#include "src/vector.h"
#include "src/unique.h"
#include "src/priority_queue.h"
#include "src/unordered_map.h"


void testVector();
void testStack();
void testQueue();
void testUnique();
void testPriority();
void testMap();


int main(){

    testVector();

    testUnique();

    testPriority();

    testMap();


    // testStack();

    // testQueue();

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


void testUnique(){
    unique_ptr<int> myptr = unique_ptr<int>(new int(10));
    unique_ptr<int> myptr1 = std::move(myptr);

    assert(myptr == nullptr);
    assert(!(myptr == myptr1));

}

void testPriority(){
    priority_queue<int> pq;
    pq.push(10);
    pq.push(2);
    pq.push(100);
    pq.push(50);

    assert(pq.top() == 100);
    pq.pop();

    assert(pq.top() == 50);

}


void testMap(){
    unordered_map<int, int> map;
    map[1]++;

    assert(map[1] == 1);

    map[20] = 5;
}
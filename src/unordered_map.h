#ifndef _MAP_H
#define _MAP_H

#include <cstdint>
#include <functional>
#include <vector>


// A bucket's status tells you whether it's empty, occupied, 
// or contains a deleted element.
enum class Status : uint8_t {
    Empty,
    Occupied,
    Deleted
};

template<typename K, typename V, typename Hasher = std::hash<K>>
class unordered_map {
public:
    // A bucket has a status, a key, and a value.
    struct Bucket {
        Status status = Status::Empty;
        K key;
        V val;
    };

    HashTable() {
        buckets.resize(20);
    }

    size_t size() const {
        return num_elements;
    }


    V& operator[](const K& key) {

        if(num_elements >= buckets.size()/2){
            rehash_and_grow();
        }

        Hasher hash;
        size_t index = find_bucket(key, hash);

        if(buckets[index].status == Status::Occupied){
            return buckets[index].val;
        }

        buckets[index] = Bucket{Status::Occupied, key, V()};
        ++num_elements;
        return buckets[index].val;
    }


    bool insert(const K& key, const V& val) {
        if(num_elements >= buckets.size()/2){
            rehash_and_grow();
        }

        Hasher hash;
        size_t index = find_bucket(key, hash);

        if(buckets[index].status == Status::Occupied){
            return false;
        }

        buckets[index] = Bucket{Status::Occupied, key, val};
        ++num_elements;
        return true;

    }
   
    
    size_t erase(const K& key) {
        Hasher hash;

        size_t index = delete_bucket(key, hash);


        if(index == buckets.size()){
            return 0;
        }

        buckets[index].status = Status::Deleted;
        ++num_deleted;
        --num_elements;
        return 1;

    }

private:
    size_t num_elements = 0;
    size_t num_deleted = 0; 
    std::vector<Bucket> buckets;

    void rehash_and_grow() {
        std::vector<Bucket> temp = buckets;
        buckets.clear();
        
        buckets.resize(temp.size() * 2);
        num_elements = 0;

        for(auto& a : temp){
            if(a.status == Status::Occupied){
                insert(a.key, a.val);
            }
        }
    }


    size_t find_bucket(const K& key, const Hasher& hash) const {
        size_t index = hash(key) % buckets.size();
        size_t first_deleted = buckets.size(); 
    
        for(size_t i = 0; i < buckets.size(); ++i){
            size_t probe = (index + i) % buckets.size();
    
            if(buckets[probe].status == Status::Empty){
                return (first_deleted < buckets.size()) ? first_deleted : probe;

            } else if(buckets[probe].status == Status::Deleted){
                if(first_deleted == buckets.size()) {
                    first_deleted = probe;
                }

            } else if(buckets[probe].status == Status::Occupied && buckets[probe].key == key){

                return probe; 
            }
        }
        
        return (first_deleted < buckets.size()) ? first_deleted : index;
    }

    size_t delete_bucket(const K& key, const Hasher& hash) {


        size_t index = hash(key) % buckets.size();

        for(size_t i = 0; i < buckets.size(); ++i){
            size_t probe = (index + i) % buckets.size();
    
            if(buckets[probe].status == Status::Empty){
                return buckets.size(); 
            } 

            else if(buckets[probe].status == Status::Occupied && buckets[probe].key == key){
                return probe;
            }
        }
    
        return buckets.size();
    }

};

#endif
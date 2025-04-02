#pragma once
#include "DynamicArray.hpp"

template <typename K, typename V>
class HashTable {
private:
    struct Entry {
        K key;
        V value;
        Entry* next;
        Entry(K k, V v) : key(k), value(v), next(nullptr) {}
    };

    DynamicArray<Entry*> buckets;
    size_t _size;
    
    size_t hash(const K& key) const {
        return std::hash<K>{}(key) % buckets.size();
    }

public:
    HashTable(size_t capacity = 101) : buckets(capacity), _size(0) {
        for (size_t i = 0; i < capacity; ++i) {
            buckets[i] = nullptr;
        }
    }
    
    ~HashTable() {
        clear();
    }

    void insert(const K& key, const V& value) {
        size_t index = hash(key);
        Entry* current = buckets[index];
        
        while (current) {
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        
        Entry* newEntry = new Entry(key, value);
        newEntry->next = buckets[index];
        buckets[index] = newEntry;
        _size++;
    }

    bool contains(const K& key) const {
        size_t index = hash(key);
        Entry* current = buckets[index];
        
        while (current) {
            if (current->key == key) return true;
            current = current->next;
        }
        return false;
    }

    V& operator[](const K& key) {
        size_t index = hash(key);
        Entry* current = buckets[index];
        
        while (current) {
            if (current->key == key) return current->value;
            current = current->next;
        }
        
        throw std::out_of_range("Key not found");
    }

    void clear() {
        for (size_t i = 0; i < buckets.size(); ++i) {
            Entry* current = buckets[i];
            while (current) {
                Entry* temp = current;
                current = current->next;
                delete temp;
            }
            buckets[i] = nullptr;
        }
        _size = 0;
    }

    size_t size() const { return _size; }
};
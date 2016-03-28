//
//  Map.h
//  Map
//
//  Created by Bradley Zhu on 1/22/16.
//  Copyright © 2016 Bradley Zhu. All rights reserved.
//

#ifndef Map_h
#define Map_h
#include <string>

typedef std::string KeyType;
typedef double ValueType;

class Map
{
public:
    Map();
    ~Map();
    Map(const Map& m);
    Map& operator=(const Map& m);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    void dump();
private:
    //I have a structure that contains the data necessary as well as pointers
    //to both the next and previous structures.
    struct Node {
        KeyType key;
        ValueType value;
        Node* next;
        Node* prev;
    };
    int m_size;
    Node* head;
    Node* tail;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);
#endif /* Map_h */

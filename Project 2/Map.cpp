//
//  Map.cpp
//  Map
//
//  Created by Bradley Zhu on 1/23/16.
//  Copyright © 2016 Bradley Zhu. All rights reserved.
//

#include <iostream>
#include "Map.h"

//Initializes the head and tail to point to nothing, and the size to be 0
Map::Map() {
    m_size = 0;
    head = nullptr;
    tail = nullptr;
}

//Destructs the map and deallocates all the memory
Map::~Map() {
    if(!empty()) {
        Node* temp = head;
        Node* temp2;
        while(temp != nullptr) {
            temp2 = temp->next;
            delete temp;
            temp = temp2;
        }
    }
    m_size = 0;
}

//Creates a copy of a map identical to Map m passed in.
Map::Map(const Map& m) {
    m_size = 0;
    head = nullptr;
    tail = nullptr;
    
    KeyType k;
    ValueType v;
    for(int i = 0; i < m.size(); i++) {
        m.get(i, k, v);
        insert(k, v);
    }
}

//Makes the left side into an identical version of the right side.  Returns it too.
Map& Map::operator=(const Map& m) {
    if(this!=&m) {
        Map temp(m);
        swap(temp);
    }
    return *this;
}

//Returns true if empty, false otherwise.
bool Map::empty() const {
    return m_size==0;
}

//Returns the size of the map
int Map::size() const {
    return m_size;
}

//Inserts the given key and value into a new node.  Returns false if the map
//already contains the key.  Returns true if they are inserted.
bool Map::insert(const KeyType& key, const ValueType& value) {
    if(contains(key)) {
        return false;
    }
    
    Node* n = new Node;
    n->key = key;
    n->value = value;
    n->next = nullptr;

    if(empty()) {
        head = n;
        tail = n;
        n->prev = nullptr;
    } else {
        n->prev = tail;
        tail->next = n;
        tail = n;
    }
    m_size++;
    return true;
}

//Returns false if the map doesn't currently have the key.  Returns true and
//updates the key with the new value otherwise.
bool Map::update(const KeyType& key, const ValueType& value) {
    if(empty()) {
        return false;
    }
    if(!contains(key)) {
        return false;
    }
    Node* temp = head;
    while (temp != nullptr) {
        if(temp->key == key) {
            temp->value = value;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

//Inserts the key and value pair if it doesn't exist.  Updates if the key is
//already in the Map.  It should always return true if implimented correctly.
bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if(contains(key)) {
        update(key, value);
    } else {
        insert(key, value);
    }
    return true;
}

//Deletes the key and value pair.  Returns false if the key is not in the Map.
bool Map::erase(const KeyType& key) {
    if(!contains(key)) {
        return false;
    }
    Node* temp = head;
    while(temp != nullptr) {
        if(temp->key == key) {
            if(size() == 1) {
                head = nullptr;
                tail = nullptr;
            } else if(temp == head) {
                Node* after = temp->next;
                head = after;
                after->prev = nullptr;
            } else if(temp == tail) {
                Node* before = temp->prev;
                tail = before;
                before->next = nullptr;
            } else {
                Node* after = temp->next;
                Node* before = temp->prev;
                after->prev = before;
                before->next = after;
            }
            temp = nullptr;
            delete temp;
            m_size--;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

//Returns true if the Map contains the key, false otherwise.
bool Map::contains(const KeyType& key) const {
    if(size()==0) {
        return false;
    }
    Node* temp = head;
    while(temp!=nullptr) {
        if(temp->key == key) {
            return true;
        }
        temp = temp->next;
    }
    return false;
}

//If the key is in the map, sets value to the value in the map that that key maps
//to and returns true.  Otherwise, makes no change to the value parameter of this
//function and return false.
bool Map::get(const KeyType& key, ValueType& value) const {
    if(!contains(key)) {
        return false;
    }
    Node* temp = head;
    while (temp != nullptr) {
        if(temp->key == key) {
            value = temp->value;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

//If 0 <= i < size(), copy into the key and value parameters the key and value of one
//of the key/value pairs in the map and return true.  Otherwise, leave the key and
//value parameters unchanged and return false.  It must give a different answer for
//different values of i and given the same i, it must give the same values.
bool Map::get(int i, KeyType& key, ValueType& value) const {
    if(i<0 || i >= size()) {
        return false;
    }
    if(empty()) {
        return false;
    }
    Node* temp = head;
    for(int j = 0; j < i; j++) {
        temp = temp->next;
    }
    key = temp->key;
    value = temp->value;
    return true;
}

// Exchange the contents of this map with the other one.
void Map::swap(Map& other) {
    Node* temph = other.head;
    Node* tempt = other.tail;
    other.head = head;
    other.tail = tail;
    head = temph;
    tail = tempt;
    int temp = other.m_size;
    other.m_size = m_size;
    m_size = temp;
}

//Adds a value into result if the following conditions are met:
//-If a key appears in exactly one of m1 and m2
//-If a key appears in both m1 and m2, with the same corresponding value in both
//
//If there exists a key that appears in both m1 and m2, but with different corresponding
//values, then this function returns false, but result should still go with the above rules.
bool combine(const Map& m1, const Map& m2, Map& result) {
    KeyType k;
    ValueType v;
    ValueType v2;
    Map m3(m1);
    Map m4(m2);
    bool randbool = true;
    while(result.size()!=0) {
        result.get(result.size()-1,k,v);
        result.erase(k);
    }
    for(int i = 0; i < m3.size(); i++) {
        m3.get(i, k, v);
        if(m4.get(k,v2)) {
            if(v!=v2) {
                randbool=false;
            } else {
                result.insert(k, v);
            }
        } else {
            result.insert(k, v);
        }
    }
    for (int j = 0; j < m4.size(); j++) {
        m4.get(j, k, v);
        if(m3.get(k,v2)) {
            if(v!=v2) {
                randbool=false;
            } else {
                result.insert(k, v);
            }
        } else {
            result.insert(k, v);
        }
    }
    return randbool;
}

//When this function returns, result must contain a copy of all the pairs in m1 whose keys don't appear in
//m2; it must not contain any other pairs.
void subtract(const Map& m1, const Map& m2, Map& result) {
    KeyType k;
    ValueType v;
    Map m3(m1);
    Map m4(m2);
    while(result.size()!=0) {
        result.get(result.size()-1,k,v);
        result.erase(k);
    }
    for(int i = 0; i < m3.size(); i++) {
        m3.get(i, k, v);
        if(!m4.contains(k)) {
            result.insert(k, v);
        }
    }
}

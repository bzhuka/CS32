#ifndef DISKMULTIMAP_H_
#define DISKMULTIMAP_H_

#include <string>
#include "MultiMapTuple.h"
#include "BinaryFile.h"
#include <stdio.h>
#include <functional>
#include <iostream>
#include <cstring>
#include <string>

class DiskMultiMap
{
public:
    
    class Iterator
    {
    public:
        Iterator();
        Iterator(string key, BinaryFile *b, int address);
        // You may add additional constructors
        bool isValid() const;
        Iterator& operator++();
        MultiMapTuple operator*();
        
    private:
        // Your private member declarations will go here
        BinaryFile *bf;
        int m_address;
        string m_key;
    };
    
    DiskMultiMap();
    ~DiskMultiMap();
    bool createNew(const std::string& filename, unsigned int numBuckets);
    bool openExisting(const std::string& filename);
    void close();
    bool insert(const std::string& key, const std::string& value, const std::string& context);
    Iterator search(const std::string& key);
    int erase(const std::string& key, const std::string& value, const std::string& context);
    
private:
    //Private member Structures
    struct Head {
        int tail;
        int deletedHead;
        int nbuckets;
    };
    struct Bucket {
        int bhead;
    };
    struct Node {
        Node() {}
        Node(MultiMapTuple m);
        char m_key[121];
        char m_value[121];
        char m_context[121];
        int next;
    };
    
    // Private member variables will go here
    BinaryFile bf;
    
    //Private methods
    int hashNum(const std::string& key, int nbuckets);
};

#endif // DISKMULTIMAP_H_
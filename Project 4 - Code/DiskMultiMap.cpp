//
//  DiskMultiMap.cpp
//  CyberSpider
//
//  Created by Bradley Zhu on 3/6/16.
//  Copyright © 2016 Bradley Zhu. All rights reserved.
//

#include "DiskMultiMap.h"

using namespace std;

//Iterator Functions
/*The default constructor must create an Iterator in an invalid state. This constructor must
 run in O(1) time.*/
DiskMultiMap::Iterator::Iterator() {
    m_address = 0;
    bf = nullptr;
    m_key = "";
}

/*You may write other Iterator constructors with whatever parameters you like. It is your
 choice whether the Iterator created by any such constructor is in a valid or invalid state.
 If the Iterator created is in a valid state, then it must Any such constructor MUST run in
 O(N/B) time or better, assuming that is being initialized to point into a hash table holding
 a generally diverse set of N items, with B buckets.*/
// NOT SURE IF IT RUNS IN O(N/B) TIME
DiskMultiMap::Iterator::Iterator(string key, BinaryFile *b, int address) {
    bf = b;
    m_address = address;
    m_key = key;
    Node temp;
    bf->read(temp, address);
    
    while (m_address > 0) {
        if (strcmp(temp.m_key, key.c_str()) == 0) {
            return;
        } else {
            m_address = temp.next;
            bf->read(temp, m_address);
        }
    }
    m_address = 0;
}

//Returns true if it has stuff
bool DiskMultiMap::Iterator::isValid() const{
    return m_address > 0;
}

/*You MUST implement a prefix increment operator (but not a decrement operator or a
 postfix increment operator) for the Iterator class. This operator MUST do nothing if the
 Iterator it’s called on is invalid. Otherwise, the ++ operator MUST advance the Iterator
 to the next association in the DiskMultiMap with the same key as the association the
 Iterator currently points to, if there is one; if there is no next association with the same
 key, then the ++ operator MUST change the Iterator’s state to invalid. The method
 returns a reference to the Iterator it's called on.*/
// NOT SURE IF IT RUNS IN O(N/B) TIME
DiskMultiMap::Iterator& DiskMultiMap::Iterator::operator++() {
    if (m_address <= 0) {
        return *this;
    }
    Node temp;
    bf->read(temp, m_address);
    int tempaddress = temp.next;
    while (tempaddress > 0) {
        bf->read(temp, tempaddress);
        if (strcmp(temp.m_key, m_key.c_str()) == 0) {
            m_address = tempaddress;
            return *this;
        } else {
            tempaddress = temp.next;
        }
    }
    m_address = 0;
    return *this;
}

/*You MUST implement the unary * operator for your Iterator class – this is the
 dereference operator, and it allows you to examine an association pointed to by a valid
 Iterator, using syntax akin to that of dereferencing a C++ pointer or an STL iterator. The
 * operator MUST return an object of type MultiMapTuple:*/
MultiMapTuple  DiskMultiMap::Iterator::operator*() {
    Node temp;
    MultiMapTuple tuple;
    if (m_address > 0) {
        bf->read(temp, m_address);
        //cout << "Value: " << temp.m_value << endl;
    } else {
        return tuple;
    }
    tuple.key = temp.m_key;
    tuple.value = temp.m_value;
    tuple.context = temp.m_context;
    return tuple;
}


//Constructor, not sure what to do.
DiskMultiMap::DiskMultiMap() {
}

//Destructor, not sure what to do
DiskMultiMap::~DiskMultiMap() {
    if (bf.isOpen()) {
        bf.close();
    }
}

/*You MUST implement the createNew() method. This method MUST create an empty,
 open hash table in a binary disk file with the specified filename, with the specified
 number of empty buckets. If there is already an existing disk file with the same filename
 at the time that the createNew() method is called, then your createNew() method MUST
 overwrite the original file with a brand-new file (our BinaryFile::createNew() method
 will automatically perform this overwriting for you). Once the hash table has been
 created, you may then proceed to add new associations, search through it, or delete
 associations with the object’s other methods.*/
bool DiskMultiMap::createNew(const std::string &filename, unsigned int numBuckets) {
    bool noerr = true;
    
    //close current file if any, and create a new file.
    if (bf.isOpen()) {
        bf.close();
    }
    noerr = bf.createNew(filename);
    if (!noerr) {
        return false;
    }
    
    //figure out where I can start adding nodes in my memory while allocating stuff
    //I know I need.  I allocate a head, and all my buckets.
    Head m_head;
    int currposition = sizeof(Head);
    //cout << "Size of Buck:" << sizeof(Bucket) << endl;
    //cout << "position : " << currposition;
    Bucket b;
    for (int i = 0; i < numBuckets; i++) {
        b.bhead = 0;
        noerr = noerr && bf.write(b, currposition);
        currposition += sizeof(Bucket);
    }
    
    //I set m_head's tail equal to start of it.
    m_head.tail = currposition;
    noerr = noerr && bf.read(b, currposition - sizeof(Bucket));
    noerr = noerr && bf.write(b, currposition- sizeof(Bucket));
    
    //Finish off initializing variables in my Head
    m_head.deletedHead = 0;
    m_head.nbuckets = numBuckets;
    
    noerr = noerr && bf.write(m_head, 0);
    
    //Debugging making sure my head works
    /*Head test;
    bf.read(test, 0);
    cout << test.head << endl;
    cout << test.tail << endl;
    cout << test.deletedHead << endl;
    cout << test.nbuckets << endl;*/
    
    //Debugging, print every bucket's head pointer
    /*Bucket test;
    bf.read(test, sizeof(Head));
    int currindex = sizeof(Head);
    while (currindex > 0) {
        cout << "Bucket next: " << test.bhead << endl;
        currindex = test.bnext;
        bf.read(test, test.bnext);
    }*/
    
    return noerr;
}

/*You MUST implement the openExisting() method. This method MUST open a
 previously-created disk-based hash table with the specified filename. If the specified disk
 file does not exist on the hard drive, then your method MUST return false. If your method
 succeeds in opening the specified file, then it MUST return true. Once the hash table has
 been opened, you may then proceed to search through it, add new associations, or delete
 associations.*/
bool DiskMultiMap::openExisting(const std::string& filename) {
    if (bf.isOpen()) {
        bf.close();
    }
    return bf.openExisting(filename);
}

/*You MUST implement the close() method. This method is used to ensure that all data
 written to your hash table is safely saved to the binary disk file, and the binary disk file is
 closed. If a client doesn't call this method, then your DiskMultiMap destructor MUST
 close the binary disk file. Obviously, the client should only call the close() method if they
 first called the createNew() or openExisting() methods to create/open a hash table data
 file. Otherwise, the close() method should do nothing, as it should is called on an alreadyclosed
 file.*/
void DiskMultiMap::close() {
    if (bf.isOpen()) {
        bf.close();
    }
}

/*You MUST implement the insert() method. This method adds a new association to the
 disk-based hash table associating the specified key, e.g. “foo.exe”, with the associated
 value and context strings, e.g. “www.google.com” and “m12345”. Since you’re
 implementing a multimap, your insert function MUST be able to properly store duplicate
 associations.*/
//MUST RUN IN O(N/B) TIME, NOT SURE IF I DO THAT
bool DiskMultiMap::insert(const std::string &key, const std::string &value, const std::string &context) {
    if(!bf.isOpen())
        return false;
    
    //Returns false if any are over 120 characters
    if(key.size() > 120 || value.size() > 120 || context.size() > 120) {
        return false;
    }
    
    //Loads head
    Head m_head;
    bf.read(m_head, 0);
    
    //Hashes based on (string) key value
    int bucketnum = hashNum(key, m_head.nbuckets);
    
    //Uses deleted node if possible, creates new node otherwise.
    Node temp;
    int nodeLoc;
    if (m_head.deletedHead > 0) {
        //Sets location of node at a deleted node's location.  Updates list of
        //deleted heads afterwards.
        nodeLoc = m_head.deletedHead;
        bf.read(temp, m_head.deletedHead);
        m_head.deletedHead = temp.next;
    } else {
        nodeLoc = m_head.tail;
        m_head.tail += sizeof(Node);
    }
    bf.write(m_head, 0);
    
    //Creates the node with values specified
    MultiMapTuple tup;
    tup.key = key;
    tup.value = value;
    tup.context = context;
    Node n(tup);
    //Updates pointers accordingly
    Bucket tempB;
    int currBucket = sizeof(Head) + sizeof(Bucket) * bucketnum;
    //Sets head of bucket to newly created node, sets newly created node's next pointer to old head
    bf.read(tempB, currBucket);
    int oldhead = tempB.bhead;
    tempB.bhead = nodeLoc;
    n.next = oldhead;
    bf.write(tempB, currBucket);
    //Writes the created Node with correct values and pointer to next node of bucket
    bf.write(n, nodeLoc);
    
    return true;
}

//Creates a new node, assigns strings into cstrings and puts in variables of Node
DiskMultiMap::Node::Node(MultiMapTuple m) {
    strcpy(m_key, m.key.c_str());
    strcpy(m_value, m.value.c_str());
    strcpy(m_context, m.context.c_str());
}

/*You MUST implement the search() method. This method is used to find all associations
 in the hash table that match the specified key string. Your method MUST return a
 DiskMultiMap::Iterator object, which is analogous to a C++ iterator. The user can then
 use this iterator to enumerate all associations that matched the specified key string. If no
 associations matched the specified key string, then the Iterator returned must be invalid
 (i.e., one for which DiskMultiMap::Iterator::isValid() returns false).*/
DiskMultiMap::Iterator DiskMultiMap::search(const std::string &key) {
    //Reads in heads
    Head m_head;
    bf.read(m_head, 0);
    
    //Hashes based on (string) key value
    int bucketnum = hashNum(key, m_head.nbuckets);
    
    //Finds address to give to constructor of Iterator
    Bucket buck;
    bf.read(buck, sizeof(Head) + bucketnum * sizeof(Bucket));
    
    Iterator i(key, &bf, buck.bhead);
    return i;
}

/*You MUST implement the erase() method. This method MUST remove all associations
 in the hash table that exactly match the specified key, value, and context strings passed in
 and return the number of associations removed. Your hash table MUST somehow track
 the location of all deleted nodes, so these nodes can be reused should one or more new
 associations later be added to the hash table after deletion.*/
//NOT SURE IF IT RUNS O(N/B)
int DiskMultiMap::erase(const std::string &key, const std::string &value, const std::string &context) {
    int counter = 0;
    
    //Reads in head
    Head m_head;
    bf.read(m_head, 0);
    
    //Hashes based on (string) key value
    int bucketnum = hashNum(key, m_head.nbuckets);
    
    Bucket buck;
    bf.read(buck, sizeof(Head) + bucketnum * sizeof(Bucket));
    
    //Searches through given bucket
    Node temp;
    Node prevtemp;
    int tempaddress = buck.bhead;
    int prevaddress = buck.bhead;
    int nextaddress;
    while (tempaddress > 0) {
        bf.read(temp, tempaddress);
        nextaddress = temp.next;
        //if we found something with same values
        if (strcmp(temp.m_key, key.c_str()) == 0 && strcmp(temp.m_value, value.c_str()) == 0 && strcmp(temp.m_context, context.c_str()) == 0) {
            counter++;
            //Change the old previous node's next pointer to correct pointer.  If curraddress is the head
            //that means its the first node of a bucket.
            bf.read(buck, sizeof(Head) + bucketnum * sizeof(Bucket));
            if (tempaddress == buck.bhead) {
                buck.bhead = nextaddress;
                bf.write(buck, sizeof(Head) + bucketnum * sizeof(Bucket));
            } else {
                bf.read(prevtemp, prevaddress);
                prevtemp.next = nextaddress;
                bf.write(prevtemp, prevaddress);
            }
            //Move it to the deleted list
            bf.read(m_head, 0);
            int oldhead = m_head.deletedHead;
            m_head.deletedHead = tempaddress;
            bf.write(m_head, 0);
            //Change the node's next pointer to point to the correct next node.
            temp.next = oldhead;
            bf.write(temp, tempaddress);
        } else {
            //if its not the node we want, we want to update previous address.  If it was the node, then previous address shouldn't change
            prevaddress = tempaddress;
        }
        tempaddress = nextaddress;
    }
    return counter;
}

//Private method, returns the bucket num
int DiskMultiMap::hashNum(const std::string& key, int nbuckets) {
    
    hash<string> str_hash;
    size_t hashnum = str_hash(key);
    return hashnum % nbuckets;
}
//
//  IntelWeb.cpp
//  CyberSpider
//
//  Created by Bradley Zhu on 3/6/16.
//  Copyright © 2016 Bradley Zhu. All rights reserved.
//

#include "IntelWeb.h"

using namespace std;

//Constructor, doesn't need to initialize anything
IntelWeb::IntelWeb() {
    
}

//Destructor, closes files
IntelWeb::~IntelWeb() {
    close();
}

/*The createNew() method MUST create a set of empty disk-based data structures to hold
 information about all of the telemetry log data that you’ll be loading from one or more
 telemetry log data files. Your disk-based data structures MUST consist of one or more
 DiskMultiMaps, and you MUST not use any other disk files.*/
bool IntelWeb::createNew(const std::string &filePrefix, unsigned int maxDataItems) {
    if (!toMap.createNew(filePrefix + "-to-hash-table", maxDataItems / .75)) {
        toMap.close();
        return false;
    }
    if(!fromMap.createNew(filePrefix + "-from-hash-table",maxDataItems / .75)) {
        toMap.close();
        fromMap.close();
        return false;
    }
    return true;
}

/*The openExisting() method is used to open an existing set of DiskMultiMap(s) that were
 previously created and populated using calls to IntelWeb's createNew() with the given file
 prefix and ingest(). Any data previously ingested into these DiskMultiMaps will be
 available for access after the openExisting() method has been called.*/
bool IntelWeb::openExisting(const std::string &filePrefix) {
    toMap.close();
    fromMap.close();
    if (!toMap.openExisting(filePrefix + "-to-hash-table")) {
        toMap.close();
        return false;
    }
    if(!fromMap.openExisting(filePrefix + "-from-hash-table")) {
        toMap.close();
        fromMap.close();
        return false;
    }
    return true;
}

/*The close() method is used to close any DiskMultiMaps created or opened by this
 IntelWeb object.*/
void IntelWeb::close() {
    toMap.close();
    fromMap.close();
}

/*The ingest() method is used to insert all the data from a telemetry log file of the specified
 name into your disk-based data structures. While you will ingest data from each
 telemetry file into your data structures once, you’ll be searching through these structures
 (using crawl()) over and over and over, so the speed of ingestion is much less important
 than the speed at which you can search your data structures. So you MUST design your
 data structures to facilitate fast searching rather than trying to make insertion fast if it will
 slow down searching.*/
bool IntelWeb::ingest(const std::string &telemetryFile) {
    string line;
    ifstream m_file(telemetryFile);
    if (m_file.is_open()) {
        while(getline(m_file, line)) {
            if(!process(line)) {
                return false;
            }
        }
    }
    return true;
}

//Private function, takes a string and puts it into my two DisMultiMaps
bool IntelWeb::process(string line) {
    bool bb = true;
    string buf;
    stringstream ss(line);
    
    vector<string> tokens;
    while (ss >> buf) {
        tokens.push_back(buf);
    }
    
    string from = tokens[1];
    string to = tokens[2];
    string context = tokens[0];
    
    bb = bb && toMap.insert(from, to, context);
    bb = bb && fromMap.insert(to, from, context);
    
    return bb;
}

/*The crawl() method is responsible for (a) discovering and outputting an ordered
 vector of all malicious entities found in the previously-ingested telemetry, and (b)
 outputting an ordered vector of every interaction discovered that includes at least
 one malicious entity, by “crawling” through the ingested telemetry!*/
unsigned int IntelWeb::crawl(const std::vector<std::string> &indicators, unsigned int minPrevalenceToBeGood, std::vector<std::string> &badEntitiesFound, std::vector<InteractionTuple> &interactions) {
    //Members for this function
    unsigned int bad = 0;
    bool fromMapBool = false;
    string key;
    DiskMultiMap::Iterator i;
    DiskMultiMap::Iterator j;
    MultiMapTuple tuple;
    set<InteractionTuple, cus_compare> badSets;
    set<InteractionTuple>::iterator vi;
    unordered_set<string> badEnts;
    unordered_set<string>::iterator enti;
    unordered_map<string, int> pMap;
    unordered_map<string, int>::iterator pMapIt;
    queue<string> ind;
    
    //Put the first indicators in the queue.
    for (int i = 0; i < indicators.size(); i++) {
        ind.push(indicators[i]);
        int prev = 0;
        //Have to add to set if it appears at all in telemetry
        pMapIt = pMap.find(indicators[i]);
        //If it's not already in my map
        if (pMapIt == pMap.end()) {
            //Calculate prevalence and put in map
            prev = prevalence(indicators[i]);
            pMap.insert({indicators[i], prev});
        } else {
            //its in the map and pMapIt second value is prevalence
            prev = (*pMapIt).second;
        }
        if (prev > 0) {
            badEnts.insert(indicators[i]);
        }
    }
    
    //Shouldn't stop until my queue is empty
    while (!ind.empty()) {
        key = ind.front();
        ind.pop();
        //Makes sure we haven't used the key already
        //        enti = badEnts.find(key);
        //        if (enti == badEnts.end()) {
        //First goes through the toMap for all related entries
        i = toMap.search(key);
        j = fromMap.search(key);
        //Go through every  entry in toMap with the key
        while(i.isValid() || j.isValid()) {
            fromMapBool = false;
            if (i.isValid()) {
                tuple = *i;
                ++i;
            } else {
                tuple = *j;
                ++j;
                fromMapBool = true;
            }
            if (tuple.key == key) {
                //We know that we found an entry.  We should add to badSets if we haven't already found it.  If we're reading from the fromMap, then the key and value are switched before adding it.
                InteractionTuple iTup;
                if (!fromMapBool) {
                    InteractionTuple tempTup(tuple.key, tuple.value, tuple.context);
                    iTup = tempTup;
                } else {
                    InteractionTuple tempTup(tuple.value, tuple.key, tuple.context);
                    iTup = tempTup;
                }
                //vi = badSets.find(iTup);
                //if (vi==badSets.end() ) {
                //Make sure we didn't already find the bad entity which is the value attached to the key
                enti = badEnts.find(tuple.value);
                if (enti==badEnts.end()) {
                    int prev;
                    //add it to bad entities and my queue iff prevalence value is low enough.
                    pMapIt = pMap.find(tuple.value);
                    //If it's not already in my map
                    if (pMapIt == pMap.end()) {
                        //Calculate prevalence and put in map
                        prev = prevalence(tuple.value);
                        pMap.insert({tuple.value, prev});
                    } else {
                        //its in the map and pMapIt second value is prevalence
                        prev = (*pMapIt).second;
                    }
                    
                    //If its prevalence is less that min to be good
                    if (prev < minPrevalenceToBeGood) {
                        bad++;
                        badEnts.insert(tuple.value);
                        ind.push(tuple.value);
                    }
                }
                
                //}
                //Even if its not a unique value, its a unique set, still should add to badSets
                badSets.insert(iTup);
            }
        }
    }
    
    //clears out both interactions and badEntities
    while (!interactions.empty()) {
        interactions.pop_back();
    }
    while (!badEntitiesFound.empty()) {
        badEntitiesFound.pop_back();
    }
    
    //Put everything in badSets in interactions
    vi = badSets.begin();
    while (vi != badSets.end()) {
        interactions.push_back(*vi);
        vi++;
    }
    //Put everything in badEnts in badEntitiesFound
    enti = badEnts.begin();
    while (enti != badEnts.end()) {
        badEntitiesFound.push_back(*enti);
        enti++;
    }
    sort(badEntitiesFound.begin(), badEntitiesFound.end());
    return bad;
}

//Private function, returns the prevalence of a key
int IntelWeb::prevalence(string key) {
    int count = 0;
    DiskMultiMap::Iterator i;
    i= toMap.search(key);
    while (i.isValid()) {
        count++;
        ++i;
    }
    i = fromMap.search(key);
    while (i.isValid()) {
        count++;
        ++i;
    }
    return count;
}

/*The purge command is used to remove ALL references to a specified entity (e.g., a
 filename or website) from your IntelWeb disk-based data structures. Subsequent crawls of
 your telemetry using the crawl() method MUST NOT find any references to the specified
 entry after it’s been purged.*/
bool IntelWeb::purge(const std::string &entity) {
    int count = 0;
    MultiMapTuple tuple;
    DiskMultiMap::Iterator i;
    
    //Deletes it from toMap
    i = toMap.search(entity);
    while (i.isValid()) {
        tuple = *i;
        ++i;
        count += toMap.erase(tuple.key, tuple.value, tuple.context);
        count += fromMap.erase(tuple.value, tuple.key, tuple.context);
    }
    /*
    //Deletes it from fromMap
    i = fromMap.search(entity);
    while (i.isValid()) {
        tuple = *i;
        ++i;
        count += fromMap.erase(tuple.key, tuple.value, tuple.context);
        count += toMap.erase(tuple.value, tuple.key, tuple.context);
    }*/
    
    return count > 0;
}



















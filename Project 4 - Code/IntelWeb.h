#ifndef INTELWEB_H_
#define INTELWEB_H_

#include "InteractionTuple.h"
#include "DiskMultiMap.h"
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <vector>

class IntelWeb
{
public:
    IntelWeb();
    ~IntelWeb();
    bool createNew(const std::string& filePrefix, unsigned int maxDataItems);
    bool openExisting(const std::string& filePrefix);
    void close();
    bool ingest(const std::string& telemetryFile);
    unsigned int crawl(const std::vector<std::string>& indicators,
                       unsigned int minPrevalenceToBeGood,
                       std::vector<std::string>& badEntitiesFound,
                       std::vector<InteractionTuple>& interactions
                       );
    bool purge(const std::string& entity);
    
private:
    // Your private member declarations will go here
    //Private members
    DiskMultiMap toMap;
    DiskMultiMap fromMap;
    
    //Private functions
    bool process(string line);
    int prevalence(string key);
    
    //Private Structures
    struct cus_compare {
        bool operator() (const InteractionTuple &a, const InteractionTuple &b) const {
            if (a.context == b.context) {
                if (a.from == b.from) {
                    return a.to < b.to;
                }
                return a.from < b.from;
            }
            return a.context < b.context;
        }
    };
};

#endif // INTELWEB_H_

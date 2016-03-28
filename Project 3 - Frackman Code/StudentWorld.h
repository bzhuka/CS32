#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>

using namespace std;
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
	 : GameWorld(assetDir)
    {
	}
    
    virtual ~StudentWorld();
    
    virtual int init();
    int min(int i, int j) const;
    int max(int i, int j) const;
    bool onDirt(int x, int y) const;
    bool onBoulder(int x, int y) const;
    bool clearDirtBoulder(int x, int y) const;
    bool removeDirt(int x, int y);
    void removeBarrel();
    bool collFrack(int x, int y, double r) const;
    bool annoyP(int x, int y, double r, int hp, int rpoints, int hpoints);
    bool annoyF(int x, int y, int r, int points) const;
    bool bribeP(int x, int y) const;
    bool canMove(int x, int y) const;
    bool canShout(int x, int y, Actor::Direction d) const;
    int straightFrack(int x, int y, int r) const;
    int isNearFrack(int &x, int &y) const;
    int annoyedProtester(int &x, int &y) const;
    void squirt(int x, int y);
    void addSquirts() const;
    void addGold() const;
    void addSonar() const;
    void placeGold(int x, int y);
    void reveal(int x, int y);
    virtual int move();
    virtual void cleanUp();

private:
    Dirt* dirt[64][60];
    int map[64][64];
    int mapToFrack[64][64];
    vector <Obj*> objs;
    Frackman* mc;
    
    int m_ticks;
    int m_barrels;
    int m_tickProtesters;
    int m_waitProtesters;
    int m_currProtesters;
    int m_maxProtesters;
    int m_proxHardcore;
    
    bool canPlace(int x, int y) const;
    bool noDirt(int x, int y) const;
    bool coll(int x, int y, int i, int j, double r) const;
    
    void updateMap(int x, int y, int m[64][64], int r);
    void createProtester();
    void setDisplay();
    string format(int sc, int lv, int li, int hp, int squ, int g, int son, int bar);
    
    struct Coord
    {
    public:
        Coord(int rr, int cc, int zz) : m_r(rr), m_c(cc), m_z(zz) {}
        int r() const { return m_r; }
        int c() const { return m_c; }
        int z() const { return m_z; }
    private:
        int m_r;
        int m_c;
        int m_z;
    };
};

#endif // STUDENTWORLD_H_

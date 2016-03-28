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
    
    ~StudentWorld();
    
    virtual int init();
    bool canPlace(int x, int y);
    bool noDirt(int x, int y);
    void removeDead();
    bool canMove(int x, int y);
    void squirt(int x, int y);
    void placeGold(int x, int y);
    virtual int move();
    bool coll(int x, int y, int i, int j, double r);
    bool onDirt(int x, int y);
    bool onBoulder(int x, int y);
    bool clearDirtBoulder(int x, int y);
    bool removeDirt(int x, int y);
    void removeBarrel();
    bool hitP(int x, int y, int r, int points);
    bool hitF(int x, int y, int r, int points);
    virtual void cleanUp();
    int min(int i, int j);
    int max(int i, int j);

private:
    Dirt* dirt[64][60];
    vector <Obj*> objs;
    Frackman* mc;
    
    int m_barrels;
    
    void setDisplay();
    string format(int sc, int lv, int li, int hp, int squ, int g, int son, int bar);
};

#endif // STUDENTWORLD_H_

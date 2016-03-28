#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int STABLE = 0;
const int WAITING = 1;
const int FALLING = 2;

class StudentWorld;
class Frackman;

class Actor: public GraphObject {
    public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, bool vis = true): GraphObject(imageID, startX, startY, dir, size, depth){
        setVisible(vis);
    }
    ~Actor();
    virtual bool doSomething() = 0;
};

class Dirt: public Actor {
    public:
    Dirt(int startX,int startY): Actor(IID_DIRT, startX, startY, right, .25, 3) {
    }
    ~Dirt() {
    }
    virtual bool doSomething() {
        return false;
    }
};

class Obj: public Actor {
public:
    Obj(StudentWorld* w, Frackman* f, int ID, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, bool vis = true): Actor(imageID, startX, startY, dir, size, depth, vis) {
        m_id = ID;
        m_w = w;
        m_Hp = 0;
        m_f = f;
    }
    int getHp();
    void setHp(int hp);
    void setId(int x);
    int getId();
    bool isAlive();
    void setCoord(Direction d, int& x, int& y, int r);
    StudentWorld* getWorld();
    Frackman* getFrack();
    virtual bool doSomething() = 0;
    
private:
    int m_id;
    int m_Hp;
    StudentWorld* m_w;
    Frackman* m_f;
};

class Human: public Obj {
public:
    Human(StudentWorld* w, Frackman* f, int ID, int imageID, int startX, int startY): Obj(w, f, ID, imageID, startX, startY) {
    }
    void annoy(int x){
        setHp(getHp() - x);
    }
    virtual bool doSomething() = 0;
private:
};

class Barrel: public Obj {
public:
    Barrel(StudentWorld* w, Frackman* f, int startX, int startY): Obj(w, f, 5, IID_BARREL, startX, startY, right, 1.0, 2, false) {
        setHp(1);
        isvis = false;
    }
    virtual bool doSomething();
private:
    bool isvis;
};

class Boulder: public Obj {
public:
    Boulder(StudentWorld* w, Frackman *f, int startX, int startY): Obj(w, f, 4, IID_BOULDER, startX, startY, down, 1.0, 1) {
        setHp(1);
        state = STABLE;
        timer = 30;
    }
    virtual bool doSomething();
private:
    int state;
    int timer;
};

class Water: public Obj {
public:
    Water(StudentWorld* w, Frackman* f, int startX, int startY);
    virtual bool doSomething();
};

class Squirt: public Obj {
public:
    Squirt(StudentWorld* w, Frackman* f, int startX, int startY);
    virtual bool doSomething();
private:
    Direction m_d;
};

class Gold: public Obj {
public:
    Gold(StudentWorld* w, Frackman* f, int startX, int startY, bool placed);
    virtual bool doSomething();
private:
    bool m_placed;
};

class Frackman: public Human {
    public:
    Frackman(StudentWorld* w): Human(w, this, 0, IID_PLAYER, 30, 60) {
        setHp(10);
        m_squirts = 30;
        m_sonar = 1;
        m_gold = 0;
        m_d = right;
    }
    ~Frackman();
    int getSquirts();
    void addSquirts();
    int getGold();
    void addGold();
    int getSonar();
    
    virtual bool doSomething();
    
    private:
    int m_squirts;
    int m_sonar;
    int m_gold;
    Direction m_d;
    
    void squirt();
    void placeGold();
};

#endif // ACTOR_H_

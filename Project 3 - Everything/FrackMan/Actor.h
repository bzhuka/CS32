#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

const int STABLE = 0;
const int WAITING = 1;
const int FALLING = 2;

class StudentWorld;

class Actor: public GraphObject {
    public:
    Actor(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, bool vis = true);
    virtual ~Actor();
    virtual bool doSomething() = 0;
    Direction direc(int x);
};

class Dirt: public Actor {
    public:
    Dirt(int startX,int startY): Actor(IID_DIRT, startX, startY, right, .25, 3) {
    }
    virtual ~Dirt() {
    }
    virtual bool doSomething() {
        return false;
    }
};

class Obj: public Actor {
public:
    Obj(StudentWorld* w, int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0, bool vis = true);
    virtual ~Obj();
    int getHp() const;
    void setHp(int hp);
    virtual bool canPassThrough() const;
    virtual bool canPickUp() const;
    virtual bool canAnnoy() const;
    virtual bool isHardcore() const;
    void setVis(bool isvis);
    virtual bool isAlive() const;
    void setCoord(Direction d, int& x, int& y, int r);
    StudentWorld* getWorld() const;
    virtual bool damage(int hp);
    virtual void bribe();
    virtual bool doSomething() = 0;
    
private:
    int m_Hp;
    StudentWorld* m_w;
};

class Barrel: public Obj {
public:
    Barrel(StudentWorld* w, int startX, int startY);
    virtual ~Barrel();
    virtual bool canPickUp() const;
    virtual bool doSomething();
private:
    bool isvis;
};

class Boulder: public Obj {
public:
    Boulder(StudentWorld* w, int startX, int startY);
    virtual ~Boulder();
    virtual bool canPassThrough() const;
    virtual bool doSomething();
private:
    int state;
    int timer;
};

class Water: public Obj {
public:
    Water(StudentWorld* w, int startX, int startY);
    virtual ~Water();
    virtual bool doSomething();
};

class Squirt: public Obj {
public:
    Squirt(StudentWorld* w, int startX, int startY, Direction d);
    virtual ~Squirt();
    virtual bool doSomething();
private:
    Direction m_d;
};

class Gold: public Obj {
public:
    Gold(StudentWorld* w, int startX, int startY, bool placed);
    virtual ~Gold();
    virtual bool canPickUp() const;
    virtual bool doSomething();

private:
    bool m_placed;
};

class Sonar: public Obj {
public:
    Sonar(StudentWorld* w, int startX, int startY, int hp);
    virtual ~Sonar();
    virtual bool doSomething();
};

class Protester: public Obj {
public:
    Protester(StudentWorld *w, int wait, int iid);
    virtual ~Protester();
    virtual bool isAlive() const;
    virtual bool canAnnoy() const;
    virtual bool damage(int hp);
    virtual void bribe();
    virtual bool doSomething();
protected:
    bool canMove();
    void gaveUp();
    bool doShout();
    bool seesFrackman();
    void regMove();
    void doTurn(Direction d, int x, int y);
    void setWait(int x);
private:
    int m_wait;
    int m_currwait;
    int m_squaresToMove;
    int m_waitShout;
    int m_waitTurn;
};

class HardcoreProtester: public Protester {
public:
    HardcoreProtester(StudentWorld *w, int wait);
    virtual ~HardcoreProtester();
    virtual void bribe();
    virtual bool isHardcore() const;
    virtual bool doSomething();
private:
    bool closeToFrack();
};

class Frackman: public Obj {
    public:
    Frackman(StudentWorld* w);
    virtual ~Frackman();
    int getSquirts() const;
    void addSquirts();
    int getGold() const;
    void addGold();
    int getSonar() const;
    void addSonar();
    virtual bool doSomething();
    
    private:
    int m_squirts;
    int m_sonar;
    int m_gold;
    Direction m_d;
    
    void squirt();
    void placeGold();
    void activateSonar();
};

#endif // ACTOR_H_

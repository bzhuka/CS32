 #include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp


//Deconstructor of Actor.  Not sure what to put in there yet.
Actor::~Actor() {
}

//returns HP
int Obj::getHp() {
    return m_Hp;
}

//setHp, sets hp obviously.  Only sets to 0 or above, returns true/false.
void Obj::setHp(int hp) {
    m_Hp = hp;
}

//sets ID
void Obj::setId(int x) {
    m_id = x;
}

//gets ID
int Obj::getId() {
    return m_id;
}

//isAlive, returns true is the actor is alive
bool Obj::isAlive() {
    if (m_Hp > 0) {
        return true;
    }
    return false;
}

//sets parameters to correct coordinate with correct direction
void Obj::setCoord(GraphObject::Direction d, int &x, int &y, int r) {
    if (d == left) {
        x = x - r;
    } else if (d == right) {
        x = x + r;
    } else if (d == up) {
        y = y + r;
    } else if (d == down) {
        y = y - r;
    }
}

//getWorld, returns pointer to StudentWorld
StudentWorld* Obj::getWorld() {
    return m_w;
}

//getFrack, returns pointer to Frack
Frackman* Obj::getFrack() {
    return m_f;
}

Frackman::~Frackman() {
}

//doSomething of barrel
bool Barrel::doSomething() {
    if (!isAlive()) {
        return false;;
    }
    if ((!isvis) && getWorld()->coll(getX(), getY(), getFrack()->getX(), getFrack()->getY(), 4.0)) {
        isvis = true;
        setVisible(true);
        return false;;
    }
    if (getWorld()->coll(getX(), getY(), getFrack()->getX(), getFrack()->getY(),3.0)) {
        setHp(0);
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->removeBarrel();
        return true;
    }
    return false;
}

//doSomething of boulder
bool Boulder::doSomething() {
    if(!isAlive()) {
        return false;
    }
    
    //stable state
    if(state == 0) {
        if(!(getWorld()->onDirt(getX(), getY()))) {
            state = 1;
        }
        return false;
    }
    //not sure if its else if or just if right now
    else if (state == 1) {
        if (timer == 0) {
            state = 2;
        }
        timer--;
        return false;
    }
    //see above
    else if (state == 2) {
        if(getWorld()->onDirt(getX(), getY())) {
            setHp(0);
        } else if(getY() == 0) {
            setHp(0);
        } else if(getWorld()->onBoulder(getX(), getY())) {
            setHp(0);
        } else {
            moveTo(getX(), getY() -1);
        }
        if (getWorld()->hitP(getX(), getY(), 3, 100)) {
            return true;
        }
        if (getWorld()->hitF(getX(), getY(), 3, 100)) {
            return true;
        }
    }
    return false;
}

//constructor for water
Water::Water(StudentWorld* w, Frackman* f, int startX, int startY): Obj(w, f, 9, IID_WATER_POOL, startX, startY, right, 1.0, 2) {
    setHp(w->min(100, 300 - 10*w->getLevel()));
}

//doSomething for water
bool Water::doSomething() {
    if (!isAlive()) {
        return false;
    }
    if (getWorld()->coll(getX(), getY(), getFrack()->getX(), getFrack()->getY(), 3.0)) {
        setHp(0);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getFrack()->addSquirts();
        getWorld()->increaseScore(100);
        return true;
    }
    setHp(getHp()-1);
    return false;
}

//constructor for squirt
Squirt::Squirt(StudentWorld* w, Frackman* f, int startX, int startY): Obj(w, f, 3, IID_WATER_SPURT, startX, startY, f->getDirection(), 1.0, 1) {
    setHp(5);
    m_d = f->getDirection();
}

//doSomething for squirt
bool Squirt::doSomething() {
    //checks if hit protestor IMPLIMENT LATER
    if(!isAlive()) {
        return false;
    }
    int x = getX();
    int y = getY();
    setCoord(m_d, x, y, 1);
    if(!getWorld()->clearDirtBoulder(x, y)) {
        setHp(0);
        return false;
    }
    cout << x << endl;
    moveTo(x, y);
    setHp(getHp()-1);
    return false;
}

//Constructor of Gold
Gold::Gold(StudentWorld* w, Frackman* f, int startX, int startY, bool placed): Obj(w, f, 7, IID_GOLD, startX, startY, right, 1.0, 2, placed) {
    m_placed = placed;
    setHp(100);
}

bool Gold::doSomething() {
    if (!isAlive()) {
        return false;
    }
    if (!isVisible() && !m_placed) {
        if (getWorld()->coll(getX(), getY(), getFrack()->getX(), getFrack()->getY(), 4.0)) {
            setVisible(true);
            return false;
        }
    }
    if (isVisible() && !m_placed) {
        if (getWorld()->coll(getX(), getY(), getFrack()->getX(), getFrack()->getY(), 3.0)) {
            setHp(0);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getFrack()->addGold();
            return true;
        }
    }
    //IMPLIMENT LATER, protester interaction
    if (m_placed) {
        setHp(getHp()-1);
    }
    return false;
}

//gets squirts
int Frackman::getSquirts() {
    return m_squirts;
}

//adds squirts
void Frackman::addSquirts() {
    m_squirts +=5;
}

//gets gold
int Frackman::getGold() {
    return m_gold;
}

void Frackman::addGold() {
    m_gold++;
}

//gets sonars
int Frackman::getSonar() {
    return m_sonar;
}

//sends out a squirt
void Frackman::squirt() {
    Direction d = getDirection();
    int x = getX();
    int y = getY();
    setCoord(d, x, y, 3);
    //Instructions say within 3.0 of boulder.
    if(!getWorld()->clearDirtBoulder(x, y)) {
        m_squirts--;
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
        return;
    }
    m_squirts--;
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
    getWorld()->squirt(x,y);
}

//Places a gold
void Frackman::placeGold() {
    m_gold--;
    getWorld()->placeGold(getX(), getY());
}

//doSomething of Frackman.
bool Frackman::doSomething() {
    //leaves if its not alive
    if (!isAlive()) {
        return false;
    }
    //removes dirt in vicinity
    if(getWorld()->removeDirt(getX(), getY())) {
        getWorld()->playSound(SOUND_DIG);
    }
    
    Direction d;
    int ch;
    if (getWorld()->getKey(ch) == true) {
        switch(ch)
        {
            case KEY_PRESS_LEFT:
                d = getDirection();
                if (d == left) {
                    if (getX() > 0 && getWorld()->canMove(getX()-1, getY()))
                        moveTo(getX()-1, getY());
                } else {
                    setDirection(left);
                }
                break;
            case KEY_PRESS_RIGHT:
                d = getDirection();
                if (d == right) {
                    if (getX() < 60 && getWorld()->canMove(getX()+1, getY()))
                        moveTo(getX()+1, getY());
                } else {
                    setDirection(right);
                }
                break;
            case KEY_PRESS_UP:
                d = getDirection();
                if (d == up) {
                    if (getY() < 60 && getWorld()->canMove(getX(), getY()+1))
                        moveTo(getX(), getY()+1);
                } else {
                    setDirection(up);
                }
                break;
            case KEY_PRESS_DOWN:
                d = getDirection();
                if (d == down) {
                    if (getY() > 0 && getWorld()->canMove(getX(), getY()-1))
                        moveTo(getX(), getY()-1);
                } else {
                    setDirection(down);
                }
                break;
            case KEY_PRESS_SPACE:
                if (m_squirts <= 0) {
                    return false;
                }
                squirt();
                return true;
                break;
            case KEY_PRESS_TAB:
                if (m_gold <= 0) {
                    return false;
                }
                placeGold();
                return true;
                break;
        }
    }
    return false;
}
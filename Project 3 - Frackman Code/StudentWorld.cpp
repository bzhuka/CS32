#include "StudentWorld.h"
#include "Actor.h"
#include <queue>
#include <string>
#include <cmath>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

//In this destructor I delete every dynamically allocated object including my two dimensional array of dirt, my vector of objs, and my frackman.  This is in StudentWorld because it needs a destructor.  It is a virtual function because all destructors should be virtual when inheritance is involved.
StudentWorld::~StudentWorld() {
    //Deletes dirt
    for (int i = 0 ; i < 64; i++) {
        for (int j = 0; j < 60; j++) {
            if (dirt[i][j] != nullptr) {
                delete dirt[i][j];
                dirt[i][j] = nullptr;
            }
        }
    }
    
    //Deletes actors
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while(it != objs.end()) {
        delete *it;
        it = objs.erase(it);
    }
    
    //removes frackman
    delete mc;
}

//This method is called every time my FrackMan dies and has to restart the level and every time he collects all the barrels and he can advance to the next level.  In it I initialize a lot of private members including integers, Frackman, and all sorts of Obj.  It figures out how many boulders, gold, and barrels of oil should be on the map, as well as all the different counters for protesters.  It randomly puts what it needs to put in into the map calling a method canPlace to make sure the place is fine.  It also sets the display for the first time using setDisplay.  It returns GWSTATUS_CONTINUE_GAME.  This is virtual because GameWorld (what this inherits from) has the function too as a pure abstract function, making it an abstract class.  Therefore StudentWorld has to implement it, and virtual is to make my code easier to follow.
int StudentWorld::init() {
    m_ticks = 0;
    
    //dynamically allocates a frackman
    mc = new Frackman(this);
    
    //dynamically allocates all the dirt, and pushes onto a vector
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 60; j++) {
            if((i < 30 || i > 33) || j < 4) {
                Dirt *a = new Dirt(i, j);
                dirt[i][j] = a;
            } else {
                dirt[i][j] = nullptr;
            }
        }
    }
    
    //figures out how many boulders, gold, and oil we need.
    int boulders = min(getLevel() / 2 + 2, 6);
    int gold = max(5-getLevel() / 2, 2);
    int oil = min(2 + getLevel(), 20);
    m_maxProtesters =  min(15, (int)(2 + getLevel() * 1.5));
    m_currProtesters = 0;
    m_waitProtesters =  max(0, 3 - getLevel()/4);
    m_tickProtesters = 0;
    m_proxHardcore = 16 + getLevel() * 2;
    
    int randi = (rand() % 61);
    int randj = (rand() % 37)+20;
    //puts boulders in
    for (int i = 0; i < boulders; i++) {
        while(!canPlace(randi, randj)) {
            randi = (rand() % 61);
            randj = (rand() % 37)+20;
        }
        Boulder *b = new Boulder(this, randi, randj);
        objs.push_back(b);
        removeDirt(randi, randj);
    }
    
    m_barrels = oil;
    randi = (rand() % 61);
    randj = (rand() % 37)+20;
    //puts barrels in
    for (int i = 0; i < oil; i++) {
        while(!canPlace(randi, randj)) {
            randi = (rand() % 61);
            randj = (rand() % 37)+20;
        }
        //cout << randi << ", " << randj << endl;
        Barrel *b = new Barrel(this, randi, randj);
        objs.push_back(b);
    }
    randi = (rand() % 61);
    randj = (rand() % 37)+20;
    //puts gold in
    for (int i = 0; i < gold; i++) {
        while(!canPlace(randi, randj)) {
            randi = (rand() % 61);
            randj = (rand() % 37)+20;
        }
        Gold *b = new Gold(this, randi, randj, false);
        objs.push_back(b);
    }
    
    //Sets display for the first time
    setDisplay();
    return GWSTATUS_CONTINUE_GAME;
}

//This method is just to help me determine some of the values that are a minimum between two values.  It takes the two integers given as parameters and returns the smaller one.  If they are equal it’ll return either one of them.  I have this as public rather than private because some functions in my Actor.cpp file use it as well.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
int StudentWorld::min(int i, int j) const{
    if (i < j) {
        return i;
    }
    return j;
}

//This method is just to help me determine some of the values that are a maximum between two values.  It takes the two integers given as parameters and returns the bigger one.  If they are equal it’ll return either one of them.  I have this as public rather than private because some functions in my Actor.cpp file use it as well.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
int StudentWorld::max(int i, int j) const{
    if (i > j) {
        return i;
    }
    return j;
}

//Private function that see's if its within 6.0 radius of anything else.  Also can't be in center.  Returns true or false accordingly.
bool StudentWorld::canPlace(int x, int y) const{
    //Can't be in center???
    if (x > 26 && x <= 33) {
        return false;
    }
    if (y >= 60) {
        return false;
    }
    
    vector<Obj*> :: const_iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if (coll((*it)->getX(), (*it)->getY(), x, y, 6.0)) {
            return false;
        } else {
            it++;
        }
    }
    return true;
}

//Private method that returns true if there's no dirt inA 4X4 box to the upper right 4x4 box of it
bool StudentWorld::noDirt(int x, int y) const{
    if (x > 60 || y > 56) {
        return false;
    }
    
    for (int i = x; i < x+4; i++) {
        for (int j = y; j < y+4; j++) {
            if(dirt[i][j] != nullptr) {
                return false;
            }
        }
    }
    return true;
}

//This method determines if there is any dirt below the four pixels of the given (x,y) coordinate.   It will return true if it is, false otherwise.  If given invalid coordinates it will return false as well.  This is a public method in StudentWorld because Boulders can use this while falling to see if they can continue to fall.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::onDirt(int x, int y) const{
    //should never happen
    if (x > 60 || y <= 0) {
        return false;
    }
    for (int i = x; i < x+4; i++) {
        if (dirt[i][y-1] != nullptr) {
            return true;
        }
    }
    return false;
}

//This method determines if there is any boulder pixels below the four pixels of the given (x,y) coordinate.  It will return true if it is, false otherwise.  This is a public method in StudentWorld because Boulders can use this while falling to see if they can continue to fall.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::onBoulder(int x, int y) const{
    vector<Obj*> :: const_iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->canPickUp() && x - (*it)->getX() >= -3 && x - (*it)->getX() <= 3 && y - (*it)->getY() == 4) {
            return true;
        } else {
            it++;
        }
    }
    return false;
}

//This method determines if there is any boulder within 3.0 of the given (x,y) coordinate or dirt four pixels in a 4x4 above and to the right of the given (x,y) coordinate.  It will return true if it is clear of them, false otherwise.  This is a public method in StudentWorld because Squirts and Protesters can use to see if they can move to a given coordinate.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::clearDirtBoulder(int x, int y) const{
    //To make sure I don't access something out of bounds
    if (x > 60 || x < 0) {
        return false;
    }
    if (y > 60 || y < 0) {
        return false;
    }
    
    //checks for boulder
    vector<Obj*> :: const_iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if (!(*it)->canPassThrough()) {
            if (coll(x, y, (*it)->getX(), (*it)->getY(), 3.0)) {
                return false;
            }
        }
        it++;
    }
    //dirt
    for (int i = x; i < x+4; i++) {
        for (int j = y; j < y+4 && j < 60; j++) {
            if (dirt[i][j] != nullptr) {
                return false;
            }
        }
    }
    return true;
}

//This method removes dirt in a 4x4 box above and to the right of the given (x,y) coordinate.  It will return true if any dirt was removed, false otherwise.  When given invalid coordinates it will also return false.  This is a public method in StudentWorld because Frackman uses it to remove the dirt where it moves and to determine if it needs to play a sound or not.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::removeDirt(int x, int y) {
    //should never happen
    if (x > 60 || x < 0 || y <0 || y >= 60) {
        return false;
    }
    bool sound = false;
    //Removes dirt in vicinity
    for (int i = x; i < x + 4 && i < 64; i++) {
        for (int j = y; j < y + 4 && j < 60; j++) {
            if (dirt[i][j] != nullptr) {
                delete dirt[i][j];
                dirt[i][j] = nullptr;
                sound = true;
            }
        }
    }
    return sound;
}

//This method just decrements a counter of barrels and increases the score of the game.  This is a public method in StudentWorld because Barrels use it to make sure the StudentWorld is aware that a barrel was picked up.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
void StudentWorld::removeBarrel() {
    m_barrels--;
    increaseScore(1000);
}

//This method just determines whether a given (x,y) coordinate is within a given Euclidian distance of radius r of a Frackman.  It returns true if it is, false if it isn’t.  This is a public method in StudentWorld because every Obj that is picked up uses it to make sure the StudentWorld is aware that a it was picked up.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::collFrack(int x, int y, double r) const{
    return coll(x, y, mc->getX(), mc->getY(), r);
}

//Private function.  When given two sets of coordinates, returns true if the two are within given Euclidian distance of radius r.
bool StudentWorld::coll(int x, int y, int i, int j, double r) const{
    return pow((pow((double)(x-i),2.0)) + (pow((double)(y-j),2.0)), 0.5) <= r;
}

//This method annoys protesters for a given hp within a Euclidian distance r of the given (x,y) coordinate.  It will return true if any Protesters were annoyed, false otherwise.  It will also increase the score by rpoints if a Protester dies and by hpoints if a Hardcore Protester dies.  This is a public method in StudentWorld because boulders and squirts use it to annoy Protesters at its given coordinates.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::annoyP(int x, int y, double r, int hp, int rpoints, int hpoints){
    bool somethingHappened = false;
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->canAnnoy() && (*it)->getHp() > 0) {
            if (coll(x, y, (*it)->getX(), (*it)->getY(), r)) {
                if (!(*it)->damage(hp)) {
                    m_currProtesters--;
                    if ((*it)->isHardcore()) {
                        increaseScore(hpoints);
                    } else {
                        increaseScore(rpoints);
                    }
                }
                somethingHappened = true;
            }
        }
        it++;
    }
    return somethingHappened;
}

//This method annoys the Frackman if it’s within a Euclidian distance r of the given (x,y) coordinate.  It will return true if the Frackman was annoyed, false otherwise.  This is a public method in StudentWorld because boulders use it to annoy the Frackman at its given coordinates.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::annoyF(int x, int y, int r, int points) const{
    if(coll(x, y, mc->getX(), mc->getY(), r)) {
        mc->setHp(mc->getHp()-points);
        return true;
    }
    return false;
}

//This method bribes protesters within a Euclidian distance 3.0 of the given (x,y) coordinate.  It will return true if any Protesters were bribed, false otherwise.  This is a public method in StudentWorld because gold uses it to bribe Protesters at its given coordinates.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::bribeP(int x, int y) const{
    vector<Obj*> :: const_iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->canAnnoy() && (*it)->getHp() > 0) {
            if (coll(x, y, (*it)->getX(), (*it)->getY(), 3.0)) {
                (*it)->bribe();
                return true;
            }
        }
        it++;
    }
    return false;
}

//This method checks to see if there is a boulder in the way (radius 3.0).  It will return true there isn’t, false otherwise.  This is a public method in StudentWorld because Frackman uses it to see if it can move.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::canMove(int x, int y) const{
    vector<Obj*> :: const_iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if (!(*it)->canPassThrough() && coll((*it)->getX(), (*it)->getY(), x, y, 3.0)) {
            return false;
        }
        it++;
    }
    return true;
}

//This method checks to see if there if there is a Frackman within a 4.0 radius and in the given direction of the (x,y) coordinate.  It will return true there isn’t, false otherwise.  This is a public method in StudentWorld because Protester uses it to see if it can shout at the Frackman.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
bool StudentWorld::canShout(int x, int y, Actor::Direction d) const{
    if (d == Actor::up) {
        if (y<mc->getY() && coll(x, y, mc->getX(), mc->getY(), 4.0)) {
            return true;
        }
        return false;
    }
    if (d == Actor::down) {
        if (y>mc->getY() && coll(x, y, mc->getX(), mc->getY(), 4.0)) {
            return true;
        }
        return false;
    }
    if (d == Actor::left) {
        if (x>mc->getX() && coll(x, y, mc->getX(), mc->getY(), 4.0)) {
            return true;
        }
        return false;
    }
    if (d == Actor::right) {
        if (x<mc->getX() && coll(x, y, mc->getX(), mc->getY(), 4.0)) {
            return true;
        }
        return false;
    }
    return false;
}

//This method checks to see if there if there is a Frackman within eye distance of a given Frackman.  The integer r is a limit, so if it is 0, it can be 0 blocks away and if it's 4, it has to be at least 4 blocks away.  It returns an int representing the direction.  It returns a -1 for can’t see the Frackman, 0 for on Frackman, 1 for up, 2 for right, 3 for down, and 4 for left.  This is a public method in StudentWorld because Protester uses it to see if the Frackman is in a straight line from it.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
int StudentWorld::straightFrack(int x, int y, int r) const{
    if (mc->getX() == x && mc->getY() == y) {
        return 0;
    }
    if (mc->getX() == x) {
        if (y - mc->getY() < -r) {
            for (int j = y; j < mc->getY()-r; j++) {
                if (!clearDirtBoulder(x, j)) {
                    return -1;
                }
            }
            return 1;
        } else if (y - mc->getY() > r) {
            for (int j = y; j > mc->getY()+r; j--) {
                if(!clearDirtBoulder(x, j)) {
                    return -1;
                }
            }
            return 3;
        }
    } else if (mc->getY() == y) {
        if (x - mc->getX() < -r) {
            for (int i = x; i < mc->getX()-r; i++) {
                if (!clearDirtBoulder(i, y)) {
                    return -1;
                }
            }
            return 2;
        } else if (x - mc->getX() > r) {
            for (int i = x; i > mc->getX()+r; i--) {
                if (!clearDirtBoulder(i, y)) {
                    return -1;
                }
            }
            return 4;
        }
    }
    return -1;
}

//This method checks to see if there if it is near a Frackman (given manhattan distance).  It also modifies the given x and y integers to a coordinate that’s 1 closer to Frackman.  It returns a 0 for not near Frackman, 1 for below, 2 for to the left, 3 for above, and 4 for to the right.  This is a public method in StudentWorld because Hardcore Protesters use it to go towards the Protester if they are within the given manhattan distance.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
int StudentWorld::isNearFrack(int &x, int &y) const{
    if (mapToFrack[x][y] == 1) {
        y++;
        return 1;
    } else if (mapToFrack[x][y] == 2) {
        x++;
        return 2;
    } else if (mapToFrack[x][y] == 3) {
        y--;
        return 3;
    } else if (mapToFrack[x][y] == 4) {
        x--;
        return 4;
    }
    return 0;
}

//This method helps a fully annoyed Protester find his way to the exit (60,60).  It also modifies the given x and y integers to the best coordinate that’s 1 closer to the exit using a queue-based maze searching algorithm.  It returns a 0 if it can’t move, 1 to move up, 2 to move right, 3 to move down, and 4 to move left.  This is a public method in StudentWorld because Hardcore Protesters and Protesters use it to go towards the exit if they are annoyed.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
int StudentWorld::annoyedProtester(int &x, int &y) const{
    if (map[x][y] == 1) {
        y++;
        return 1;
    } else if (map[x][y] == 2) {
        x++;
        return 2;
    } else if (map[x][y] == 3) {
        y--;
        return 3;
    } else if (map[x][y] == 4) {
        x--;
        return 4;
    }
    return 0;
}

//This method creates a new squirt object with the given x and y coordinate and the Frackman’s direction.  It then puts the newly created squirt object into the vector of objs.  This is a public method in StudentWorld because the Frackman calls it to create a new Squirt in the correct location in StudentWorld.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
void StudentWorld::squirt(int x, int y) {
    //location and everything should already be valid by the time its called
    Squirt *s = new Squirt(this, x, y, mc->getDirection());
    objs.push_back(s);
}

//This method can be called by any of the Objects inside Studentworld to add a squirt to Frackman.  It calls Frackman’s method to do all the internal changes that need to be made.  This is void because it doesn’t need to return anything.  Every time this method is called by any Object, 5 squirts will be added to Frackman’s inventory.  This is in StudentWorld to let other objects interact with the Frackman.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
void StudentWorld::addSquirts() const{
    mc->addSquirts();
}

//This method can be called by any of the Objects inside Studentworld to add a gold to Frackman.  It calls Frackman’s method to do all the internal changes that need to be made.  This is void because it doesn’t need to return anything.  Every time this method is called by any Object, 1 gold will be added to Frackman’s inventory.  This is in StudentWorld to let other objects interact with the Frackman.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
void StudentWorld::addGold() const{
    mc->addGold();
}

//This method can be called by any of the Objects inside Studentworld to add a sonar to Frackman.  It calls Frackman’s method to do all the internal changes that need to be made.  This is void because it doesn’t need to return anything.  Every time this method is called by any Object, 1 sonar will be added to Frackman’s inventory.  This is in StudentWorld to let other objects interact with the Frackman.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
void StudentWorld::addSonar() const{
    mc->addSonar();
}

//This method creates a new gold object with the given x and y coordinate and makes the value of places true.  It then puts the newly created gold object into the vector of objs.  This is a public method in StudentWorld because the Frackman calls it to create a new Gold in the correct location in StudentWorld.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
void StudentWorld::placeGold(int x, int y) {
    Gold *g = new Gold(this, x, y, true);
    objs.push_back(g);
}

//This method reveals all nearby hidden objects in a Euclidian distance of 12.0 from the given (x,y) coordinate.  This is a public method in StudentWorld because Sonars use it to interact with other Objects and change their visibility within the StudentWorld.  It is not virtual because nothing will inherit from StudentWorld and there is no need to make it virtual because it will never change.
void StudentWorld::reveal(int x, int y) {
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->canPickUp()) {
            if (coll((*it)->getX(), (*it)->getY(), x, y, 12.0)) {
                (*it)->setVis(true);
            }
        }
        it++;
    }
}

//just creates a protester
void StudentWorld::createProtester() {
    int probHard = min(90, getLevel() * 10 + 30);
    int temp = rand() % 100 + 1;
    if (temp > probHard) {
        Protester *p = new Protester(this, IID_PROTESTER, m_waitProtesters);
        objs.push_back(p);
    } else {
        HardcoreProtester *p = new HardcoreProtester(this, m_waitProtesters);
        objs.push_back(p);
    }
    
}

//updates the map
void StudentWorld::updateMap(int x, int y, int m[64][64], int r) {
    //fills map with -1's that represent areas we can't go to
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            m[i][j] = 0;
        }
    }
    
    queue<Coord> cstack;
    cstack.push(Coord(x, y, r));
    while (!cstack.empty()) {
        Coord a = cstack.front();
        cstack.pop();
        if(m[a.r()-1][a.c()] == 0 && clearDirtBoulder(a.r()-1, a.c()) && a.z() > 0) {
            cstack.push(Coord(a.r()-1, a.c(), a.z() -1));
            m[a.r()-1][a.c()] = 2;
        }
        if(m[a.r()][a.c()-1] == 0 && clearDirtBoulder(a.r(), a.c()-1) && a.z() > 0) {
            cstack.push(Coord(a.r(), a.c()-1, a.z() -1));
            m[a.r()][a.c()-1] = 1;
        }
        if(m[a.r()+1][a.c()] == 0 && clearDirtBoulder(a.r()+1, a.c()) && a.z() > 0) {
            cstack.push(Coord(a.r()+1, a.c(), a.z() -1));
            m[a.r()+1][a.c()] = 4;
        }
        if(m[a.r()][a.c()+1] == 0 && clearDirtBoulder(a.r(), a.c()+1) && a.z() > 0) {
            cstack.push(Coord(a.r(), a.c()+1, a.z() -1));
            m[a.r()][a.c()+1] = 3;
        }
    }
}

//does a move
int StudentWorld::move() {
    //First increments all my counters.
    m_ticks++;
    m_tickProtesters--;
    
    //Builds the maps for annoyed Protesters and for Hardcore Protesters who are near the Frackman.
    updateMap(60, 60, map, INT16_MAX);
    updateMap(mc->getX(), mc->getY(), mapToFrack, m_proxHardcore);
    
    //Creates Protester if able to.  I’m doing this before asking every Object to do something to have Protesters work on the tick they are created.
    if(m_tickProtesters <= 0 && m_currProtesters != m_maxProtesters) {
        createProtester();
        m_tickProtesters = max(25, 200 - getLevel());
        m_currProtesters++;
    }
    
    // Asks all of the actors that are currently active in the game world to do something
    bool update = false;
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while ( it != objs.end()) {
        if ((*it)->doSomething()) {
            update = true;
        }
        if (!mc->isAlive()) {
            playSound(SOUND_PLAYER_GIVE_UP);
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        if (m_barrels <= 0) {
            playSound(SOUND_FINISHED_LEVEL);
            return GWSTATUS_FINISHED_LEVEL;
        }
        it++;
    }
    
    //Asks Frackman to do something
    if (mc->doSomething()) {
        update = true;
    }
    
    //Deletes any actors that need to be removed from the game during this tick and removes them from my STL container that tracks them.
    it = objs.begin();
    while (it != objs.end()) {
        if (!(*it)->isAlive()) {
            delete (*it);
            it = objs.erase(it);
        } else {
            it++;
        }
    }
    
    //If Frackman is dead play sound and return a value
    if (!mc->isAlive()) {
        playSound(SOUND_PLAYER_GIVE_UP);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    //If Frackman won the level play sound and return so
    if (m_barrels <= 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    //If something of note happened update the display
    if (update) {
        setDisplay();
    }
    
    //Put random objects into the game if able to and at random locations.
    int randi = (rand() % (getLevel() * 25 + 300));
    if (randi == 0) {
        int randj = (rand() % 5);
        if (randj == 0) {
            int shp = max(100, 300-getLevel());
            Sonar *s = new Sonar(this, 0, 60, shp);
            objs.push_back(s);
        } else {
            vector<int> x1;
            vector<int> y1;
            for (int i = 0; i <= 60; i++) {
                for (int j = 0; j <= 56; j++) {
                    if (noDirt(i, j)) {
                        x1.push_back(i);
                        y1.push_back(j);
                    }
                }
            }
            int rando = (rand() % x1.size());
            Water *w= new Water(this, x1.at(rando), y1.at(rando));
            objs.push_back(w);
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

//In this method, I delete every dynamically allocated object including my two dimensional array of dirt, my vector of objs, and my frackman.  This is in StudentWorld because it needs to be able to clean up all dynamically allocated objects between levels, or the memory used will just keep building up.  It is a virtual function because all destructors should be virtual when inheritance is involved.  This is virtual because GameWorld (what this inherits from) has the function too as a pure abstract function, making it an abstract class.  Therefore StudentWorld has to implement it, and virtual is to make my code easier to follow.
void StudentWorld::cleanUp() {
    //Deletes dirt
    for (int i = 0 ; i < 64; i++) {
        for (int j = 0; j < 60; j++) {
            if (dirt[i][j] != nullptr) {
                delete dirt[i][j];
                dirt[i][j] = nullptr;
            }
        }
    }
    
    //Deletes actors
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while( it != objs.end()) {
        delete *it;
        it = objs.erase(it);
    }
    
    //removes frackman
    delete mc;
}

//Sets the display above the game
void StudentWorld::setDisplay() {
    int score = getScore();
    int level = getLevel();
    int lives = getLives();
    int health = mc->getHp();
    int squirts = mc->getSquirts();
    int gold = mc->getGold();
    int sonar = mc->getSonar();
    int barrelsLeft = m_barrels;
    string s = format(score, level, lives,health, squirts, gold, sonar, barrelsLeft);
    setGameStatText(s);
}

//formats the given values a string with the given format
string StudentWorld::format(int sc, int lv, int li, int hp, int squ, int g, int son, int bar) {
    string s = "Scr: ";
    stringstream ssc;
    //Specs say 6, example shows 6.  Also 6 fits better
    ssc << setw(6) << setfill('0') << sc;
    stringstream slv;
    slv << setw(2) << setfill(' ') << lv;
    stringstream shp;
    shp << setw(2) << setfill(' ') << hp;
    stringstream ssqu;
    ssqu << setw(2) << setfill(' ') << squ;
    stringstream sg;
    sg << setw(2) << setfill(' ') << g;
    stringstream sson;
    sson << setw(2) << setfill(' ') << son;
    stringstream sbar;
    sbar << setw(2) << setfill(' ') << bar;
    s.append(ssc.str()).append("  Lvl: ").append(slv.str()).append("  Lives: ").append(to_string(li)).append("  Hlth: ").append(shp.str()).append("0%  Wtr: ").append(ssqu.str()).append("  Gld: ").append(sg.str()).append("  Sonar: ").append(sson.str()).append("  Oil Left: ").append(sbar.str());
    return s;
}
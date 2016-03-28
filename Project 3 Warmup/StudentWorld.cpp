#include "StudentWorld.h"
#include "Actor.h"
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

//deconstructor of StudentWorld
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
    //Deletes frackman
    delete mc;
}

//returns min.  If equal, returns either
int StudentWorld::min(int i, int j) {
    if (i < j) {
        return i;
    }
    return j;
}

//returns max.  If equal, returns either
int StudentWorld::max(int i, int j) {
    if (i > j) {
        return i;
    }
    return j;
}

//See's if its within 6.0 radius of anything else.  Also can't be in center
bool StudentWorld::canPlace(int x, int y) {
    //Can't be in center???
    if (x > 26 && x <= 33) {
        return false;
    }
    if (y >= 60) {
        return false;
    }
    
    vector<Obj*> :: iterator it;
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

//returns true if there's no dirt inA 4X4 box to the upper right 4x4 box of it
bool StudentWorld::noDirt(int x, int y) {
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

//initializes studentworld
int StudentWorld::init() {
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
    
    int randi = (rand() % 61);
    int randj = (rand() % 37)+20;
    
    //puts boulders in
    for (int i = 0; i < boulders; i++) {
        while(!canPlace(randi, randj)) {
            randi = (rand() % 61);
            randj = (rand() % 37)+20;
        }
        Boulder *b = new Boulder(this, mc, randi, randj);
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
        cout << randi << ", " << randj << endl;
        Barrel *b = new Barrel(this, mc, randi, randj);
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
        Gold *b = new Gold(this, mc, randi, randj, false);
        objs.push_back(b);
    }
    
    //Sets display for the first time
    setDisplay();
    
    return GWSTATUS_CONTINUE_GAME;
}

//checks to see if there is dirt beneath
bool StudentWorld::onDirt(int x, int y) {
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

//checks to see if its on top of a boulder
bool StudentWorld::onBoulder(int x, int y) {
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->getID() == 4 && x - (*it)->getX() >= -3 && x - (*it)->getX() <= 3 && y - (*it)->getY() == 4) {
            return true;
        } else {
            it++;
        }
    }
    return false;
}

//returns true if there's no dirt in the 4x4 box or boulder there and that it's in bounds
bool StudentWorld::clearDirtBoulder(int x, int y) {
    //To make sure I don't access something out of bounds
    //not sure why its 61 rather than 60
    if (x > 60 || x < 0) {
        return false;
    }
    
    //checks for boulder
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->getID() == 4) {
            if (coll(x, y, (*it)->getX(), (*it)->getY(), 3.0)) {
                return false;
            }
        }
        it++;
    }
    
    if (y > 60 || y < 0) {
        return false;
    }
    for (int i = x; i < x+4; i++) {
        for (int j = y; j < y+4 && j < 60; j++) {
            if (dirt[x][y] != nullptr) {
                return false;
            }
        }
    }
    return true;
}

//removes necessary dirt
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

//removes one barrel
void StudentWorld::removeBarrel() {
    m_barrels--;
    increaseScore(1000);
    if(m_barrels == 0) {
        advanceToNextLevel();
    }
}

//given two sets of coordinates, returns true if the two are within given radius
bool StudentWorld::coll(int x, int y, int i, int j, double r) {
    return pow((pow((double)(x-i),2.0)) + (pow((double)(y-j),2.0)), 0.5) <= r;
}


//hits protestor, given position, radius, and annoyance points
//FIX LATER
bool StudentWorld::hitP(int x, int y, int r, int points) {
/*    vector<Obj*> :: iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->getId() == 1 || (*it)->getId() == 2) {
            if (coll(x, y, (*it)->getX(), (*it)->getY(), r)) {
                Annoyable *o = dynamic_cast<Annoyable *>((*it));
                o->annoy(points);
            }
        } else {
            it++;
        }
    }*/
    return false;
}

//hits frackman, given position, radius, and annoyance points
bool StudentWorld::hitF(int x, int y, int r, int points) {
    if(coll(x, y, mc->getX(), mc->getY(), r)) {
        mc->annoy(points);
        return true;
    }
    return false;
}

//returns true if there isn't a boulder in the way
bool StudentWorld::canMove(int x, int y) {
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while (it != objs.end()) {
        if ((*it)->getId() == 4 && coll((*it)->getX(), (*it)->getY(), x, y, 3.0)) {
            return false;
        }
        it++;
    }
    return true;
}

//creates a squirt
void StudentWorld::squirt(int x, int y) {
    //location and everything should already be valid by the time its called
    Squirt *s = new Squirt(this, mc, x, y);
    objs.push_back(s);
}

//places a gold
void StudentWorld::placeGold(int x, int y) {
    Gold *g = new Gold(this, mc, x, y, true);
    objs.push_back(g);
}

//does a move
int StudentWorld::move() {
    //removes dead stuff in vector objs
    //The reason I have this before the doSomethings is because I believe the example game we were gave had this before the doSomethings rather than after.  If it was after, the objects that died in the tick will disappear, making objects that last for one tick never appear.
    
    //makes everything doSomething
    bool update = false;
    
    if (mc->doSomething()) {
        update = true;
    }
    
    vector<Obj*> :: iterator it;
    it = objs.begin();
    while ( it != objs.end()) {
        if ((*it)->doSomething()) {
            update = true;
        }
        if (!mc->isAlive()) {
            return GWSTATUS_PLAYER_DIED;
        }
        it++;
    }
    
    it = objs.begin();
    while (it != objs.end()) {
        if (!(*it)->isAlive()) {
            delete (*it);
            it = objs.erase(it);
        } else {
            it++;
        }
    }
    
    if (!mc->isAlive()) {
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (m_barrels <= 0) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if (update) {
        setDisplay();
    }
    
    //adds random objects
    int randi = (rand() % (getLevel() * 25 + 300));
    if (randi == 0) {
        int randj = (rand() % 5);
        if (randj == 0) {
            
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
            Water *w= new Water(this, mc, x1.at(rando), y1.at(rando));
            objs.push_back(w);
        }
    }
    
		  // This code is here merely to allow the game to build, run, and terminate after you hit enter a few times.
		  // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    return GWSTATUS_CONTINUE_GAME;
}

//cleans up
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

//Sets the display above
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

//formats into a string
string StudentWorld::format(int sc, int lv, int li, int hp, int squ, int g, int son, int bar) {
    string s = "Scr: ";
    stringstream ssc;
    //Specs say 8, example shows 6.  Also 6 fits better
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
    s.append(ssc.str()).append("  Lvl: ").append(slv.str()).append("  Lives: ").append(to_string(li)).append("  Hlth: ").append(shp.str()).append("0%  Water: ").append(ssqu.str()).append("  Gld: ").append(sg.str()).append("  Sonar: ").append(sson.str()).append("  Oil Left: ").append(sbar.str());
    return s;
}
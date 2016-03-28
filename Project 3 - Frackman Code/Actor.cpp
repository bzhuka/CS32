#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//      ____ ____ ___ ____ ____
//      |__| |     |  |  | |__/
//      |  | |___  |  |__| |  \
//

//This is the constructor for my Actor class.  I make it need all the parameters that GraphObject needs, and I pass it right down to GraphObject’s constructor in the initializer list.  The one extra method is a boolean: vis.  It determines whether the object starts off visible or not.  I created it with barrels and gold nuggets in mind.  It’s default value is true because the vast majority of objects start out visible.  I set the object’s visibility to match the bool.
Actor::Actor(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, bool vis): GraphObject(imageID, startX, startY, dir, size, depth){
    setVisible(vis);
}

//This is the destructor for my Actor class.  I don’t do anything inside it because I don’t have any dynamically added objects I need to delete.  In addition, there isn’t any functionality an actor should do when it is deleted (such as a print statement).  This is in Actor because Actor needs a destructor.  It is a virtual function because all destructors should be virtual when inheritance is involved.
Actor::~Actor() {
}

//This method takes an int and returns a direction.  If given a 0, it will return none, 1: up, 2: right, 3:down, 4:left.  It should only ever be given a number in the range [0,5], and that’s a given.  It’s a public function in actor because it is just a helper function that I want to be in every subclass.  It helps with writing less code, writing it this once rather than every time I need to interpret an integer into a direction.  Throughout my entire program I will use the same int to Direction mapping.
Actor::Direction Actor::direc(int x) {
    if (x == 0) {
        return none;
    } else if (x == 1) {
        return up;
    } else if (x == 2) {
        return right;
    } else if (x == 3) {
        return down;
    } else {
        return left;
    }
}

//      ____ ___   _
//      |  | |__]  |
//      |__| |__] _|
//

//This is the constructor for my Obj class.  This is basically my main class (instead of Actor because dirt doesn’t really need any of this and it doesn’t make sense to inherit from dirt).  I make it need all the parameters that Actor needs, and I pass it right down to Actor’s constructor in the initializer list.  The default values are what the majority of Objs require.  Inside I set two private members that every Obj should have: health and a pointer to the world they are in.  Every Object has health, because every object needs to “die” at some point.  For some later classes rather than have states I use the health to achieve the same result.  The pointer to the StudentWorld is so every inherited subclass will be able to call upon the methods of StudentWorld because only the StudentWorld and not the Obj should know the information about other Objects.  Obviously I created this in Obj because it needs a constructor.
Obj::Obj(StudentWorld* w, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, bool vis): Actor(imageID, startX, startY, dir, size, depth, vis) {
    m_w = w;
    m_Hp = 0;
}

//Deconstructor of Object.
Obj::~Obj() {
}

//This method returns the amount of Hp the Object has.  It is const because it doesn’t change any private members, only returning one of them.  I need to have this for my inherited subclasses to access it.  This is not virtual because I should never need to redefine it.  This is in Obj because once again, every class I will use will use Hp in some way.
int Obj::getHp() const{
    return m_Hp;
}

//This method takes in an integer and sets the Hp to that integer.  Because derived subclasses cannot directly access an inherited class’s private members, I need getters and setters for the private members, in this case only m_hp.  This is void because it never needs to return anything, and it should never really have errors.  This is not virtual because I should never have to redefine it.
void Obj::setHp(int hp) {
    m_Hp = hp;
}


//This method returns a boolean concerning whether this object can be passed through.  This is const because it doesn’t change any variables.  The default is to return true, because most Obj’s can be passed through.  When an object (boulders) can’t, then I’ll redefine it, which is why I made it virtual.
bool Obj::canPassThrough() const{
    return true;
}

//This method returns a boolean concerning whether this object can be picked up.  This is const because it doesn’t change any variables.  The default is to return false, because most Obj’s cannot be picked up.  When an object (water or gold or barrels) can, then I’ll redefine it, which is why I made it virtual.
bool Obj::canPickUp() const {
    return false;
}

//This method returns a boolean concerning whether this object can be annoyed.  This is const because it doesn’t change any variables.  The default is to return false, because most Obj’s cannot be annoyed.  When an object (Frackman or Protesters) can, then I’ll redefine it, which is why I made it virtual.
bool Obj::canAnnoy() const {
    return false;
}

//This method returns a boolean concerning whether this is “hardcore”.  This is const because it doesn’t change any variables.  The default is to return false, because most Obj’s are not “hardcore”.  When an object (Hardcore Protesters) is, then I’ll redefine it, which is why I made it virtual.
bool Obj::isHardcore() const {
    return false;
}


//This method sets the object’s visibility to the boolean passed in.  This is void because it doesn’t need to return anything and should never run into errors.  This is in my Obj class because many derived subclasses will need to use this such as gold or barrels.  This is not virtual because it should never have to be redefined.
void Obj::setVis(bool isvis) {
    setVisible(isvis);
}

//This method returns a boolean depending on whether the Obj is still alive or not.  It accesses m_hp, because all dead Objs will have an hp of 0 or less, but alive ones will not.  I do this rather than use a private boolean.  This is in Obj because many subclasses will need to use this, and also StudentWorld will need to call this to make decisions regarding certain objects inside it.  This is virtual because some classes (Protesters) aren’t truly dead even when they have no hit points, and I’ll redefine it for them.
bool Obj::isAlive() const{
    if (m_Hp > 0) {
        return true;
    }
    return false;
}

//This method takes two references to two ints, a direction, and a distance.  I then sets the (x,y) coordinate to the correct one the distance away from given one, in that direction.  This is to keep me from having to copy paste the same things in later derived subclasses.  It doesn’t do anything if d is none, and doesn’t do any edge checking because other functions will do that.  It assumes it will be passed in valid parameters.  This is in object because many derived classes will need to update move according to directions, and this helps process where it needs to move to.  This is not virtual because it never needs to be changed.
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

//This method returns a pointer to the StudentWorld that the Obj will be in.  This is in Obj because every Obj contains a pointer to the StudentWorld it is created in.  Then it can call methods in the StudentWorld that will help it perform actions, especially ones that involve interactions with other objects.
StudentWorld* Obj::getWorld() const{
    return m_w;
}

//This method damages an Obj and returns true if the Obj is dead.  This is essentially a setHp that returns a boolean.  I can override it for Protester and Frackman because it is virtual and some derived objects do specific things when damaged such as make noises.  This is in Obj because my StudentWorld contains a vector of Objects, and I need to call damage on some of them.
bool Obj::damage(int hp) {
    setHp(getHp() - hp);
    if (getHp() <=0) {
        return true;
    }
    return false;
}

//This method bribes and doesn’t return anything. It is purely here because my StudentWorld contains a vector of Objects, and I need to call bribe on some of them.  It doesn’t do anything and is here to be overridden by derived classes.
void Obj::bribe() {
}

//      ___  ____ ____ ____ ____ _
//      |__] |__| |__/ |__/ |___ |
//      |__] |  | |  \ |  \ |___ |___
//

//This is the constructor for barrel.  All it needs is an x and y coordinate and a pointer to the world it is created in.  Everything else is assumed.  It calls Obj’s constructor with the correct values, and passes in the X and Y coordinates and the pointer to StudentWorld given by the parameters.  All the values that aren’t equal to Actor’s default values are according to the specs.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.
Barrel::Barrel(StudentWorld* w, int startX, int startY): Obj(w, IID_BARREL, startX, startY, right, 1.0, 2, false) {
    setHp(1);
    isvis = false;
}

//Destructor of barrel
Barrel::~Barrel() {
}

//I override canPickUp to return true because Barrels can be picked up.  This is for identification purposes in StudentWorld.  It is const because it doesn’t change private variables.  It is virtual because as before stated, it overrides an existing definition in Obj.
bool Barrel::canPickUp() const {
    return true;
}

//Like in every doSomething, if it’s not alive, I return false.  Then if it’s not visible, but the Frackman is within a Euclidean distance of 4.0 (calling a method in StudentWorld)from it, I set it to be visible, and return false because the score doesn’t need to be changed.  If it makes it past that, then if it’s within a distance of 3.0 from Frackman, then the Frackman “collects it”.  This entails setting the hp to 0 so it “dies”, playing a sound, removing the barrel from the StudentWorld, and returning true because the score needs to be updated.  If none of the above applied, then return false because nothing needs to be changed.
bool Barrel::doSomething() {
    if (!isAlive()) {
        return false;;
    }
    if ((!isvis) && getWorld()->collFrack(getX(), getY(), 4.0)) {
        isvis = true;
        setVis(true);
        return false;;
    }
    if (getWorld()->collFrack(getX(), getY(), 3.0)) {
        setHp(0);
        getWorld()->playSound(SOUND_FOUND_OIL);
        getWorld()->removeBarrel();
        return true;
    }
    return false;
}

//      ___  ____ _  _ _    ___  ____ ____
//      |__] |  | |  | |    |  \ |___ |__/
//      |__] |__| |__| |___ |__/ |___ |  \
//

//This is the constructor for boulder.  All it needs is an x and y coordinate and a pointer to the world it is created in.  Everything else is assumed.  It calls Obj’s constructor with the correct values, and passes in the X and Y coordinates and the pointer to StudentWorld given by the parameters.  All the values that aren’t equal to Actor’s default values are according to the specs.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.
Boulder::Boulder(StudentWorld* w, int startX, int startY): Obj(w, IID_BOULDER, startX, startY, down, 1.0, 1) {
    setHp(1);
    state = STABLE;
    timer = 30;
}

//Destructor of Boulder
Boulder::~Boulder() {
}

//This returns false because Boulders can not be passed through.  This method is in Boulder because of the above reason (for identification reasons in StudentWorld).  This is virtual to override the canPassThrough in Obj.
bool Boulder::canPassThrough() const {
    return false;
}

//This method is obviously in Boulder because every Boulder needs a doSomething method.  Also, because Obj defined doSomething as a pure abstract method, every derived subclass needs an implementation of it to be instantiated, which Boulders need to be.  It is virtual to override the doSomething in Obj.
bool Boulder::doSomething() {
    //If not alive return false as nothing needs to be updated.
    if(!isAlive()) {
        return false;
    }
    //If the Boulder is currently in the stable state (I use an int), then it must check to see if there is any Dirt in the 4 squares immediately below it (I use a method in StudentWorld). If none of the 4 squares beneath the Boulder have any Dirt, then the Boulder must transition into a waiting state for the next 30 ticks.
    if(state == 0) {
        if(!(getWorld()->onDirt(getX(), getY()))) {
            state = 1;
        }
        return false;
    }
    //If the Boulder is in a waiting state and 30 ticks have elapsed, then it must transition into a falling state and play the sound SOUND_FALLING_ROCK.
    if (state == 1) {
        if (timer == 0) {
            state = 2;
        }
        timer--;
        return false;
    }
    //It must continue to move downward one square during each tick until it either (a) hits the bottom of the oil field , (b) runs into the top of another Boulder, or (c) it runs into Dirt. If so, the Boulder must set its state to dead. If the Boulder comes within a radius of 3 of any Protester(s) or the FrackMan while falling, it must cause 100 points of annoyance to those actors. The Boulder will continue falling down as described above, even if it annoys one or more Protesters on its way down. However, if the Boulder annoys the FrackMan, the player will instantly lose a life.
    if (state == 2) {
        if(getWorld()->onDirt(getX(), getY())) {
            setHp(0);
        } else if(getY() == 0) {
            setHp(0);
        } else if(getWorld()->onBoulder(getX(), getY())) {
            setHp(0);
        } else {
            moveTo(getX(), getY() -1);
        }
        if (getWorld()->annoyP(getX(), getY(), 3, 100, 500, 500) > 0) {
            return true;
        }
        if (getWorld()->annoyF(getX(), getY(), 3, 100)) {
            return true;
        }
    }
    return false;
}

//      _ _ _ ____ ___ ____ ____
//      | | | |__|  |  |___ |__/
//      |_|_| |  |  |  |___ |  \
//

//This is the constructor for Water.  All it needs is an x and y coordinate and a pointer to the world it is created in.  Everything else is assumed.  It calls Obj’s constructor with the correct values, and passes in the X and Y coordinates and the pointer to StudentWorld given by the parameters.  All the values that aren’t equal to Obj’s default values are according to the specs.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.
Water::Water(StudentWorld* w, int startX, int startY): Obj(w, IID_WATER_POOL, startX, startY, right, 1.0, 2) {
    setHp(w->min(100, 300 - 10*w->getLevel()));
}

//Destructor for Water
Water::~Water() {
}

//This method is obviously in Water because every Water needs a doSomething method.  Also, because Obj defined doSomething as a pure abstract method, every derived subclass needs an implementation of it to be instantiated, which Water need to be.  It is virtual to override the doSomething in Obj.
bool Water::doSomething() {
    //Return immediately if not alive.
    if (!isAlive()) {
        return false;
    }
    //Otherwise, if the Water pool is within a radius of 3.0 (<= 3.00 units away) from the FrackMan, then the Water pool will activate.
    //The Water pool must set its state to dead, play the sound effect SOUND_GOT_GOODIE, tell the FrackMan object that it just received 5 new squirts of water so it can update its inventory, and increase the player’s score by 100 points.
    if (getWorld()->collFrack(getX(), getY(), 3.0)) {
        setHp(0);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->addSquirts();
        getWorld()->increaseScore(100);
        return true;
    }
    //Since the Pool of Water is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead
    setHp(getHp()-1);
    return false;
}

//      ____ ____ _  _ _ ____ ___
//      [__  |  | |  | | |__/  |
//      ___] |_\| |__| | |  \  |


//This is the constructor for Squirt.  All it needs is an x and y coordinate, a Direction and a pointer to the world it is created in.  Everything else is assumed.  It calls Obj’s constructor with the correct values, and passes in the X and Y coordinates, the Direction, and the pointer to StudentWorld given by the parameters.  They need to be constructed in the direction that the Frackman is currently facing, so passing in a Direction is the obvious solution.  All the values that aren’t equal to Obj’s default values are according to the specs.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.
Squirt::Squirt(StudentWorld* w, int startX, int startY, Direction d): Obj(w, IID_WATER_SPURT, startX, startY, d, 1.0, 1) {
    setHp(5);
    m_d = d;
}

//Destructor for squirt
Squirt::~Squirt() {
}

//This method is obviously in Squirt because every Squirt needs a doSomething method.  Also, because Obj defined doSomething as a pure abstract method, every derived subclass needs an implementation of it to be instantiated, which Squirts need to be.  It is virtual to override the doSomething in Obj.
bool Squirt::doSomething() {
    //If a Squirt is within a radius of 3.0 of one or more Protesters, it will cause 2 points of annoyance to these Protester(s), and then immediately set its state to dead, so it can be removed from the oil field at the end of the tick.  It will return true because the text at the top should be changed.
    if (getWorld()->annoyP(getX(), getY(), 3.0, 2, 100, 250)) {
        setHp(0);
        return true;
    }
    //If a Squirt has traveled through its full travel distance, then it immediately sets its state to dead, so it can be removed from the oil field at the end of the current tick.  It will return false because nothing of importance happened.
    if(!isAlive()) {
        return false;
    }
    //Otherwise, the Squirt must check to see if it can move one square in its currently facing direction. If this target location is occupied by either Dirt or a Boulder, then the Squirt immediately sets its state to dead so it can be removed from the oil field at the end of the current tick.  It will return false because nothing needs to be updated.
    int x = getX();
    int y = getY();
    setCoord(m_d, x, y, 1);
    if(!getWorld()->clearDirtBoulder(x, y)) {
        setHp(0);
        return false;
    }
    //Otherwise, the Squirt moves one square forward in its currently-facing direction, and then returns false.
    moveTo(x, y);
    setHp(getHp()-1);
    return false;
}

//      ____ ____ _    ___
//      | __ |  | |    |  \
//      |__] |__| |___ |__/
//

//This is the constructor for Gold.  All it needs is an x and y coordinate, a boolean of whether it was placed by the Frackman or not, and a pointer to the world it is created in.  Everything else is assumed.  It calls Obj’s constructor with the correct values, and passes in the X and Y coordinates, the bool visible is determined by the bool placed, and the pointer to StudentWorld given by the parameters.  Gold’s visibility depends on the circumstances of its instantiation, so I use a bool to determine it.  All the values that aren’t equal to Obj’s default values are according to the specs.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.
Gold::Gold(StudentWorld* w, int startX, int startY, bool placed): Obj(w, IID_GOLD, startX, startY, right, 1.0, 2, placed) {
    m_placed = placed;
    setHp(100);
}

//Destructor of Gold
Gold::~Gold() {
}

//I override canPickUp to return true because Barrels can be picked up.  This is for identification purposes in StudentWorld.  It is const because it doesn’t change private variables.  It is virtual because as before stated, it overrides an existing definition in Obj.
bool Gold::canPickUp() const {
    return true;
}

//This method is in Gold because every Gold needs a doSomething method.  Also, because Obj defined doSomething as a pure abstract method, every derived subclass needs an implementation of it to be instantiated, which Gold needs to be.  It is virtual to override the doSomething in Obj.
bool Gold::doSomething() {
    //Returns false immediately if not alive.
    if (!isAlive()) {
        return false;
    }
    //Otherwise, if the Gold Nugget is not currently visible AND the FrackMan is within a radius of 4.0 of it then it must make itself visible with setVisible() and immediately return false.
    if (!isVisible() && !m_placed) {
        if (getWorld()->collFrack(getX(), getY(), 4.0)) {
            setVis(true);
            return false;
        }
    }
    //Otherwise, if the Gold Nugget is pickup-able by the FrackMan and it is within a radius of 3.0 from the FrackMan, then it will activate, and:
    //a. The Gold Nugget must set its state to dead
    //b. The Gold Nugget must play a SOUND_GOT_GOODIE.
    //c. The Gold Nugget increases the player’s score by 10 points.
    //d. The Gold Nugget must tell the FrackMan object that it just received a new Nugget so it can update its inventory.
    //Returns true because top changed.
    if (isVisible() && !m_placed) {
        if (getWorld()->collFrack(getX(), getY(),  3.0)) {
            setHp(0);
            getWorld()->playSound(SOUND_GOT_GOODIE);
            getWorld()->increaseScore(10);
            getWorld()->addGold();
            return true;
        }
    }
    //Otherwise, if the Gold Nugget is pickup-able by Protesters and it is within a radius of 3.0 (<= 3.00 units away) from a Protester, then the Gold Nugget will activate and set its state to dead, play SOUND_PROTESTER_FOUND_GOLD, tell the Protester object that it just received a new Nugget so it can react appropriately (e.g., be bribed), increase the player’s score by 25 points
    if (m_placed) {
        if (getWorld()->bribeP(getX(), getY())) {
            setHp(0);
            return true;
        } else {
            //If the Gold Nugget has a temporary state, then it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead
            setHp(getHp()-1);
        }
    }
    return false;
}

//      ____ ____ _  _ ____ ____
//      [__  |  | |\ | |__| |__/
//      ___] |__| | \| |  | |  \
//

//This is the constructor for Sonar.  All it needs is an x and y coordinate, and integer representing hp, and a pointer to the world it is created in.  Everything else is assumed.  It calls Obj’s constructor with the correct values, and passes in the X and Y coordinates and the pointer to StudentWorld given by the parameters.  All the values that aren’t equal to Obj’s default values are according to the specs.  The hp int is determined by StudentWorld as it depends on the level and is passed in.  The constructor sets the hp equal to this integer, and then reduces it by one every tick.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.
Sonar::Sonar(StudentWorld* w, int startX, int startY, int hp): Obj(w, IID_SONAR, startX, startY, right, 1.0, 2) {
    setHp(hp);
}

//Destructor of Sonar
Sonar::~Sonar() {
}

//This method is in Sonar because every Sonar needs a doSomething method.  Also, because Obj defined doSomething as a pure abstract method, every derived subclass needs an implementation of it to be instantiated, which Sonar needs to be.  It is virtual to override the doSomething in Obj.
bool Sonar::doSomething() {
    //Return false immediately if not alive.
    if (!isAlive()) {
        return false;
    }
    //Otherwise, if the Sonar Kit is within a radius of 3.0 (<= 3.00 units away) from the FrackMan, then the Sonar Kit will activate, and set its state to dead, play SOUND_GOT_GOODIE, tell the FrackMan object that it just received a new Sonar Kit so it can update its inventory, and increases the player’s score by 75 points.
    if (getWorld()->collFrack(getX(), getY(), 3.0)) {
        setHp(0);
        getWorld()->playSound(SOUND_GOT_GOODIE);
        getWorld()->addSonar();
        getWorld()->increaseScore(75);
        return true;
    }
    //3. Since the Sonar Kit is always in a temporary state, it will check to see if its tick lifetime has elapsed, and if so it must set its state to dead.
    setHp(getHp()-1);
    return false;
}

//      ___  ____ ____ ___ ____ ____ ___ ____ ____
//      |__] |__/ |  |  |  |___ [__   |  |___ |__/
//      |    |  \ |__|  |  |___ ___]  |  |___ |  \
//

//This is the constructor for Protester.  It doesn’t need an x and y coordinate because they all start at 60,60 and I can directly pass that in Obj’s constructor.  It needs an integer that represents the wait tick, (It can move one / wait ticks).  I pass in an iid because I know hardcore protester will inherit from that, so I need to pass it in.  It calls Obj’s constructor with the correct values, including the given pointer to the StudentWorld and the IID.  All the values that aren’t equal to Obj’s default values are according to the specs.  The int wait is used to determine if the Protester can move or not on a given tick.  It is calculated by StudentWorld as it depends on the level and is passed in.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.
Protester::Protester(StudentWorld *w, int iid, int wait): Obj(w, iid, 60, 60, left) {
    setHp(5);
    m_wait = wait;
    m_currwait = 0;
    m_squaresToMove = rand() % 53 + 8;
    m_waitShout = 0;
    m_waitTurn = 0;
}

//Destructor of Protester
Protester::~Protester() {
}

//For protester I have to redefine isAlive.  This is in Protester to help my StudentWorld decide when to delete an obj.  For Protester it’s not truly dead when its hit points reach 0.  It just starts behaving in a different way.  It truly dies when It’s hit points reach 0, and it reaches the coordinates (60,60).  I redefine it to only return true if the above statement is met.  This is virtual because I’m redefining it from Obj, and it helps with readability.
bool Protester::isAlive() const{
    if (getX() == 60 && getY() == 60 && getHp() <= 0) {
        return false;
    }
    return true;
}

//I’m redefining canAnnoy in Protester because it shouldn’t return false.  It should return true.  This is here to help StudentWorld run it’s methods with more information.  Otherwise I wouldn’t be able to tell which of my Objs are protesters.  This is virtual because I’m redefining it from Obj, and it helps with readability.
bool Protester::canAnnoy() const {
    return true;
}

//This method damages the Protester with the given amount of hit points to deduct.  This also updates the wait time (from the game as an example) and plays a sound.  This is called by my StudentWorld to damage my Protester if a squirt or boulder hits it.  This returns false if the Protester dies, true otherwise.  It determines the sound to play on whether the Protester dies or not.  This is in Protester because StudentWorld calls this method to damage Protesters.  This is virtual because I’m redefining it from Obj, and it helps with readability.
bool Protester::damage(int hp) {
    //cout << getHp()<< endl;
    setHp(getHp()-hp);
    if (getHp() > 0) {
        getWorld()->playSound(SOUND_PROTESTER_ANNOYED);
        m_currwait = getWorld()->max(50, 100 - getWorld()->getLevel() * 10);
    } else {
        getWorld()->playSound(SOUND_PROTESTER_GIVE_UP);
        m_currwait = 0;
        return false;
    }
    return true;
}

//This method bribes the protester because it “picked up” gold.  It basically plays a sound, sets the hit points to zero, and increases the score.  This is called by my StudentWorld to bribe my Protester if it goes over a gold.  This doesn’t return anything because nothing needs to be returned.  This is in Protester because StudentWorld calls it for Protesters.  This is virtual because I’m redefining it from Obj, and it helps with readability, and also because I’ll redefine it for Hardcore Protesters later on.
void Protester::bribe() {
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(25);
    setHp(0);
}

//If the Regular Protester is in a “rest state” during the current tick, it must do nothing other than update its resting tick count, and immediately return false. Otherwise reset the rest tick and decrement necessary counters and return true.
bool Protester::canMove() {
    if (m_currwait > 0) {
        m_currwait--;
        return false;
    } else {
        m_currwait = m_wait;
        m_waitTurn--;
        m_waitShout--;
        return true;
    }
}

//The Regular Protester must move one square closer to its exit point using a queue-based maze-searching algorithm in StudentWorld.  After moving one step toward their exit point, the method will be done.
void Protester::gaveUp() {
    int tempdir = 0;
    int tempx = getX();
    int tempy = getY();
    tempdir = getWorld()->annoyedProtester(tempx, tempy);
    if (tempdir != 0) {
        setDirection(direc(tempdir));
        moveTo(tempx, tempy);
    }
}

//IN THE GAME IT RESTS FOR 45 TICKS, DOESN'T SAY SO IN THE SPECS
//If it can shout, this method plays SOUND_PROTESTER_YELL, informs the FrackMan that he’s been annoyed for a total of 2 annoyance points, and sets its wait counter to 45.  The specs say to update some state variable in a manner that prevents this Regular Protester from shouting again for at least 15 non-resting ticks, but the example game does not behave this way, rather doing what I made my method do.  It returns true if it shouted, false otherwise.
bool Protester::doShout() {
    if (m_waitShout <= 0) {
        getWorld()->playSound(SOUND_PROTESTER_YELL);
        getWorld()->annoyF(getX(), getY(), 4.0, 2);
        //m_waitShout = 15;
        //THE SPECS DON'T SAY TO DO THIS, BUT THE EXECUTIVE MAKES THE ENTIRE
        //FRACKMAN WAIT AFTER SHOUTING RATHER THAN JUST HAVING HIM NOT SHOUT
        //HITTING F AND COUNTING TICKS MAKES IT ROUGHLY 45.
        //RIGHT AFTER RECOVERING IT CAN SHOUT AGAIN
        m_currwait = 45;
        return true;
    }
    return false;
}

//This method changes the protester’s direction to face in the direction of the FrackMan, AND then take one step toward him. The Regular Protester will set its squaresToMove counter to zero, forcing it to pick a new direction/distance to move during its next non-resting tick.  Then the method will immediately return true.
bool Protester::seesFrackman() {
    int fdir =getWorld()->straightFrack(getX(), getY(), 4);
    if (fdir != -1) {
        if (fdir != 0) {
            setDirection(direc(fdir));
            int tempx = getX();
            int tempy = getY();
            setCoord(direc(fdir), tempx, tempy, 1);
            moveTo(tempx, tempy);
        }
        m_squaresToMove = 0;
        return true;
    } else {
        fdir =getWorld()->straightFrack(getX(), getY(), 0);
        if (fdir > 0) {
            setDirection(direc(fdir));
        }
        m_squaresToMove = 0;
        return true;
    }
}

//regMove decrements its squaresToMove variable by one. If the Regular Protester has finished walking squaresToMove steps in its currently-selected direction then:
//a. The Regular Protester will pick a random new direction to move.
//b. If the random direction is blocked, then it will continue checking directions until it has picked a direction that is not blocked.
//c. The Regular Protester will then change its direction to this new chosen direction.
//d. The Regular Protester will pick a new value for squaresToMove in between 8 and 60.
//e. The Regular Protester will then jump over the next part
//Otherwise if the Regular Protester is sitting at an intersection where it could turn and move at least one square, and it hasn’t made a perpendicular turn in the last 200 non-resting ticks. Then the Regular Protester will:
//a. Determine which of the two perpendicular directions are viable.
//b. Pick a viable perpendicular direction. If both perpendicular directions are viable, then pick one of the two choices randomly.
//c. Set its direction to the selected perpendicular direction.
//d. Pick a new value for squaresToMove between 8 and 60.
//e. Set a counter back to 200 (ticks to turn)
//f. Continue on with the next step.
//Finally, the Regular Protester will then attempt to take one step in its currently facing direction.  If the Regular Protester is for some reason blocked, it will set squaresToMove to zero, resulting in a new direction being chosen during the Regular Protester’s next non-resting tick (but not the current tick – the Regular Protester must do nothing during the current tick).
void Protester::regMove() {
    if (--m_squaresToMove <=0) {
        Direction dir[4] = {up, left, down, right};
        random_shuffle(&dir[0], &dir[4]);
        for (int i = 0; i < 4; i++) {
            int tempx = getX();
            int tempy = getY();
            setCoord(dir[i], tempx, tempy, 1);
            if (getWorld()->clearDirtBoulder(tempx, tempy) && tempy <=60 && tempy >=0 && tempx <=60 && tempx >=0) {
                setDirection(dir[i]);
                break;
            }
        }
        m_squaresToMove = rand() % 53 + 8;
    } else if (m_waitTurn <= 0){
        int tempx = getX();
        int tempy = getY();
        doTurn(getDirection(), tempx, tempy);
    }
    int tempx = getX();
    int tempy = getY();
    setCoord(getDirection(), tempx, tempy, 1);
    if (getWorld()->clearDirtBoulder(tempx, tempy) && tempy <=60 && tempy >=0 && tempx <=60 && tempx >=0) {
        moveTo(tempx, tempy);
    } else {
        m_squaresToMove = 0;
    }

}

//Given the direction the Protester is currently in and it’s coordinates, it turns in a perpendicular direction if it’s able to.  If both perpendicular directions are turnable into, then it randomly chooses one.
void Protester::doTurn(Direction d, int x, int y) {
    int temp = 0;
    if (d == up || d == down) {
        if (getWorld()->clearDirtBoulder(x+1, y)) {
            temp += 1;
        }
        if (getWorld()->clearDirtBoulder(x-1, y)) {
            temp += 2;
        }
        if (temp == 1) {
            setDirection(right);
        } else if (temp == 2) {
            setDirection(left);
        } else if (temp == 3){
            int randnum = rand() % 2;
            if (randnum == 0) {
                setDirection(right);
            } else {
                setDirection(left);
            }
        }
    } else if (d == left || d == right) {
        if (getWorld()->clearDirtBoulder(x, y+1)) {
            temp += 1;
        }
        if (getWorld()->clearDirtBoulder(x, y-1)) {
            temp +=2;
        }
        if (temp == 1) {
            setDirection(up);
        } else if (temp == 2) {
            setDirection(down);
        } else if (temp == 3){
            int randnum = rand() % 2;
            if (randnum == 0) {
                setDirection(up);
            } else {
                setDirection(down);
            }
        }
    }
    if (temp != 0) {
        m_waitTurn = 200;
        m_squaresToMove = rand() % 53 + 8;
    }
}

//Sets the private wait variable to the given integer.  This is primarily for HardCore Protesters to use as they can’t access this class’s privates.
void Protester::setWait(int x) {
    m_currwait = x;
}

//This method is in Protester because every Protester needs a doSomething method.  Also, because Obj defined doSomething as a pure abstract method, every derived subclass needs an implementation of it to be instantiated, which Sonar needs to be.  It is virtual to override the doSomething in Obj, and to let Hardcore Protester override it later.
bool Protester::doSomething() {
    //The Regular Protester must check to see if it is currently alive. If not, then it's doSomething() method must return false immediately.
    if (!isAlive()) {
        return false;
    }
    //If the Regular Protester is in a “rest state” during the current tick, it must do nothing other than update its resting tick count, and immediately return.  I do this by using a protected method.  If it returns false I will return false.
    if (!canMove()) {
        return false;
    }
    //Otherwise, if the Regular Protester is in a leave-the-oil-field state (its hp is less than or equal to zero but it’s still alive), then call gaveUp and return false.
    if (getHp() <= 0) {
        gaveUp();
        return false;
    //Otherwise, the Regular Protester will check to see if they are within a distance of 4 units of the FrackMan, AND they are currently facing in the FrackMan’s direction using a method from studentWorld. According to the specs, if both are true and the Regular Protester hasn’t shouted within its last non-resting 15 ticks, then the Regular Protester do it's Shout.  However, according to the example game, it sets its wait counter to 45 ticks, and can immediately shout upon exiting it.  I’m going with the game.  It calls doShout, which returns true if it can shout.  If it returns true, then my doSomething returns true as the health changed and I need to update that.
    } else if (getWorld()->canShout(getX(), getY(), getDirection())) {
        if (doShout()) {
            return true;
        }
    //Otherwise, if the Regular Protester is in a straight horizontal or vertical line of sight to the FrackMan, is more than 4 units away from the FrackMan, and can move the entire way to the FrackMan with no Dirt or Boulders blocking its path, then the Regular Protester will call see Frackman and return false as nothing in the text will need to be changed.
    }else if (getWorld()->straightFrack(getX(), getY(), 0) > 0) {
        seesFrackman();
        return false;
    } else  {
        regMove();
    }
    return false;
}

//      _  _ ____ ____ ___  ____ ____ ____ ____
//      |__| |__| |__/ |  \ |    |  | |__/ |___
//      |  | |  | |  \ |__/ |___ |__| |  \ |___
//
//      ___  ____ ____ ___ ____ ____ ___ ____ ____
//      |__] |__/ |  |  |  |___ [__   |  |___ |__/
//      |    |  \ |__|  |  |___ ___]  |  |___ |  \
//

//This is the constructor for HardcoreProtester.  It doesn’t need an x and y coordinate because they all start at 60,60 and I can directly pass that in Obj’s constructor.  It needs an integer that represents the wait tick, (It can move one / wait ticks).  It calls Protester’s constructor with the correct values, including the given pointer to the StudentWorld and the wait integer.  The int wait is used to determine if the HardCoreProtester can move or not on a given tick.  It is calculated by StudentWorld as it depends on the level and is passed in.  This inherits from Protester rather than from Obj because it is essentially the same thing with a bit of added functionality.
HardcoreProtester::HardcoreProtester(StudentWorld *w, int wait): Protester(w, IID_HARD_CORE_PROTESTER, wait) {
    setHp(20);
}

//Destructor of HardcoreProtester
HardcoreProtester::~HardcoreProtester() {
}

//This method bribes the hardcoreprotester because it “picked up” gold.  It basically plays a sound, sets the wait ticker to a given number, and increases the score.  This is called by my StudentWorld to bribe my HardcoreProtester if it goes over a gold.  This doesn’t return anything because nothing needs to be returned.  This is in HardcoreProtester because StudentWorld calls it for HardcoreProtesters.  This is virtual because I’m redefining it from Protester because it works slightly differently, and doing so helps with readability.
void HardcoreProtester::bribe() {
    getWorld()->playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld()->increaseScore(50);
    setWait(getWorld()->max(50, 100 - getWorld()->getLevel() * 10));
}

//This method returns true because hardcore protesters are hardcore.  This method is used in StudentWorld to determine which objects are hardcore.  It can do certain things to hardcore objects that it can’t do to other things.  This function is here to redefine it.  This function is virtual because I’m overriding Obj’s function, and having virtual there helps with readability.
bool HardcoreProtester::isHardcore() const {
    return true;
}

//This method returns true if Hardcore is close to Frack, and moves one towards him using a method from Studentworld and a queue-based maze searching algorithm.
bool HardcoreProtester::closeToFrack() {
    int tempx = getX();
    int tempy = getY();
    int tempd =getWorld()->isNearFrack(tempx, tempy);
    if (tempd > 0) {
        setDirection(direc(tempd));
        moveTo(tempx, tempy);
        return true;
    }
    return false;
}

//This method is in HardcoreProtester because every HardcoreProtester needs a doSomething method.  It is virtual to override the doSomething in Protester and for better readablity.
bool HardcoreProtester::doSomething() {
    if (!isAlive()) {
        return false;
    }
    if (!canMove()) {
        return false;
    }
    if (getHp() <= 0) {
        gaveUp();
    } else if (getWorld()->canShout(getX(), getY(), getDirection())) {
        if (doShout()) {
            return true;
        }
    } else if (closeToFrack()) {
        return false;
    }else if (getWorld()->straightFrack(getX(), getY(), 0) > 0) {
        //If Protester can "see" frackman it goes to him and returns.
        //MAYBE CHANGE LATER, DON'T KNOW HOW TO SOLVE THIS
        seesFrackman();
        return false;
    } else  {
        regMove();
    }
    return false;
}

//      ____ ____ ____ ____ _  _ _  _ ____ _  _
//      |___ |__/ |__| |    |_/  |\/| |__| |\ |
//      |    |  \ |  | |___ | \_ |  | |  | | \|
//

//This is the constructor for Frackman.  All it needs is a pointer to the world it is created in.  Everything else is assumed.  It calls Obj’s constructor with the correct values, and passes in the pointer to StudentWorld given by the parameters.  All the values that aren’t equal to Obj’s default values are according to the specs.  This inherits from Obj rather than from Actor as will everything else because all it need a pointer to a StudentWorld.  Inside it sets some private members to their initial values.
Frackman::Frackman(StudentWorld* w): Obj(w, IID_PLAYER, 30, 60) {
    setHp(10);
    m_squirts = 5;
    m_sonar = 1;
    m_gold = 0;
    m_d = right;
}

//Destructor of Frackman
Frackman::~Frackman() {
}

//This is an accessor for a private member that StudentWorld can be used to find how many squirts are in Frackman to put in the text at the top.  It is const because nothing is changed inside it.  This is in Frackman because the StudentWorld needs to get it from the Frackman object.  This is not virtual because nothing will inherit from Frackman.
int Frackman::getSquirts() const{
    return m_squirts;
}

//This is an accessor for a private member that StudentWorld can use to add squirts to Frackman.  This is void because it doesn’t need to return anything.  Every time this method is called by StudentWorld it will add 5 squirts to Frackman’s inventory.  This is in Frackman because the StudentWorld needs to get it from the Frackman object.  This is not virtual because nothing will inherit from Frackman.
void Frackman::addSquirts() {
    m_squirts +=5;
}

//This is an accessor for a private member that StudentWorld can be used to find how many gold Frackman has to put in the text at the top.  It is const because nothing is changed inside it.  This is in Frackman because the StudentWorld needs to get it from the Frackman object.  This is not virtual because nothing will inherit from Frackman.
int Frackman::getGold() const{
    return m_gold;
}

//This is an accessor for a private member that StudentWorld can use to add gold to Frackman.  This is void because it doesn’t need to return anything.  Every time this method is called by StudentWorld it will add 1 gold to Frackman’s inventory.  This is in Frackman because the StudentWorld needs to get it from the Frackman object.  This is not virtual because nothing will inherit from Frackman.
void Frackman::addGold() {
    m_gold++;
}

//This is an accessor for a private member that StudentWorld can be used to find how many sonars Frackman has to put in the text at the top.  It is const because nothing is changed inside it.  This is in Frackman because the StudentWorld needs to get it from the Frackman object.  This is not virtual because nothing will inherit from Frackman.
int Frackman::getSonar() const{
    return m_sonar;
}

//This is an accessor for a private member that StudentWorld can use to add sonars to Frackman.  This is void because it doesn’t need to return anything.  Every time this method is called by StudentWorld it will add 1 sonar to Frackman’s inventory.  This is in Frackman because the StudentWorld needs to get it from the Frackman object.  This is not virtual because nothing will inherit from Frackman.
void Frackman::addSonar() {
    m_sonar++;
}

//This is a private function that basically sends out a squirt using a method in StudentWorld
void Frackman::squirt() {
    Direction d = getDirection();
    int x = getX();
    int y = getY();
    setCoord(d, x, y, 4);
    if(!getWorld()->clearDirtBoulder(x, y)) {
        m_squirts--;
        getWorld()->playSound(SOUND_PLAYER_SQUIRT);
        return;
    }
    m_squirts--;
    getWorld()->playSound(SOUND_PLAYER_SQUIRT);
    getWorld()->squirt(x,y);
}

//This is a private function that basically places a gold using a method in StudentWorld
void Frackman::placeGold() {
    m_gold--;
    getWorld()->placeGold(getX(), getY());
}

//This is a private function that decrements m_sonars by one, plays a sound, and uses a method in StudentWorld to reveal the surrounding hidden objects.
void Frackman::activateSonar() {
    m_sonar--;
    getWorld()->playSound(SOUND_SONAR);
    getWorld()->reveal(getX(), getY());
}

//This method is in Frackman because every Frackman needs a doSomething method.  Also, because Obj defined doSomething as a pure abstract method, every derived subclass needs an implementation of it to be instantiated, which Sonar needs to be.  It is virtual to override the doSomething in Obj and for better readability.
bool Frackman::doSomething() {
    //Returns false if not alive.
    if (!isAlive()) {
        return false;
    }
    //Calls on a method in StudentWorld to delete overlapping dirt objects, and to play SOUND_DIG if any dirt was removed this tick.
    if(getWorld()->removeDirt(getX(), getY())) {
        getWorld()->playSound(SOUND_DIG);
    }
    
    Direction d;
    int ch;
    if (getWorld()->getKey(ch) == true) {
        switch(ch)
        {
            // If the user presses the Escape key, this allows the user to abort the current level. In this case, the FrackMan object should set itself to completely annoyed.
            case KEY_PRESS_ESCAPE:
                setHp(0);
                return false;
                break;
            //If the user presses the spacebar key, then the FrackMan will fire a Squirt into the oil field, assuming they have sufficient water in their squirt gun to do so. I use a private method to do the following: The FrackMan will then reduce their water count by 1. To fire a Squirt, the player must create and add a new Squirt object into the oil field at a location that is four squares immediately in front of the FrackMan, facing the same direction as the FrackMan. Every time the FrackMan fires a Squirt, it must play the sound SOUND_PLAYER_SQUIRT. If the initial location where the Squirt starts is occupied by one or more Dirt objects or is within a radius of 3.0 of a Boulder object, then the FrackMan must still make a squirting noise and will waste 1 unit of water, but the FrackMan must not add any Squirt object into the oil field.
            case KEY_PRESS_SPACE:
                if (m_squirts <= 0) {
                    return false;
                }
                squirt();
                return true;
                break;
            //If the player wants their FrackMan to turn to face a different direction then it is currently facing, then the FrackMan’s direction should be adjusted to the indicated direction but the FrackMan must not move in that direction.
            //Else, if the FrackMan is already facing in a particular direction and the user hits the arrow key to move in that same direction, AND the FrackMan is allowed to move in that direction, then your code must use the GraphObject’s moveTo() method to adjust the FrackMan’s location by one square in the indicated direction. Valid coordinates for the FrackMan are from x=0,y=0 to x=60,y=60, inclusive. Moreover, the FrackMan cannot occupy a square that is less than or equal to a radius of 3 away from the center of any Boulder.
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
            //If the player presses the ‘Z’ or ‘z’ keys and the FrackMan has one or more sonar charges remaining in his inventory, then this will cause the FrackMan to use his sonar charge to illuminate the contents of the oil field within a radius of 12 of his location. I use a method to do the following: i. The count of sonar charges held by the FrackMan must be decremented by 1. ii. All hidden game objects that are within a radius of 12 must be made visible via setVisible() and revealed to the player.
            case 'Z':
            case 'z':
                if (m_sonar <=0) {
                    return false;
                }
                activateSonar();
                return true;
                break;
            //If the player presses the TAB key and they have one or more units of gold in their inventory, then they must add a Gold Nugget object into the oil field at their current x,y location and reduce their gold count by 1 unit. I use a method to do this.  The dropped Gold Nugget must have a temporary state and a lifetime of 100 game ticks, and is only pickup-able by Protesters. Such a dropped Nugget will start out in a visible state.
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
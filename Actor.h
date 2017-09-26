#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "Compiler.h"

class StudentWorld;

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


class Actor : public GraphObject {
public:
    Actor(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir = right, unsigned int depth = 2);
    virtual void doSomething() = 0;
    StudentWorld* getPtr() const {return m_studentWorld;};
    virtual bool isPoison() const;
    virtual bool isWater() const;
    virtual bool isFood() const;
    virtual bool isInsect() const;
    virtual bool isPebble() const;
    virtual bool isAnthill() const;
    virtual bool isAnt() const;
    virtual bool hasHealth() const;
    virtual int setHealth(int change);          //need this here to avoid using dynamic cast
    virtual int getHealth() const;
    virtual bool isDead() const;
    virtual void setDead();
    virtual bool canBiteBack() const;
    virtual bool didMove();
    virtual void setBit(bool bit);
    virtual bool getBitten();
    virtual void setFoodHeld(int amt);
    virtual int foodHeld();
    virtual int getColony() const;
    virtual void setBlocked();
    virtual bool gotBlocked() const;
    virtual bool isPheromone() const;
    virtual ~Actor();
private:
    StudentWorld* m_studentWorld;
    int m_health;
    bool m_isdead;
    int foodheld;
};


class EnergyHolder : public Actor {
public:
    EnergyHolder(int imageID, int startX, int startY, StudentWorld* ptr, int energyUnits, Direction dir = right, unsigned int depth = 2);
    virtual int setHealth(int change);
    virtual int getHealth() const;
    virtual bool hasHealth() const;
    virtual bool isDead() const;
    virtual void setDead();
    virtual void doSomething() = 0;
    virtual ~EnergyHolder();
private:
    int m_health;
    bool m_isdead;
};


class Pebble : public Actor {
public:
    Pebble(int startX, int startY, StudentWorld* ptr);
    virtual void doSomething() {return;};
    virtual bool isPebble() const;
    virtual ~Pebble() {};
private:
    
};


class Pool : public Actor {
public:
    Pool(int startX, int startY, StudentWorld* ptr);
    virtual void doSomething();
    virtual bool isWater() const;
    virtual ~Pool() {};
private:
    
};


class Insect : public EnergyHolder {
public:
    Insect(int imageID, int startX, int startY, StudentWorld* ptr, int energyUnits);
    virtual void bite(Actor* target) = 0;
    virtual void doSomething() = 0;
    bool isStunned();
    void resetStun();
    void decrementStun();
    virtual ~Insect();
    int getTicks() const;
    void increaseTicks();
    void decreaseTicks();
    virtual bool didMove();
    void setMovedTrue();
    void setMovedFalse();
    virtual bool isInsect() const;
private:
    int m_stillStunned;
    int tickCount;
    bool moved;
};

class Poison : public Actor {
public:
    Poison(int startX, int startY, StudentWorld* ptr);
    virtual void doSomething();
    void poison(Insect* insect);
    virtual bool isPoison() const;
    virtual ~Poison() {};
private:
    
};


class Food : public EnergyHolder {
public:
    Food(int startX, int startY, StudentWorld* ptr, int energyUnits);
    virtual void doSomething();
    virtual bool isFood() const;
    virtual bool hasHealth() const;
    virtual ~Food() {};
private:
    
};


class Anthill : public EnergyHolder {
public:
    Anthill(int imageID, int startX, int startY, StudentWorld* ptr, int colony, Compiler* cmp);
    void giveBirth(Compiler* cmp);
    virtual void doSomething();
    virtual bool isAnthill() const;
    virtual int getColony() const;
    virtual ~Anthill();
private:
    int m_colonyNumber;
    Compiler* m_anthillComp;
};


class Pheromone : public EnergyHolder {
public:
    Pheromone(int imageID, int startX, int startY, int colonyNumber, StudentWorld* ptr);
    virtual void doSomething();
    virtual bool isPheromone() const;
    virtual int getColony() const;
    virtual ~Pheromone(){};
private:
    int m_pheromoneLevel;
    int m_colonyNumber;
};



class Ant : public Insect {
public:
    Ant(int imageID, int startX, int startY, StudentWorld* ptr, int colony, Compiler* cmp);
    virtual void doSomething();
    virtual void bite(Actor* target);
    virtual void setBlocked(bool blocked);
    virtual bool gotBlocked() const;
    void setBit(bool bitten);
    bool getBitten();
    virtual int foodHeld();
    virtual void setFoodHeld(int amt);
    virtual bool isAnt() const;
    bool interpreter();
    virtual bool hasHealth() const;
    int convert(std::string operand);
    virtual int getColony() const;
    virtual ~Ant();
private:
    Compiler* m_compiler;
    int m_colonyNumber;
    int m_foodHeld;
    int instructionCounter;
    bool wasJustBitten;
    bool wasJustBlocked;
    int lastRandom;
};


class Grasshopper : public Insect {
public:
    Grasshopper(int imageID, int startX, int startY, StudentWorld* ptr, int energyUnits);
    virtual void doSomething()=0;
    virtual void bite(Actor* target) = 0;
    int getDistance() const;
    void setDistance();
    void decrementDistance();
    int getSleepTime() const;
    void decreaseSleepTime();
    void resetSleepTime();
    void resetSleepRightorUp();         //need this because for some reason adult grasshoppers skip a sleep 
    void setAteSleep();
    void setAte(bool didIt);
    bool getAte();
    virtual ~Grasshopper();
private:
    int m_sleepTime;
    int m_distance;
    bool justAte;
};


class AdultGrasshopper : public Grasshopper {
public:
    AdultGrasshopper(int startX, int startY, StudentWorld* ptr);
    virtual bool jump();
    virtual void doSomething();
    virtual void bite(Actor* target);
    virtual bool canBiteBack() const;
    virtual bool hasHealth() const;
    virtual ~AdultGrasshopper(){};
private:
    
};


class BabyGrasshopper : public Grasshopper {
public:
    BabyGrasshopper(int startX, int startY, StudentWorld* ptr);
    virtual void bite(Actor* target);
    virtual void doSomething();
    virtual bool hasHealth() const;
    virtual ~BabyGrasshopper(){};
private:
    
};




#endif // ACTOR_H_

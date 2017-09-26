#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Field.h"
#include "Actor.h"
#include "GraphObject.h"
#include <string>
#include <vector>


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp


class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    
    virtual int init();
    
    virtual int move();
    
    virtual void cleanUp();
    
    void setDisplayText();
    
    void replaceWith(Actor::Direction dir, int origX, int origY, Actor* moveMe);
    
    ~StudentWorld();
    
    int rightTicks() const;
    
    bool isValidMove(int x, int y, Actor::Direction dir, Actor* insect);
    
    void evolve(int x, int y, Insect* old);
    
    void addFood(int x, int y, int amt);
    
    bool findPoison(int x, int y, Insect* insect);
    
    bool findWater(int x, int y, Insect* insect);
    
    bool findFood(int x, int y, EnergyHolder* energyholder, int amt);
    
    Actor* findInsect(int x, int y, Insect* insect);
    
    void getRidOf(int x, int y, Actor* end, Actor::Direction dir);
    
    void addAnts(int x, int y, int colonyNumber, Compiler* cmp);
    
    bool checkJump(int x, int y);
    
    void updateForJump(int x, int y, int horizontal, int vertical, AdultGrasshopper* grass);
    
    void addPheromone(int x, int y, int colonyNumber, int amt);
    
    bool findAnthill(int x, int y, int colonyNumber, Ant* ant);
    
    bool findPheromone(int x, int y, int colonyNumber, Ant* ant);
    
    bool smellDanger(int x, int y, int colonyNumber, Actor::Direction dir);
    
    int getWinning(int arr[], int size);

private:
    std::vector<Actor*> actorVec[VIEW_WIDTH][VIEW_HEIGHT];
    int ticks;
    std::vector<Compiler*> compilers;
    int numOfAnts[4];
    std::vector<std::string> colonynames;
    int winningNumber;
};

#endif // STUDENTWORLD_H_









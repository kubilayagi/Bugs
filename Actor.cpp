#include "Actor.h"
#include <random>
#include "Field.h"
#include <vector>
#include "StudentWorld.h"
#include <cmath>
#include "Compiler.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////                            ACTOR                               /////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Actor::Actor(int imageID, int startX, int startY, StudentWorld* ptr, Direction dir, unsigned int depth)
: GraphObject(imageID, startX, startY, dir, depth)
{
    m_studentWorld = ptr;
    m_health = 1;
    m_isdead = false;
    foodheld = 0;
}


Actor::~Actor() {
    //setVisible(false);
}


bool Actor::isPoison() const {
    return false;
}

bool Actor::isWater()const{return false;}
bool Actor::isFood()const{return false;}
bool Actor::isInsect() const {return false;}
bool Actor::isPebble() const {return false;}
bool Actor::hasHealth() const {return false;}
int Actor::getHealth() const {return 1;}
int Actor::setHealth(int change) {m_health = 1; return m_health;}
bool Actor::isDead() const {return m_isdead;}
void Actor::setDead() {m_isdead = true; return;}
bool Actor::canBiteBack() const {return false;}
bool Actor::didMove() {return false;}
bool Actor::isAnthill() const {return false;}
bool Actor::isAnt() const {return false;}
void Actor::setFoodHeld(int amt) {foodheld = amt;}
int Actor::foodHeld() {return foodheld;}
int Actor::getColony() const {return -1;}
void Actor::setBit(bool bit) {return;}
bool Actor::getBitten() {return false;}
void Actor::setBlocked() {return;}
bool Actor::gotBlocked() const {return false;}
bool Actor::isPheromone() const {return false;}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////           ENERGY HOLDER           ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


EnergyHolder::EnergyHolder(int imageID, int startX, int startY, StudentWorld* ptr, int energyUnits, Direction dir,  unsigned int depth)
: Actor(imageID, startX, startY, ptr, dir, depth) {
    m_health = energyUnits;
    m_isdead = false;
}

EnergyHolder::~EnergyHolder() {
    
}

int EnergyHolder::setHealth(int change)
{
    m_health += change;
    return m_health;
}

int EnergyHolder::getHealth() const {return m_health;}
bool EnergyHolder::isDead() const {return m_isdead;}
void EnergyHolder::setDead() {m_isdead = true;}
bool EnergyHolder::hasHealth() const {return true;}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                   ANT    HILL            ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Anthill::Anthill(int imageID, int startX, int startY, StudentWorld* ptr, int colony, Compiler* cmp)
: EnergyHolder(imageID, startX, startY, ptr, 8999){
    m_anthillComp = cmp;
    m_colonyNumber = colony;
}

void Anthill::giveBirth(Compiler* cmp) {
    StudentWorld* ptr = getPtr();
    ptr->addAnts(getX(), getY(), m_colonyNumber, cmp);
}

bool Anthill::isAnthill() const {return true;}
int Anthill::getColony() const {return m_colonyNumber;}

void Anthill::doSomething() {
   
    StudentWorld* ptr = getPtr();
    setHealth(-1);
    if (getHealth() <= 0 ) {
        setDead();
        return;
    }
    
    if (ptr->findFood(getX(), getY(), this, 10000)) {
        return;
    }
    
    
    
    
    if (getHealth() >= 2000) {
        giveBirth(m_anthillComp);
        this->setHealth(-1500);
    }
    
    
    return;
    
}

Anthill::~Anthill() {
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                           PEBBLE                               ///////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////


Pebble::Pebble(int startX, int startY, StudentWorld* ptr)
: Actor(IID_ROCK, startX, startY, ptr, right, 1) {
    
}

bool Pebble::isPebble() const{
    return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                           FOOD                               ///////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Food::Food(int startX, int startY, StudentWorld* ptr, int energyUnits)
: EnergyHolder(IID_FOOD, startX, startY, ptr, energyUnits) {
    
}

void Food::doSomething() {
    if (getHealth() <= 0)
        setDead(); 
        return;
}

bool Food::isFood() const{
    return true;
}

bool Food::hasHealth() const {return true;}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                           POOL                               ///////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pool::Pool(int startX, int startY, StudentWorld* ptr) : Actor(IID_WATER_POOL, startX, startY, ptr) {
    
}

bool Pool::isWater() const{
    return true;
}

void Pool::doSomething() {
    return;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////                        PHEROMONE                             ///////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Pheromone::Pheromone(int imageID, int startX, int startY, int colonyNumber, StudentWorld* ptr)
: EnergyHolder(imageID, startX, startY, ptr, 256) {
    m_colonyNumber = colonyNumber;
}

bool Pheromone::isPheromone() const {return true;}
int Pheromone::getColony() const {return m_colonyNumber;}

void Pheromone::doSomething() {
    if (getHealth() > 768) {
        int subtract = getHealth()-768;
        setHealth(-subtract);
    }
    setHealth(-1);
    if (getHealth() < 0) {
        setDead();
    }
    return;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////            INSECT               ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Insect::Insect(int imageID, int startX, int startY, StudentWorld* ptr, int energyUnits)
: EnergyHolder(imageID, startX, startY, ptr, energyUnits, static_cast<GraphObject::Direction>(randInt(1,4)), 1) {
    tickCount = 0;
    moved = false;
    m_stillStunned = 0;
}

Insect::~Insect() {
    //setVisible(false);
}


int Insect::getTicks() const {return tickCount;}
void Insect::increaseTicks(){tickCount++;}
void Insect::decreaseTicks(){tickCount--;}
bool Insect::isStunned() {return (m_stillStunned > 0);}
void Insect::decrementStun(){m_stillStunned--; return;};
void Insect::resetStun(){m_stillStunned = 2; return;}
bool Insect::isInsect()const{return true;}
bool Insect::didMove() {return moved;}
void Insect::setMovedTrue() {moved = true;}
void Insect::setMovedFalse() {moved = false;}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////            POISON               ///////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Poison::Poison(int startX, int startY, StudentWorld* ptr) : Actor(IID_POISON, startX, startY, ptr) {
    
}

void Poison::poison(Insect* insect) {
    insect->setHealth(-150);
}

void Poison::doSomething() {return;}

bool Poison::isPoison() const{
    return true;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////                   ANT                    ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////



Ant::Ant (int imageID, int startX, int startY, StudentWorld* ptr, int colony, Compiler* cmp)
: Insect(imageID, startX, startY, ptr, 1500)
{
    m_colonyNumber = colony;
    instructionCounter = 0;
    wasJustBitten = false;
    wasJustBlocked = false;
    m_compiler = cmp;
    m_foodHeld = 0;
    lastRandom = 0;
}

bool Ant::hasHealth() const {return true;}
int Ant::getColony() const {return m_colonyNumber;}



int Ant::convert(std::string operand) {
    return atoi(operand.c_str());
}

bool Ant::interpreter() {
    StudentWorld* ptr = getPtr();

    //instructionCounter = 0;
    for (int i = 0; i < 10; i++) {
        //todo: implement the actions based on the commands from the compiler first
        //get command from compiler string command = cmp->getCommand() or some shit
        Actor* target;
        Compiler::Command cmd;
        
        if (!(*m_compiler).getCommand(instructionCounter, cmd) ) {
            setDead();
            return false; // error - no such instruction!
        }
        
        switch (cmd.opcode) {
            case Compiler::moveForward:    //move forward
                if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                    if (getDirection() == up) {
                        moveTo(getX(), getY()+1);
                        setMovedTrue();
                    }
                    else if (getDirection() == down) {
                        moveTo(getX(), getY()-1);
                        setMovedTrue();
                    }
                    else if (getDirection() == left) {
                        moveTo(getX()-1, getY());
                        setMovedTrue();
                    }
                    else if (getDirection() == right) {
                        moveTo(getX()+1, getY());
                        setMovedTrue();
                    }
                    setBlocked(false);
                    setBit(false);
                }
                else {
                    setBlocked(true);
                    setMovedFalse();
                }
                instructionCounter++;
                return true;
                break;
            case Compiler::eatFood:                       //eat food
                if (foodHeld() <= 100) {
                    setHealth(foodHeld());
                    setFoodHeld(0);
                }
                else if (foodHeld() > 100) {
                    setHealth(100);
                    setFoodHeld(foodHeld()-100);
                }
                instructionCounter++;
                return true;
                break;
            case Compiler::dropFood:   //drop food
                if (ptr->findAnthill(getX(), getY(), m_colonyNumber, this)) {
                    ptr->addFood(getX(), getY(), foodHeld());
                    setFoodHeld(-foodHeld());
                }
                instructionCounter++;
                return true;
                break;
            case Compiler::bite: //bite
                target = ptr->findInsect(getX(), getY(), this);
                if (target != nullptr && target->isInsect())
                    bite(target);
                instructionCounter++;
                return true;
                break;
            case Compiler::pickupFood:   //find food
                if (!ptr->findAnthill(getX(), getY(), m_colonyNumber, this)) {
                    ptr->findFood(getX(), getY(), this, 400);
                }
                instructionCounter++;
                return true;
                break;
            case Compiler::faceRandomDirection:   //face random directions
                setDirection(static_cast<GraphObject::Direction>(randInt(1,4)));
                instructionCounter++;
                return true;              //todo: why does this make them just spin around
                break;
            case Compiler::generateRandomNumber: //generate random number
                //todo: replace 10 with operand1
                instructionCounter = randInt(0, convert(cmd.operand1));
                lastRandom = randInt(0, convert(cmd.operand1));
                if (convert(cmd.operand1) == 0) {
                    instructionCounter = 0;
                }
                break;
            case Compiler::emitPheromone:
                ptr->addPheromone(getX(), getY(), m_colonyNumber, 256);
                instructionCounter++;
                return true;
                break;
            case Compiler::rotateClockwise:
                if (getDirection() == up) {
                    setDirection(right);
                }
                else if (getDirection() == down) {
                    setDirection(left);
                }
                else if (getDirection() == left) {
                    setDirection(up);
                }
                else if (getDirection() == right) {
                    setDirection(down);
                }
                instructionCounter++;
                //return true;
                break;
            case Compiler::rotateCounterClockwise:
                if (getDirection() == up) {
                    setDirection(left);
                }
                else if (getDirection() == down) {
                    setDirection(right);
                }
                else if (getDirection() == left) {
                    setDirection(down);
                }
                else if (getDirection() == right) {
                    setDirection(up);
                }
                instructionCounter++;
                return true;
                break;
            case Compiler::goto_command:
                instructionCounter = convert(cmd.operand1);
                break;
            case Compiler::if_command:
                switch(convert(cmd.operand1)) {
                    case Compiler::last_random_number_was_zero:
                        if (lastRandom == 0) {
                            instructionCounter = convert(cmd.operand2);
                        }
                        else
                            instructionCounter++;
                        break;
                    case Compiler::i_am_carrying_food:
                        if (foodHeld() > 0) {
                            instructionCounter = convert(cmd.operand2);
                        }
                        else {
                            instructionCounter++;
                        }
                        break;
                    case Compiler::i_am_hungry:
                        if (getHealth() <= 25)
                            instructionCounter = convert(cmd.operand2);
                        else
                            instructionCounter++;
                        break;
                    case Compiler::i_am_standing_with_an_enemy:
                        if (ptr->findInsect(getX(), getY(), this) != nullptr)
                            instructionCounter = convert(cmd.operand2);
                        else
                            instructionCounter++;
                        break;
                    case Compiler::i_am_standing_on_food:
                        if (ptr->findFood(getX(), getY(), this, 0))
                            instructionCounter = convert(cmd.operand2);
                        else
                            instructionCounter++;
                        break;
                    case Compiler::i_am_standing_on_my_anthill:
                        if (ptr->findAnthill(getX(), getY(), m_colonyNumber, this))
                            instructionCounter = convert(cmd.operand2);
                        else
                            instructionCounter++;
                        break;
                    case Compiler::i_smell_pheromone_in_front_of_me:
                        if (ptr->findPheromone(getX(), getY(), m_colonyNumber, this)) {
                            instructionCounter = convert(cmd.operand2);
                        }
                        else {
                            instructionCounter++;
                        }
                        break;
                    case Compiler::i_smell_danger_in_front_of_me:
                        if (ptr->smellDanger(getX(), getY(), m_colonyNumber, getDirection())) {
                            instructionCounter = convert(cmd.operand2);
                        }
                        else {
                            instructionCounter++;
                        }
                        break;
                    case Compiler::i_was_bit:
                        if (getBitten()) {
                            instructionCounter = convert(cmd.operand2);
                            setBit(false);
                        }
                        else {
                            instructionCounter++;
                        }
                        break;
                    case Compiler::i_was_blocked_from_moving:
                        if (gotBlocked()) {
                            instructionCounter = convert(cmd.operand2);
                            setBlocked(false);
                        }
                        else {
                            instructionCounter++;
                        }
                        break;
                }
                break;
            default:
                instructionCounter++;
                break;
        }
    }
    return true;
}






void Ant::setBlocked(bool blocked) {wasJustBlocked = blocked;}
bool Ant::gotBlocked() const {return wasJustBlocked;}
void Ant::setBit(bool bitten) {wasJustBitten = bitten;}
bool Ant::getBitten() {return wasJustBitten;}
int Ant::foodHeld() {return m_foodHeld;}
void Ant::setFoodHeld(int amt) {m_foodHeld += amt; if (m_foodHeld > 1800) {m_foodHeld = 1800;}}
bool Ant::isAnt() const {return true;}

void Ant::bite(Actor* target) {
    target->setHealth(-15);
    if (target->getHealth() <= 0)
        target->setDead();
}


void Ant::doSomething() {
    StudentWorld* ptr = getPtr();
    
    setHealth(-1);
    
    increaseTicks();
    if (getTicks() > ptr->rightTicks()) {           //to prevent objects that moved to the next active spot from
        decreaseTicks();                            //being called again
        return;
    }
    
    
    if (getHealth() <= 0) {                             //todo: move this check into insect
        //put 100 points of food on the field
        setMovedTrue();
        setDead();
        ptr->addFood(getX(), getY(), 100);
        return;
    }
    
    if (isStunned()) {
        decrementStun();
        return;
    }
    
    interpreter();
    if (ptr->findPoison(getX(), getY(), this) && this->isDead())
        ptr->addFood(getX(), getY(), 100);
    if (ptr->findWater(getX(), getY(), this) && this->isDead())
        ptr->addFood(getX(), getY(), 100);
}

Ant::~Ant() {
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////           GRASSHOPPER            ///////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

Grasshopper::Grasshopper(int imageID, int startX, int startY, StudentWorld* ptr, int energyUnits)
: Insect(imageID, startX, startY, ptr, energyUnits)
{
    m_sleepTime=0;
    justAte = false;
    m_distance=0;
}
int Grasshopper::getDistance() const {return m_distance;};
void Grasshopper::setDistance(){m_distance = randInt(2, 10); return;};
void Grasshopper::decrementDistance(){m_distance--;};
int Grasshopper::getSleepTime() const {return m_sleepTime;};
void Grasshopper::decreaseSleepTime(){m_sleepTime--; return;};
void Grasshopper::resetSleepTime(){m_sleepTime = 2; return;};
void Grasshopper::resetSleepRightorUp() {m_sleepTime = 3;}
Grasshopper::~Grasshopper(){}
void Grasshopper::setAte(bool didIt){justAte = didIt;}
bool Grasshopper::getAte(){return justAte;}
void Grasshopper::setAteSleep() {m_sleepTime = 4; return;}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////                ADULT GRASSHOPPER               ////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

AdultGrasshopper::AdultGrasshopper(int startX, int startY, StudentWorld* ptr)
: Grasshopper(IID_ADULT_GRASSHOPPER, startX, startY, ptr, 1600) {
    
}

bool AdultGrasshopper::jump() {
    StudentWorld* ptr = getPtr();
    
    int degree = randInt(1, 360);
    double radian = (M_PI * degree)/180;
    int radius = randInt(2, 10);
    Direction facing;
    if (degree < 135 && degree >= 45) {
        facing = up;
    }
    else if (degree < 225 && degree >= 135) {
        facing = left;
    }
    else if (degree < 315 && degree >= 225) {
        facing = down;
    }
    else if (degree < 45 || degree >= 315) {
        facing = right;
    }
    int vertical = radius * sin(radian);
    int horizontal = radius * cos(radian);
    if ((getX() + horizontal) > 62 || (getX() + horizontal) < 1)    //has to be + in second statement bc always adding (pos and neg cos() change the direction)
        return false;
    if ((getY()+vertical) > 62 || (getY() + vertical) < 1)
        return false;
    if (ptr->checkJump(getX()+horizontal, getY()+vertical)) {
        ptr->updateForJump(getX(), getY(), horizontal, vertical, this);
        moveTo(getX()+horizontal, getY()+vertical);
    }
    return true;
}

void AdultGrasshopper::bite(Actor* target) {
    target->setHealth(-50);
    if (target->getHealth() <= 0)
        target->setDead();
    target->setBit(true);
}

bool AdultGrasshopper::canBiteBack() const {
    return true;
}

void AdultGrasshopper::doSomething() {
    setHealth(-1);
    StudentWorld* ptr = getPtr();
                                            //todo: initialize this earlier
    increaseTicks();
    if (getTicks() > ptr->rightTicks()) {           //to prevent objects that moved to the next active spot from
        decreaseTicks();                            //being called again
        return;
    }
    
    if (getHealth() <= 0) {                             //todo: move this check into insect
        //put 100 points of food on the field
        setDead();
        ptr->addFood(getX(), getY(), 100);
        return;
    }
    
    if (didMove())
        setMovedFalse();

    
    if (getSleepTime() > 0) {
        decreaseSleepTime();                //sometimes objects move into the next slot to be called so they get called twice
        return;                             //in the same tick
    }
    
    int shouldBite = randInt(0, 2);
    if (shouldBite == 1) {
        Actor* target = ptr->findInsect(getX(), getY(), this);
        if (target != nullptr && target->isInsect())
            bite(target);
    }
    
    int wantsToJump = randInt(1, 10);
    if (wantsToJump == 5) {
        for (int i = 0; i < 10; i++) {
            if (jump())
                break;;
        }
        if (getDirection() == right || getDirection() == up) {
            resetSleepRightorUp();
            return;
        }
        else 
            resetSleepTime();
        setMovedFalse();
        return;
    }
    
    if (ptr->findFood(getX(), getY(), this, 200))
        setAte(true);
    
    
    int wantsToSleep = randInt(0, 1);           //50% chance it wants to sleep
    if (getAte() && wantsToSleep) {                 //if wantsToSleep is 0, it won't run this
        setAteSleep();
        setAte(false);
        return;                         //todo: get this to work
    }
    setAte(false);

    if (getDistance() == 0) {
        Direction dirs = static_cast<GraphObject::Direction>(randInt(1, 4));
        setDirection(dirs);
        setDistance();
    }
    
    if (!ptr->isValidMove(getX(), getY(), getDirection(), this)) {
        setDistance();
        setDirection(static_cast<GraphObject::Direction>(randInt(1, 4)));
    }

    
    switch(getDirection()) {
        case right:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX()+1, this->getY());
                decrementDistance();
                setMovedTrue();
                resetSleepRightorUp();
                //resetSleepTime();
            }
            break;
        case down:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX(), this->getY()-1);
                decrementDistance();
                setMovedTrue();
                resetSleepTime();
            }
            break;
        case left:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX()-1, this->getY());
                decrementDistance();
                setMovedTrue();
                resetSleepTime();
            }
            break;
        case up:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX(), this->getY()+1);
                decrementDistance();
                setMovedTrue();
                resetSleepRightorUp();
                //resetSleepTime();
            }
            break;
        default:
            setMovedFalse();
            resetSleepTime();
            break;
    }

    
    return;
}


bool AdultGrasshopper::hasHealth() const {return true;}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////                 BABY GRASSHOPPER                 /////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

BabyGrasshopper::BabyGrasshopper(int startX, int startY, StudentWorld* ptr)
: Grasshopper(IID_BABY_GRASSHOPPER, startX, startY, ptr, 500)
{
    
}
void BabyGrasshopper::bite(Actor* target){target->setHealth(-50); return;}
bool BabyGrasshopper::hasHealth() const {return true;}

void BabyGrasshopper::doSomething() {
    setHealth(-1);      //decrease health one per tick
    
    
    StudentWorld* ptr = getPtr();                       //todo: initialize this earlier
    increaseTicks();
    if (getTicks() > ptr->rightTicks()) {           //to prevent objects that moved to the next active spot from
        decreaseTicks();                            //being called again
        return;
    }
    
    if (isDead())
        return;
    
    if (getHealth() <= 0) {                             //todo: move this check into insect 
        //put 100 points of food on the field
        //setVisible(false);
        setMovedTrue();
        setDead();
        ptr->addFood(getX(), getY(), 100);
        //ptr->getRidOf(getX(), getY(), this);
        return;
    }
    
    if (didMove())
        setMovedFalse();
    
    if (isStunned()) {
        decrementStun();
        return;
    }

    
    if (getSleepTime() > 0) {
        decreaseSleepTime();                //sometimes objects move into the next slot to be called so they get called twice
        return;                             //in the same tick
    }
    
    
    //if program gets to here, then the grasshopper will do something
    
    
    if (getHealth() >= 1600) {
        setHealth(-getHealth());
        setDead();
        setMovedFalse();
        ptr->addFood(getX(), getY(), 100);           //drop 100 points of food,
        ptr->evolve(getX(), getY(), this);      //create new adult grasshopper,
        
        
        return;
    }

    
    if (ptr->findFood(getX(), getY(), this, 200))
        setAte(true);

    
    int wantsToSleep = randInt(0, 1);           //50% chance it wants to sleep
    if (getAte() && wantsToSleep) {                 //if wantsToSleep is 0, it won't run this
        setAteSleep();
        setAte(false);
        return;                         //todo: get this to work
    }
    setAte(false);
    
    
    if (getDistance() == 0) {
        Direction dirs = static_cast<GraphObject::Direction>(randInt(1, 4));
        setDirection(dirs);
        setDistance();
    }
    
    if (!ptr->isValidMove(getX(), getY(), getDirection(), this)) {
        setDistance();
        setDirection(static_cast<GraphObject::Direction>(randInt(1, 4)));
    }
    
    
    //static_cast<GraphObject::Direction>(rand() % 4 + 1);
    
    
    switch(getDirection()) {
        case right:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX()+1, this->getY());
                decrementDistance();
                setMovedTrue();
            }
            break;
        case down:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX(), this->getY()-1);
                decrementDistance();
                setMovedTrue();
            }
            break;
        case left:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX()-1, this->getY());
                decrementDistance();
                setMovedTrue();
            }
            break;
        case up:
            if (ptr->isValidMove(getX(), getY(), getDirection(), this)) {
                moveTo(this->getX(), this->getY()+1);
                decrementDistance();
                setMovedTrue();
            }
            break;
        default:
            break;
    }
    if (ptr->findPoison(getX(), getY(), this) && this->isDead())
        ptr->addFood(getX(), getY(), 100);
    if (ptr->findWater(getX(), getY(), this) && this->isDead())
        ptr->addFood(getX(), getY(), 100);

    resetSleepTime();
    
    return;
}





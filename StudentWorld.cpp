

#include "StudentWorld.h"
#include <string>
#include <algorithm>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


StudentWorld::StudentWorld(std::string assetDir)
: GameWorld(assetDir)
{
    ticks = 0;
    for (int i = 0; i < 4; i++){
        numOfAnts[i] = 0;
    }
    winningNumber = 0;
}

int StudentWorld::init()
{
    Field f;
    std::string fieldFile = getFieldFilename();
    
    if (f.loadField(fieldFile) != Field::LoadResult::load_success)
        return false; // something bad happened!
    // otherwise the load was successful and you can access the
    // contents of the field – here’s an example
    
    Compiler *compilerForEntrant0 = nullptr;
    Compiler *compilerForEntrant1 = nullptr;
    Compiler *compilerForEntrant2 = nullptr;
    Compiler *compilerForEntrant3 = nullptr;
    //Anthill *ah0, *ah1, *ah2, *ah3;
    // get the names of all of the ant program source files
    // we provide the getFilenamesOfAntPrograms() function for
    // you in our GameWorld class.
    
    std::vector<std::string> fileNames = getFilenamesOfAntPrograms();
    std::string error;
    
    
    for (int k = 0; k < fileNames.size(); k++) {
        if (k == 0) {
            compilerForEntrant0 = new Compiler;
            compilers.push_back(compilerForEntrant0);
            if (!compilers[k]->compile(fileNames[k], error) ) {
                setError(colonynames[k] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            colonynames.push_back(compilerForEntrant0->getColonyName());
        }
        else if (k == 1) {
            compilerForEntrant1 = new Compiler;
            compilers.push_back(compilerForEntrant1);
            if (!compilers[k]->compile(fileNames[k], error) ) {
                setError(colonynames[k] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            colonynames.push_back(compilerForEntrant1->getColonyName());
        }
        else if (k == 2) {
            compilerForEntrant2 = new Compiler;
            compilers.push_back(compilerForEntrant2);
            if (!compilers[k]->compile(fileNames[k], error) ) {
                setError(colonynames[k] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            colonynames.push_back(compilerForEntrant2->getColonyName());
        }
        else if (k == 3) {
            compilerForEntrant3 = new Compiler;
            compilers.push_back(compilerForEntrant3);
            if (!compilers[k]->compile(fileNames[k], error) ) {
                setError(colonynames[k] + " " + error);
                return GWSTATUS_LEVEL_ERROR;
            }
            colonynames.push_back(compilerForEntrant3->getColonyName());
        }

    }
    
    if (colonynames.size() == 0)
        return GWSTATUS_LEVEL_ERROR;
     
    
    int x = 0;
    int y = 0;
    Field::FieldItem item;
    for (y=0; y < 64; y++) {
        for (x=0; x < 64; x++) {
            item = f.getContentsOf(x,y); // note it’s x,y and not y,x!!!
            /*
            if (x==2 && y==2) {
                Ant* ant0 = new Ant(IID_ANT_TYPE0, x, y, this, 0, compilerForEntrant0);
                actorVec[y][x].push_back(ant0);
            }
            */
            if (item == Field::FieldItem::empty) {
                continue;
            }
            
            else if (item == Field::FieldItem::rock) {
                Actor* rock = new Pebble(x,y, this);
                actorVec[y][x].push_back(rock);
            }
            
            else if (item == Field::FieldItem::grasshopper) {
                Actor* grasshopper = new BabyGrasshopper(x, y, this);
                actorVec[y][x].push_back(grasshopper);
            }
            
            
            else if (item == Field::FieldItem::anthill0) {
                Actor* anthill0 = new Anthill(IID_ANT_HILL, x, y, this, 0, compilerForEntrant0);
                actorVec[y][x].push_back(anthill0);
            }
            
            else if (item == Field::FieldItem::anthill1) {
                Actor* anthill1 = new Anthill(IID_ANT_HILL, x, y, this, 1, compilerForEntrant1);
                actorVec[y][x].push_back(anthill1);
            }
            
            else if (item == Field::FieldItem::anthill2) {
                Actor* anthill2 = new Anthill(IID_ANT_HILL, x, y, this, 2, compilerForEntrant2);
                actorVec[y][x].push_back(anthill2);
            }
            
            else if (item == Field::FieldItem::anthill3) {
                Actor* anthill3 = new Anthill(IID_ANT_HILL, x, y, this, 3, compilerForEntrant3);
                actorVec[y][x].push_back(anthill3);
            }
            
            else if (item == Field::FieldItem::food) {
                Actor* food = new Food(x, y, this, 6000);
                actorVec[y][x].push_back(food);
            }
            
            else if (item == Field::FieldItem::poison) {
                Actor* poison = new Poison(x, y, this);
                actorVec[y][x].push_back(poison);
            }
            
            else if (item == Field::FieldItem::water) {
                Actor* water = new Pool(x, y, this);
                actorVec[y][x].push_back(water);
            }
            
            else
                return GWSTATUS_LEVEL_ERROR;
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}


void StudentWorld::setDisplayText() {
    
    winningNumber = getWinning(numOfAnts, colonynames.size());
    
    std::ostringstream oss;
    oss << "Ticks:" << std::setw(5) << 2000 - ticks;
    if (colonynames.size() > 0) {
        oss << " -  ";
    }
    for (int i = 0; i < colonynames.size(); i++) {
        if (i != 0) {
            oss << "  ";
        }
        oss << colonynames[i];
        if (i == winningNumber) {
            oss << "*";
        }
        oss << ": ";
        oss << setw(2) << setfill('0') << numOfAnts[i];
    }
    setGameStatText(oss.str());
    
    return;
    
}


int StudentWorld::getWinning(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        if (numOfAnts[i] > numOfAnts[winningNumber]) {
            winningNumber = i;
        }
    }
    return winningNumber;
}



int StudentWorld::move() {
    
    setDisplayText();
    
    ticks++;
    
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            long sizeOfVec = actorVec[y][x].size();
            if (!actorVec[y][x].empty()) {
                int k = 0;
                std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
                while (k < sizeOfVec) {
                    if (!actorVec[y][x][k]->isDead())
                        (actorVec[y][x][k])->doSomething();
                    replaceWith((actorVec[y][x][k])->getDirection(), (actorVec[y][x][k])->getX(), (actorVec[y][x][k])->getY(), (actorVec[y][x][k]));
                    k++;
                    actorIterator++;
                    int prevSize = sizeOfVec;
                    sizeOfVec = actorVec[y][x].size();      //updates size incase something dies
                    if (prevSize > sizeOfVec)
                        k--;
                }
            }
        }
    }
    
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            long sizeOfVec = actorVec[y][x].size();
            if (!actorVec[y][x].empty()) {
                //std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
                int k = 0;
                while (k < sizeOfVec) {
                    if (actorVec[y][x][k]->isDead() || actorVec[y][x][k]->getHealth() <= 0) {      //bad access??
                        getRidOf((actorVec[y][x][k])->getX(), (actorVec[y][x][k])->getY(), (actorVec[y][x][k]), (actorVec[y][x][k])->getDirection());
                        //actorIterator--;
                        k--;
                        sizeOfVec = actorVec[y][x].size();
                    }
                    //actorIterator++;
                    k++;
                }
                
            }
        }
    }
    if (ticks == 2000) {
        int losers = 0;
        for (int i = 0; i < colonynames.size(); i++) {
            if (numOfAnts[i] <= 5)
                losers++;
        }
        if (losers == colonynames.size())
            winningNumber = -1;
    }
    
    if (ticks == 2000 && winningNumber >= 0) {
        GameWorld::setWinner(colonynames[winningNumber]);
        return GWSTATUS_PLAYER_WON;
    }
    else if (ticks == 2000 && winningNumber < 0)
        return GWSTATUS_NO_WINNER;
    else
        return GWSTATUS_CONTINUE_GAME;
}


bool StudentWorld::isValidMove(int x, int y, Actor::Direction dir, Actor* insect) {
    /*
    int k = 0;
    while (k != actorVec[y][x].size()) {
        if (actorVec[y][x][k] == insect) {
            break;
        }
        k++;
    }                           //todo: if time is left, change this to be like checkJump()
     */
    switch (dir) {
        case GraphObject::right:
            if (!actorVec[y][x+1].empty() && actorVec[y][x+1].front()->isPebble())
                return false;
            break;
        case GraphObject::up:
            if (!actorVec[y+1][x].empty() && actorVec[y+1][x].front()->isPebble())
                return false;
            break;
        case GraphObject::down:
            if (!actorVec[y-1][x].empty() && actorVec[y-1][x].front()->isPebble())
                return false;
            break;
        case GraphObject::left:
            if (!actorVec[y][x-1].empty() && actorVec[y][x-1].front()->isPebble())
                return false;
            break;
        default:
            break;
    }
    return true;
}



void StudentWorld::replaceWith(Actor::Direction dir, int origX, int origY, Actor* moveMe) {


    if(!moveMe->isPebble() && moveMe->didMove()) {         //bad access??
        switch (dir) {
            case GraphObject::right:
                origX--;
                break;
            case GraphObject::left:
                origX++;
                break;
            case GraphObject::up:
                origY--;
                break;
            case GraphObject::down:
                origY++;
                break;
            default:
                break;
        }
    }
    
    if (!moveMe->isInsect())
        return;
    
    int k = 0;
    std::vector<Actor*> :: iterator actorIterator = actorVec[origY][origX].begin();
    bool stop = false;
    while (k != actorVec[origY][origX].size()) {
        if (actorVec[origY][origX][k] == moveMe) {
            stop = true;
            break;
        }
        k++;
        actorIterator++;
    }
    
    if (!stop)
        return;
    
    if (actorIterator == actorVec[origY][origX].end())
        return;
    
    if (actorVec[origY][origX].empty())
        return;
    
    if (!(moveMe->isInsect()))
        if (!moveMe->isFood())
            return;
    
    
    
    EnergyHolder* p = static_cast<EnergyHolder*>(*actorIterator);  
    
    if (p != nullptr && (p->isDead() || ((p)->getHealth()) <= 0)) {
        delete p;
        p = nullptr;
        actorIterator = actorVec[origY][origX].erase(actorIterator);
        return;
    }

    if (actorVec[origY][origX].empty() || (*actorIterator)->isFood()) {
        return;
    }
    

    
    switch (dir) {
        case GraphObject::right:
            if ((*actorIterator)->didMove()) {
                actorVec[origY][origX+1].push_back(*actorIterator);
                actorIterator = actorVec[origY][origX].erase(actorIterator);
            }
            break;
        case GraphObject::left:
            if ((*actorIterator)->didMove()) {
                actorVec[origY][origX-1].push_back(*actorIterator);
                actorIterator = actorVec[origY][origX].erase(actorIterator);
            }
            break;
        case GraphObject::up:
            if ((*actorIterator)->didMove()) {
                actorVec[origY+1][origX].push_back(*actorIterator);
                actorIterator = actorVec[origY][origX].erase(actorIterator);
                
            }
            break;
        case GraphObject::down:
            if ((*actorIterator)->didMove()) {
                actorVec[origY-1][origX].push_back(*actorIterator);
                actorIterator = actorVec[origY][origX].erase(actorIterator);
            }
            break;
        default:
            break;
    }
    
}


void StudentWorld::updateForJump(int x, int y, int horizontal, int vertical, AdultGrasshopper* grass) {
    std::vector<Actor*> :: iterator actorIterator = find(actorVec[y][x].begin(), actorVec[y][x].end(), grass);
    actorVec[y+vertical][x+horizontal].push_back(grass);
    actorIterator = actorVec[y][x].erase(actorIterator);
}



int StudentWorld::rightTicks() const {
    return ticks;
}


void StudentWorld::cleanUp()
{
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 64; j++) {
            long sizeOfVec = actorVec[i][j].size();
            if (!actorVec[i][j].empty()) {
                int k = 0;
                std::vector<Actor*> :: iterator actorIterator = actorVec[i][j].begin();
                while (k < sizeOfVec) {
                    delete actorVec[i][j][k];
                    actorVec[i][j][k] = nullptr;
                    actorIterator = actorVec[i][j].erase(actorIterator);
                    k++;
                    int prevSize = sizeOfVec;
                    sizeOfVec = actorVec[i][j].size();      //updates size incase something dies
                    if (prevSize > sizeOfVec)
                        k--;
                }
            }
        }
    }
    
    for (int i = 0; i < compilers.size(); i++) {
        delete compilers[i];
    }
}

void StudentWorld::evolve(int x, int y, Insect* old) {
    
    Actor* grass = new AdultGrasshopper(x, y, this);
    actorVec[y][x].push_back(grass);
    
}


void StudentWorld::addFood(int x, int y, int amt) {
    //std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    int k = 0;
    while (k != actorVec[y][x].size()) {
        if ((actorVec[y][x][k])->isFood()) {
            break;
        }
        k++;
    }
    
    if (k < actorVec[y][x].size() && (actorVec[y][x][k])->isFood()) {
        (actorVec[y][x][k])->setHealth(amt);
    }
    else {
        Actor* food = new Food(x, y, this, amt);
        actorVec[y][x].push_back(food);
    }
    
}

bool StudentWorld::findPoison(int x, int y, Insect* insect) {
    int k = 0;
    std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    while (k < actorVec[y][x].size()) {
        if (!actorVec[y][x].empty() && actorVec[y][x][k]->isPoison()) {
            insect->setHealth(-150);
            if (insect->getHealth() <= 0)
                insect->setDead();
            return true;
        }
        actorIterator++;
        k++;
    }
    return false;
}

bool StudentWorld::findWater(int x, int y, Insect* insect) {
    int k = 0;
    std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    while (k < actorVec[y][x].size()) {
        if (!actorVec[y][x].empty() && actorVec[y][x][k]->isWater()) {
            insect->resetStun();
            return true;
        }
        actorIterator++;
        k++;
    }
    return false;
}



bool StudentWorld::checkJump(int x, int y) {
    if (!actorVec[y][x].empty() && actorVec[y][x].front()->isPebble()) {
        return false;
    }
    return true;
}



bool StudentWorld::findFood(int x, int y, EnergyHolder* energyholder, int amt) {
    
    int k = 0;
    std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    while (!actorVec[y][x].empty() && actorIterator != actorVec[y][x].end()-1 && !(*actorIterator)->isFood()) {
        actorIterator++;
        k++;
    }
    
    if (energyholder->isAnt() && !actorVec[y][x].empty() && actorVec[y][x][k]->isFood()) {
        if (actorVec[y][x][k]->getHealth() <= amt && !actorVec[y][x][k]->isDead()) {
            energyholder->setFoodHeld(actorVec[y][x][k]->getHealth());
            actorVec[y][x][k]->setHealth(-5000);        //arbitrarily low
            actorVec[y][x][k]->setDead();
        }
        else if(actorVec[y][x][k]->getHealth() <= amt && !actorVec[y][x][k]->isDead() && energyholder->foodHeld()>0) {
            int add = amt - energyholder->foodHeld();
            energyholder->setFoodHeld(add);
            actorVec[y][x][k]->setHealth(-add);        //arbitrarily low
            if (actorVec[y][x][k]->getHealth() < 0)
                actorVec[y][x][k]->setDead();
        }
        else if (!actorVec[y][x][k]->isDead()) {
            actorVec[y][x][k]->setHealth(-amt);
            energyholder->setFoodHeld(amt);
        }
        
        return true;
        
    }
    
    else if (!energyholder->isAnt() && !actorVec[y][x].empty() && actorVec[y][x][k]->isFood()) {
        if (actorVec[y][x][k]->getHealth() <= amt && !actorVec[y][x][k]->isDead()) {
            energyholder->setHealth(actorVec[y][x][k]->getHealth());
            actorVec[y][x][k]->setHealth(-5000);        //arbitrarily low
            actorVec[y][x][k]->setDead();
        }
        else if (!actorVec[y][x][k]->isDead()) {
            actorVec[y][x][k]->setHealth(-amt);
            energyholder->setHealth(amt);
        }
        
        return true;
    }
    return false;
}



void StudentWorld::addAnts(int x, int y, int colonyNumber, Compiler* cmp) {
    if (colonyNumber < 0 || colonyNumber > 3)
        return;
    
    if (colonyNumber == 0) {
        Ant* ant0 = new Ant(IID_ANT_TYPE0, x, y, this, colonyNumber, cmp);
        actorVec[y][x].push_back(ant0);
        numOfAnts[colonyNumber]++;
        //winningNumber = getWinning(numOfAnts, colonynames.size());
    }
    else if (colonyNumber == 1) {
        Ant* ant1 = new Ant(IID_ANT_TYPE1, x, y, this, colonyNumber, cmp);
        actorVec[y][x].push_back(ant1);
        numOfAnts[colonyNumber]++;
        //winningNumber = getWinning(numOfAnts, colonynames.size());
    }
    else if (colonyNumber == 2) {
        Ant* ant2 = new Ant(IID_ANT_TYPE2, x, y, this, colonyNumber, cmp);
        actorVec[y][x].push_back(ant2);
        numOfAnts[colonyNumber]++;
        //winningNumber = getWinning(numOfAnts, colonynames.size());
    }
    else if (colonyNumber == 3) {
        Ant* ant3 = new Ant(IID_ANT_TYPE3, x, y, this, colonyNumber, cmp);
        actorVec[y][x].push_back(ant3);
        numOfAnts[colonyNumber]++;
        //winningNumber = getWinning(numOfAnts, colonynames.size());
    }
}





Actor* StudentWorld::findInsect(int x, int y, Insect* insect) {
    int pos = 0;
    std::vector<int> targets;
    if (actorVec[y][x].empty())
        return nullptr;
    std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    while (actorIterator != actorVec[y][x].end()) {
        if ((*actorIterator)->isInsect()) {
            if ((*actorIterator) == insect) {
                pos++;
                actorIterator++;
                continue;                       //todo: change this function so that if a grasshopper is bitten, it bites back,
            }                                   //and if another grasshopper gets bitten then it bites back etc.
            if ((*actorIterator)->isAnt() && (*actorIterator)->getColony() > 0) {
                if ((*actorIterator)->getColony() == insect->getColony()) {
                    pos++;
                    actorIterator++;
                    continue;
                }
            }
            targets.push_back(pos);
        }
        pos++;
        actorIterator++;
    }
    if (targets.empty())
        return nullptr;
    int posoftarget = randInt(0, targets.size()-1);
    pos = targets[posoftarget];
    if (actorVec[y][x][pos]->isAnt()) {
        actorVec[y][x][pos]->setBit(true);
    }
    
    if (actorVec[y][x][pos]->canBiteBack()) {
        int biteBack = randInt(0, 2);                                   //doesn't account for
        if (biteBack == 1) {                                        //repeated bite backs
            int secondaryTarget = randInt(0, targets.size()-1);
            actorVec[y][x][secondaryTarget]->setHealth(-50);
        }
        else {
            insect->setHealth(-50);         //need this here because insect not included in vector
        }
    }
    
    return actorVec[y][x][pos];
}





void StudentWorld::getRidOf(int x, int y, Actor* end, Actor::Direction dir) {
    
    std::vector<Actor*> :: iterator actorIterator = find(actorVec[y][x].begin(), actorVec[y][x].end(), end);
    //actorIterator = find(actorVec[y][x].begin(), actorVec[y][x].end(), end);
    if (actorIterator != actorVec[y][x].end() && (end->isDead() || end->getHealth() <= 0)) {
        delete end;
        end = nullptr;
        actorIterator = actorVec[y][x].erase(actorIterator);
        return;
    }
}



void StudentWorld::addPheromone(int x, int y, int colonyNumber, int amt) {
    
    int k = 0;
    while (k != actorVec[y][x].size()) {
        if ((actorVec[y][x][k])->isPheromone()) {
            break;
        }
        k++;
    }
    
    if (k < actorVec[y][x].size() && (actorVec[y][x][k])->isPheromone() && (actorVec[y][x][k])->getColony() == colonyNumber) {
        (actorVec[y][x][k])->setHealth(amt);
    }
    else {
        if (colonyNumber == 0) {
            Pheromone* pheromone0 = new Pheromone(IID_PHEROMONE_TYPE0, x, y, colonyNumber, this);
            actorVec[y][x].push_back(pheromone0);
        }
        else if (colonyNumber == 1) {
            Pheromone* pheromone1 = new Pheromone(IID_PHEROMONE_TYPE1, x, y, colonyNumber, this);
            actorVec[y][x].push_back(pheromone1);
        }
        else if (colonyNumber == 2) {
            Pheromone* pheromone2 = new Pheromone(IID_PHEROMONE_TYPE2, x, y, colonyNumber, this);
            actorVec[y][x].push_back(pheromone2);
        }
        else if (colonyNumber == 3) {
            Pheromone* pheromone3 = new Pheromone(IID_PHEROMONE_TYPE3, x, y, colonyNumber, this);
            actorVec[y][x].push_back(pheromone3);
        }
    }

    
    
    
}



bool StudentWorld::findAnthill(int x, int y, int colonyNumber, Ant* ant) {
    /*int k = 0;
    std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    while (!actorVec[y][x].empty() && actorIterator != actorVec[y][x].end()-1 && !(*actorIterator)->isAnthill()) {
        actorIterator++;
        k++;
    }
    if (actorIterator != actorVec[y][x].end() ) {
        return true;
    }*/
    if (!actorVec[y][x].empty() && (*actorVec[y][x].begin())->isAnthill()) {
        if ((*actorVec[y][x].begin())->getColony() == ant->getColony()) {
            return true;
        }
    }
    return false;
}

bool StudentWorld::findPheromone(int x, int y, int colonyNumber, Ant* ant) {
    int k = 0;
    std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    while (!actorVec[y][x].empty() && actorIterator != actorVec[y][x].end()-1 && !(*actorIterator)->isPheromone()) {
        actorIterator++;
        k++;
    }
    if (actorIterator != actorVec[y][x].end() && (*actorIterator)->getHealth() > 0  && !(*actorIterator)->isPheromone()) {
        return true;
    }
    return false;
}


bool StudentWorld::smellDanger(int x, int y, int colonyNumber, Actor::Direction dir) {
    if (dir == Actor::up) {y++;}
    else if (dir == Actor::down) {y--;}
    else if (dir == Actor::right) {x++;}
    else if (dir == Actor::left) {x--;}
    int k = 0;
    std::vector<Actor*> :: iterator actorIterator = actorVec[y][x].begin();
    while (!actorVec[y][x].empty() && actorIterator != actorVec[y][x].end()-1) {
        if (actorVec[y][x][k]->isInsect() && actorVec[y][x][k]->getColony() != colonyNumber) {
            return true;
        }
        else if (actorVec[y][x][k]->isWater()) {
            return true;
        }
        else if (actorVec[y][x][k]->isPoison()) {
            return true;
        }
        actorIterator++;
        k++;
    }
    return false;
}

/*
void StudentWorld::setWinner() {
    if (num of ants at whatever position is lower than 5,
        if it is then return and don't do anything'
}
*/

StudentWorld::~StudentWorld() {
    std::vector<Actor*> :: iterator end;
    for (int y = 0; y < 64; y++) {
        for (int x = 0; x < 64; x++) {
            end = actorVec[y][x].begin();
            while (end != actorVec[y][x].end()) {
                if (*end != nullptr) {
                    delete *end;
                }
                end = actorVec[y][x].erase(end);
            }
        }
    }
}























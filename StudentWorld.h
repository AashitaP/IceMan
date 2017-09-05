#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "GameController.h"
#include <vector>
#include <string>
#include <algorithm>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir)
    : GameWorld(assetDir)
    {;}
    
    ~StudentWorld()
    {
        cleanUp();
    }
    
    virtual int init();
    
    bool removeIce(int x, int y);
    
    virtual int move();
    
    virtual void cleanUp();
    
    void squirtWater(int x, int y, GraphObject::Direction dir);
    
    void setPositions(int x, int y, char actortype);
    
    char getPositions(int x, int y )
    {
        return ActorPositions[x][y];
    }
    
    void illuminate(int x, int y);
    
    void dropGold(int x, int y)
    {
        Actors.push_back(new Gold(x, y, this, true, false)); //visible, not pickable by iceman, temporary state
    }
    
    bool isIce(int x, int y, GraphObject::Direction dir);
    
    bool isBoulder(int x, int y, GraphObject:: Direction dir);
    
    bool canFall(int x, int y);
    
    bool NearIceman(int x, int y, int amount);
    
    IceMan* Iceman_ptr()
    {
        return m_iceman;
    }
    
    void pickupBarrel(int x, int y);
    
    bool playerDied();
    
    bool finishedLevel();
    
    bool canShout(int x, int y, GraphObject::Direction dir);
    
    bool canFace(int x, int y, GraphObject::Direction& dir);
    
    bool noOverlap(int x, int y);
    
    void updateDisplayText();
    
    void removeDeadGameObjects();
    
    bool canAddProtester();
    
    bool canAddWater(int x, int y);
    
    Actor* findProtester(int x, int y);
    
    void boulderAnnoyActors(int x, int y);
    
    bool annoyProtester(int x, int y, int amt);
    
    bool noIcenoBoulder(int x, int y, GraphObject::Direction dir);
    
    int generateNewDir(int x, int y);
    
private:
    IceMan* m_iceman;
    int nIce;
    Ice* IcePointers[3616];
    std::vector<Actor*> Actors;
    int nBoulders;
    int nGold;
    int nBarrels;
    char ActorPositions[64][64];
    int pickedBarrels;
    int ticksSinceLastProtester;
    int ticksToWaitToAddProtester;
    int nProtesters;
};

#endif // STUDENTWORLD_H_

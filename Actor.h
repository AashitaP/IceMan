#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <queue>
class StudentWorld;


struct Point
{
    Point(int x, int y): m_x(x),m_y(y) {;}
    int m_x;
    int m_y;
};


class Actor:public GraphObject{
public:
    Actor(int imageID, int startX, int startY, StudentWorld* sw, Direction startDirection, float size, unsigned int depth):GraphObject(imageID, startX, startY, startDirection, size, depth)
    {
        m_studentworld = sw;
        m_alive = true;
    }
    
    StudentWorld* getWorld()
    {
        return m_studentworld;
    }
    
    virtual void doSomething() = 0;
    
    bool isAlive()
    {
        return m_alive;
    }
    
    virtual void setDead()
    {
        m_alive = false;
    }
    
    virtual bool isProtester()
    {
        return false;
    }
    
    virtual void takeDamage(int amt){;}
    
    virtual void Bribed() {;}
    
    virtual std::string getState() {return "";}
    
    
    
private:
    StudentWorld* m_studentworld;
    bool m_alive;
    
};

class Goodies:public Actor{
public:
    
    Goodies(int imageID, int startX, int startY, StudentWorld* sw, Direction startDirection, float size, unsigned int depth);
    
    bool isVisible()
    {
        return m_visible;
    }
    
    void setVisible(bool visibility)
    {
        GraphObject::setVisible(visibility);
        m_visible = visibility;
    }
    
    void setPickup(bool canPick)
    {
        canPickupIceman = canPick;
    }
    
    
    bool isPickable()
    {
        return canPickupIceman;
    }
    
    
    int getTicks()
    {
        return tickSpan;
    }
    
    void reduceTicks()
    {
        tickSpan--;
    }
    
    void setTicks(int amt)
    {
        tickSpan = amt;
    }
private:
    bool m_visible;
    bool canPickupIceman;
    int tickSpan;
};


class Gold:public Goodies
{
public:
    Gold(int startX, int startY, StudentWorld* sw, bool isVisible, bool canPick):Goodies(IID_GOLD, startX, startY, sw, right, 1.0, 2)
    {
        setVisible(isVisible);
        setPickup(canPick);
        setTicks(100);
    }
    virtual void doSomething();
    
};


class Water:public Goodies{
public:
    Water(int startX, int startY, StudentWorld* sw):Goodies(IID_WATER_POOL, startX, startY, sw, right, 1.0, 2)
    {
        setVisible(true);
        setPickup(true);
    }
    
    virtual void doSomething();
    
};


class Oil:public Goodies{
    
public:
    Oil(int startX, int startY, StudentWorld* sw):Goodies(IID_BARREL, startX, startY, sw, right, 1.0, 2)
    {
        setVisible(false);
    }
    virtual void doSomething();
    
    
};

class Sonar:public Goodies{
public:
    Sonar(StudentWorld* sw):Goodies(IID_SONAR, 0, 60, sw, right, 1.0, 2)
    {
        setVisible(true);
        setPickup(true);
    }
    virtual void doSomething();
};

class Squirt:public Actor{
public:
    Squirt(int startX, int startY, StudentWorld* sw, Direction startDirection):Actor(IID_WATER_SPURT, startX, startY, sw, startDirection, 1.0, 1)
    {
        setVisible(true);
        m_distance = 0;
    }
    void doSomething();
    
private:
    int m_distance;
};


class Boulder:public Actor{
public:
    Boulder(int startX, int startY, StudentWorld* sw):Actor(IID_BOULDER, startX, startY, sw, down, 1.0, 1)
    {
        setVisible(true);
        m_state = 0;
    }
    
    virtual void doSomething();
    
private:
    int m_state; //0 is stable, 1 waiting, 2 is falling
    int m_tick;
};


class Ice:public Actor{
public:
    Ice(int startX, int startY, StudentWorld* sw): Actor(IID_ICE, startX, startY, sw, right, 0.25, 3)
    {
        setVisible(true);
    }
    
    virtual ~Ice()
    {;}
    
    virtual void doSomething() {;}
};

class AnnoyedActor:public Actor{
public:
    virtual void takeDamage(int amt)
    {
        m_hitpoints -= amt;
        if (m_hitpoints <= 0)
            giveUp();
    }
    
    int gethitPoints(){
        return m_hitpoints;
    }
    
    void sethitPoints(int hitpoints) {
        m_hitpoints = hitpoints;
    }
    
    virtual void setDead()
    {
        Actor::setDead();
        sethitPoints(0);
    }
    
    AnnoyedActor(int imageID, int startX, int startY, StudentWorld* sw, Direction startDirection, float size, unsigned int depth): Actor(imageID, startX, startY, sw, startDirection, size, depth)
    {
        setVisible(true);
    }
    
    virtual void giveUp() = 0;
    
private:
    int m_hitpoints;
};

class IceMan:public AnnoyedActor {
public:
    
    IceMan(int startX, int startY, StudentWorld* sw): AnnoyedActor(IID_PLAYER, startX, startY, sw, right, 1.0, 0)
    {
        sethitPoints(10);
        m_sonar = 1;
        m_gold = 0;
        m_water = 5;
    }
    
    virtual ~IceMan() {;}
    
    void addGold()
    {
        m_gold++;
    }
    
    void addKit()
    {
        m_sonar++;
    }
    
    void addWater()
    {
        m_water += 5;
    }
    
    int getSquirts()
    {
        return m_water;
    }
    
    int getGold()
    {
        return m_gold;
    }
    
    int getSonar()
    {
        return m_sonar;
    }
    
    virtual void doSomething();
    virtual void giveUp();
    
private:
    int m_sonar;
    int m_gold;
    int m_water;
};

class Protesters:public AnnoyedActor{
public:
    Protesters(int imageID, int startX, int startY, StudentWorld* sw, Direction startDirection, float size, unsigned int depth);
    
    int getRestingTicksLeft()
    {
        return restingTicksLeft;
    }
    
    void rest()
    {
        restingTicksLeft--;
    }
    
    bool findNearestPath(int startX, int startY, int finalX, int finalY, Direction& finalDir, int& steps);
    
    int getTicksSinceShout()
    {
        return nonRestingTicksSinceShouted;
    }
    
    void prepareNextMove()
    {
        nonRestingTicksSinceTurned++;
        nonRestingTicksSinceShouted++;
        restingTicksLeft = ticksToWait;
    }
    
    void resetSquaresToMoveInDir()
    {
        numSquaresToMoveInDir = 0;
    }
    
    void decreaseSquaresToMoveInDir()
    {
        numSquaresToMoveInDir--;
    }
    
    int getSquaresToMoveInDir()
    {
        return numSquaresToMoveInDir;
    }
    
    void pickNewSquaresToMoveInDir()
    {
        numSquaresToMoveInDir = rand()% 53 + 8;
    }
    
    int getTicksSinceTurned()
    {
        return nonRestingTicksSinceTurned;
    }
    
    void resertTicksSinceTurned()
    {
        nonRestingTicksSinceTurned=0;
    }
    virtual void giveUp();
    
    void Stunned();
    
    void setLeaveOilFieldState()
    {
        m_state = "leave-oil-field";
        restingTicksLeft = 0;
    }
    
    virtual bool isProtester()
    {
        return true;
    }
    
    virtual void takeDamage(int amt);
    
    void shout();
    
    virtual std::string getState()
    {
        return m_state;
    }
    
    virtual bool isHardcoreProtester()
    {
        return false;
    }
    
private:
    int ticksToWait;
    std::string m_state;
    int restingTicksLeft;
    int nonRestingTicksSinceShouted;
    int nonRestingTicksSinceTurned;
    int numSquaresToMoveInDir;
};

class RegularProtesters: public Protesters {
public:
    RegularProtesters(StudentWorld* sw): Protesters(IID_PROTESTER, 60, 60, sw, left, 1.0, 0)
    {
        sethitPoints(5);
    }
    
    virtual void doSomething();
    
    virtual void Bribed();
};

class HardcoreProtesters:public Protesters {
public:
    HardcoreProtesters(StudentWorld* sw): Protesters(IID_HARD_CORE_PROTESTER, 60, 60, sw, left, 1.0, 0)
    {
        sethitPoints(20);
    }
    virtual void doSomething();
    virtual bool isHardcoreProtester()
    {
        return true;
    }
    virtual void Bribed();
};



#endif // ACTOR_H_

#include "Actor.h"
#include "StudentWorld.h"
using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Goodies::Goodies(int imageID, int startX, int startY, StudentWorld* sw, Direction startDirection, float size, unsigned int depth):Actor(imageID, startX, startY, sw, startDirection, size, depth)
{
    tickSpan = max<unsigned int>(100, 300 - 10*(getWorld() -> getLevel()));
}


Protesters:: Protesters(int imageID, int startX, int startY, StudentWorld* sw, Direction startDirection, float size, unsigned int depth): AnnoyedActor(imageID, startX, startY, sw, startDirection, size, depth)
{
    ticksToWait = max<unsigned int>(0, 3 - (getWorld() -> getLevel())/4);
    restingTicksLeft = ticksToWait;
    nonRestingTicksSinceShouted = 15;
    nonRestingTicksSinceTurned = 0;
    m_state = "rest";
    numSquaresToMoveInDir = rand()% 53 + 8;
    
}


void IceMan::doSomething(){ //during a tick
    if (!isAlive()) //checking if its currently alive
        return;
    
    
    if(getWorld() -> removeIce(getX(), getY()))
        GameController::getInstance().playSound(SOUND_DIG);
    
    int ch;
    if (getWorld() -> getKey(ch) == true)
    {
        switch(ch)
        {
            case KEY_PRESS_ESCAPE:
                setDead();
                break;
            case KEY_PRESS_SPACE:
                if (m_water>0)
                {
                    if (getDirection() == right && getX() < 60)
                    {
                        getWorld() -> squirtWater(getX() + 4, getY(), right);
                        m_water--;
                    }
                    
                    else if (getDirection() == left && getX() > 0)
                    {
                        getWorld() -> squirtWater(getX() - 4, getY(), left);
                        m_water--;
                    }
                    
                    else if (getDirection() == up && getY() < 60)
                    {
                        getWorld() -> squirtWater(getX(), getY() + 4, up);
                        m_water--;
                    }
                    
                    else if (getDirection() == down && getY() > 0)
                    {
                        getWorld() -> squirtWater(getX(), getY() - 4, down);
                        m_water--;
                    }
                }
                break;
            case KEY_PRESS_UP:
                if (getDirection() != up)
                    setDirection(up);
                else if (getY() <= 59 && !getWorld() -> isBoulder(getX(), getY()+1, up))
                {
                    moveTo(getX(), getY() + 1);
                }
                else if(getY() == 60)
                {
                    moveTo(getX(), getY());
                }
                
                break;
            case KEY_PRESS_DOWN:
                if (getDirection() != down)
                    setDirection(down);
                else if (getY() >= 1 && !getWorld() -> isBoulder(getX(), getY()-1, down))
                {
                    moveTo(getX(), getY() - 1);
                }
                
                else if(getY() == 0)
                {
                    moveTo(getX(), getY());
                }
                break;
            case KEY_PRESS_LEFT:
                if (getDirection() != left)
                    setDirection(left);
                else if (getX() >= 1 && !getWorld() -> isBoulder(getX() - 1, getY(), left))
                {
                    moveTo(getX() - 1, getY());
                }
                else if(getX() == 0)
                {
                    moveTo(getX(), getY());
                }
                break;
            case KEY_PRESS_RIGHT:
                if (getDirection() != right)
                    setDirection(right);
                else if (getX() <= 59 && !getWorld() -> isBoulder(getX() +1, getY(), right))
                {
                    moveTo(getX() + 1, getY());
                }
                else if (getX() == 60)
                {
                    moveTo(getX(), getY());
                }
                
                break;
                
            case 'Z':
            case 'z':
                if(m_sonar>0)
                {
                    m_sonar--;
                    getWorld() -> illuminate(getX(), getY());
                }
                break;
            case KEY_PRESS_TAB:
                if (m_gold > 0)
                {
                    getWorld() -> dropGold(getX(), getY());
                    m_gold--;
                }
                break;
        }
    }
}

void IceMan::giveUp()
{
    setDead();
    GameController::getInstance().playSound(SOUND_PLAYER_GIVE_UP);
}

void Boulder::doSomething()
{
    if (!isAlive())
        return;
    
    if (m_state == 0)
    {
        if (!getWorld() -> isIce(getX(), getY() -1, down))
        {
            m_state = 1;
            m_tick = 0;
        }
        
    }
    
    else if (m_state == 1 && m_tick < 30)
        m_tick++;
    
    else if (m_state == 1 && m_tick  == 30)
    {
        m_state = 2;
        GameController::getInstance().playSound(SOUND_FALLING_ROCK);
    }
    
    else if (m_state == 2)
    {
        if(getWorld() -> canFall(getX(), getY() - 1))
        {
            getWorld() -> setPositions(getX(), getY(), 0);
            moveTo(getX(), getY() - 1);
            getWorld() -> boulderAnnoyActors(getX(), getY());
            getWorld() -> setPositions(getX(), getY(), 'B');
        }
        
        else if(!getWorld() -> canFall(getX(), getY() - 1))
            setDead();
    }
    
}

void Squirt::doSomething()
{
    if (getWorld() -> annoyProtester(getX(), getY(), 2))
    {
        setDead();
    }
    
    
    if (m_distance == 4)
        setDead();
    
    else if (getDirection() == right)
    {
        if (!getWorld() -> noIcenoBoulder(getX() + 1, getY(), right)|| getX() >= 60)
            setDead();
        else
        {
            moveTo(getX() + 1, getY());
            m_distance++;
        }
    }
    
    else if (getDirection() == left)
    {
        if (!getWorld() -> noIcenoBoulder(getX() -1, getY(), left)|| getX()<= 0)
            setDead();
        else
        {
            moveTo(getX() - 1, getY());
            m_distance++;
        }
    }
    
    else if (getDirection() == up)
    {
        if (!getWorld() -> noIcenoBoulder(getX(), getY() + 1, up) || getY() >= 60)
            setDead();
        else
        {
            moveTo(getX(), getY() + 1);
            m_distance++;
        }
    }
    
    else if (getDirection() == down)
    {
        if (!getWorld() -> noIcenoBoulder(getX(), getY() - 1, down) || getY() <= 0)
            setDead();
        else
        {
            moveTo(getX(), getY() -1);
            m_distance++;
        }
    }
    
}

void Oil::doSomething()
{
    if (!isAlive())
        return;
    
    if (!isVisible() && getWorld() -> NearIceman(getX(), getY(), 4))
    {
        setVisible(true);
        return;
    }
    
    else if (getWorld() -> NearIceman(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_FOUND_OIL);
        getWorld() -> increaseScore(1000);
        getWorld() -> pickupBarrel(getX(), getY());
    }
    
}

void Gold::doSomething()
{
    if (!isAlive())
        return;
    if (!isVisible() && getWorld() -> NearIceman(getX(), getY(), 4))
    {
        setVisible(true);
        return;
    }
    
    if (isPickable() && getWorld() -> NearIceman(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        getWorld() -> increaseScore(10);
        getWorld() -> Iceman_ptr() -> addGold();
    }
    
    else if (!isPickable())
    {
        Actor* actor = getWorld() -> findProtester(getX(), getY());
        if(actor == nullptr)
        {
            if(getTicks() == 0)
                setDead();
            else
                reduceTicks();
        }
        else
        {
            setDead();
            actor -> Bribed();
        }
        
    }//protestor stuff
    
    
}

void Sonar::doSomething()
{
    if (!isAlive())
        return;
    if(getWorld()-> NearIceman(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        getWorld() -> increaseScore(75);
        getWorld() ->Iceman_ptr() -> addKit();
        
    }
    
    else if (getTicks() == 0)
        setDead();
    
    reduceTicks();
}

void Water::doSomething()
{
    if (!isAlive())
        return;
    if (getWorld() -> NearIceman(getX(), getY(), 3))
    {
        setDead();
        GameController::getInstance().playSound(SOUND_GOT_GOODIE);
        getWorld() -> increaseScore(100);
        getWorld() ->Iceman_ptr() -> addWater();
    }
    
    else if (getTicks() == 0)
        setDead();
    
    reduceTicks();
}


void RegularProtesters::doSomething()
{
    if (!isAlive())
        return;
    
    if(getRestingTicksLeft()  > 0)
    {
        rest();
        return;
    }
    
    if(getState() == "leave-oil-field")
    {
        if(getX() == 60 && getY() == 60)
            setDead();
        Direction dir = none;
        int steps;
        if(findNearestPath(60, 60, getX(), getY(),dir, steps))
        {
            if (dir == up)
            {
                setDirection(up);
                moveTo(getX(), getY() + 1);
            }
            else if(dir == down)
            {
                setDirection(down);
                moveTo(getX(), getY() - 1);
            }
            else if(dir == left)
            {
                setDirection(left);
                moveTo(getX() - 1, getY());
            }
            else if(dir == right)
            {
                setDirection(right);
                moveTo(getX() + 1, getY());
            }
            
        }
        
        prepareNextMove();
        return;
    }
    
    if(getWorld() -> canShout(getX(), getY(), getDirection()))
    {
        if(getTicksSinceShout() > 15)
        {
            shout();
            prepareNextMove();
            return;
        }
        
        else if (getTicksSinceShout() <= 15)
        {
            prepareNextMove();
            return;
        }
    }
    
    Direction dir;
    if(getWorld() -> canFace(getX(), getY(), dir))
    {
        setDirection(dir);
        if (dir == up)
            moveTo(getX(), getY() + 1);
        else if(dir == down)
            moveTo(getX(), getY() - 1);
        else if(dir == left)
            moveTo(getX() - 1, getY());
        else if(dir == right)
            moveTo(getX() + 1, getY());
        
        resetSquaresToMoveInDir();
        prepareNextMove();
        
        return;
    }
    
    
    if(!getWorld() -> canFace(getX(), getY(), dir))
    {
        decreaseSquaresToMoveInDir();
    }
    
    if(getSquaresToMoveInDir() <= 0)
    {
        int newDir = getWorld()->generateNewDir(getX(), getY());
        setDirection((Direction) newDir);
        
        pickNewSquaresToMoveInDir();
    }
    
    else if(getTicksSinceTurned() > 200)
    {
        if (getDirection() == right || getDirection() == left)
        {
            if(getWorld() -> noIcenoBoulder(getX(), getY() + 1, up) && getY() < 60)
            {
                setDirection(up);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
            
            else if(getWorld() -> noIcenoBoulder(getX(), getY() -1, down) && getY() > 0)
            {
                setDirection(down);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
        }
        
        else if(getDirection() == up || getDirection() == down)
        {
            if(getWorld() -> noIcenoBoulder(getX() -1, getY(), left) && getX() > 0)
            {
                setDirection(left);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
            
            else if(getWorld() -> noIcenoBoulder(getX() + 1, getY(), right) && getX() < 60)
            {
                setDirection(right);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
        }
    }
    
    if(getDirection() == left)
    {
        if(getWorld()-> noIcenoBoulder(getX() - 1, getY(), left) && getX() > 0)
            moveTo(getX() -1, getY());
        else
            resetSquaresToMoveInDir();
    }
    else if(getDirection() == right)
    {
        if(getWorld()-> noIcenoBoulder(getX()+1, getY(), right) && getX() < 60)
            moveTo(getX()+1, getY());
        else
            resetSquaresToMoveInDir();
    }
    
    else if(getDirection() == up)
    {
        if(getWorld()-> noIcenoBoulder(getX(), getY() + 1, up) && getY() < 60)
            moveTo(getX(), getY() + 1);
        else
            resetSquaresToMoveInDir();
    }
    else if(getDirection() == down)
    {
        if(getWorld()-> noIcenoBoulder(getX(), getY() -1, down) && getY() > 0)
            moveTo(getX(), getY() -1);
        else
            resetSquaresToMoveInDir();
    }
    
    prepareNextMove();
    
}

void HardcoreProtesters::doSomething()
{
    Direction dir = none;
    if (!isAlive())
        return;
    if(getRestingTicksLeft() > 0)
    {
        rest();
        return;
    }
    
    if(getState() == "leave-oil-field")
    {
        if(getX() == 60 && getY() == 60)
            setDead();
        int steps;
        if(findNearestPath(60, 60, getX(), getY(),dir, steps))
        {
            setDirection(dir);
            if (dir == up)
                moveTo(getX(), getY() + 1);
            else if(dir == down)
                moveTo(getX(), getY() - 1);
            else if(dir == left)
                moveTo(getX() - 1, getY());
            else if(dir == right)
                moveTo(getX() + 1, getY());
            
        }
        
        prepareNextMove();
        return;
    }
    
    if(getWorld() -> canShout(getX(), getY(), getDirection()))
    {
        if(getTicksSinceShout() > 15)
        {
            shout();
            prepareNextMove();
            return;
        }
        
        else if (getTicksSinceShout() <= 15)
        {
            prepareNextMove();
            return;
        }
    }
    
    if (!getWorld() -> NearIceman(getX(), getY(), 4))
    {
        int M = 16 + getWorld() -> getLevel() * 2;
        int steps = 0;
        bool canFindIceMan = findNearestPath(getWorld()-> Iceman_ptr() -> getX(), getWorld()-> Iceman_ptr() -> getY(), getX(), getY(), dir, steps);
        if (steps <= M && canFindIceMan)
        {
            setDirection(dir);
            if (dir == up)
                moveTo(getX(), getY() + 1);
            else if(dir == down)
                moveTo(getX(), getY() - 1);
            else if(dir == left)
                moveTo(getX() - 1, getY());
            else if(dir == right)
                moveTo(getX() + 1, getY());
            
            prepareNextMove();
            
            return;
        }
    }
    
    if(getWorld() -> canFace(getX(), getY(), dir))
    {
        setDirection(dir);
        if (dir == up)
            moveTo(getX(), getY() + 1);
        else if(dir == down)
            moveTo(getX(), getY() - 1);
        else if(dir == left)
            moveTo(getX() - 1, getY());
        else if(dir == right)
            moveTo(getX() + 1, getY());
        
        resetSquaresToMoveInDir();
        prepareNextMove();
        return;
    }
    
    
    if(!getWorld() -> canFace(getX(), getY(), dir))
    {
        decreaseSquaresToMoveInDir();
    }
    
    if(getSquaresToMoveInDir() <= 0)
    {
        
        int newDir = getWorld()->generateNewDir(getX(), getY());
        setDirection((Direction) newDir);
        pickNewSquaresToMoveInDir();
    }
    
    else if(getTicksSinceTurned() >= 200)
    {
        if (getDirection() == right || getDirection() == left)
        {
            if(getWorld() -> noIcenoBoulder(getX(), getY() + 1, up) && getY() < 60)
            {
                setDirection(up);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
            
            else if(getWorld() -> noIcenoBoulder(getX(), getY() -1, down) && getY() > 0)
            {
                setDirection(down);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
        }
        
        else if(getDirection() == up || getDirection() == down)
        {
            if(getWorld() -> noIcenoBoulder(getX() -1, getY(), left) && getX() > 0)
            {
                setDirection(left);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
            
            else if(getWorld() -> noIcenoBoulder(getX() + 1, getY(), right) && getX() < 60)
            {
                setDirection(right);
                pickNewSquaresToMoveInDir();
                resertTicksSinceTurned();
            }
        }
    }
    
    if(getDirection() == left)
    {
        if(getWorld()-> noIcenoBoulder(getX() - 1, getY(), left) && getX() > 0)
            moveTo(getX() -1, getY());
        else
            resetSquaresToMoveInDir();
    }
    else if(getDirection() == right)
    {
        if(getWorld()-> noIcenoBoulder(getX()+1, getY(), right) && getX() < 60)
            moveTo(getX()+1, getY());
        else
            resetSquaresToMoveInDir();
    }
    
    else if(getDirection() == up)
    {
        if(getWorld()-> noIcenoBoulder(getX(), getY() + 1, up) && getY() < 60)
            moveTo(getX(), getY() + 1);
        else
            resetSquaresToMoveInDir();
    }
    else if(getDirection() == down)
    {
        if(getWorld()-> noIcenoBoulder(getX(), getY() -1, down) && getY() > 0)
            moveTo(getX(), getY() -1);
        else
            resetSquaresToMoveInDir();
    }
    
    prepareNextMove();
    
}


bool Protesters::findNearestPath(int startX, int startY, int finalX, int finalY, Direction& finalDir, int& steps)
{
    int stepsArray[64][64];
    bool rv = false;
    for (int i = 0; i != 64; i++)
        for(int j = 0; j != 64; j++)
            stepsArray[i][j] = 9999;
    std::queue<Point> exitPath;
    
    exitPath.push(Point(startX, startY));
    stepsArray[startX][startY] = 0;
    
    int curSteps;
    
    while(!exitPath.empty())
    {
        int curX = exitPath.front().m_x;
        int curY = exitPath.front().m_y;
        exitPath.pop();
        
        if(curX == finalX && curY == finalY)
            rv = true;
        
        curSteps = stepsArray[curX][curY];
        curSteps++;
        
        
        
        if(curX<60 && getWorld() -> noIcenoBoulder(curX + 1, curY, right) && stepsArray[curX+1][curY] == 9999)
        {
            exitPath.push(Point(curX + 1, curY));
            stepsArray[curX+1][curY] = curSteps;
        }
        
        if(curX>1 && getWorld() -> noIcenoBoulder(curX - 1, curY, left) && stepsArray[curX-1][curY] == 9999)
        {
            exitPath.push(Point(curX - 1, curY));
            stepsArray[curX-1][curY] = curSteps;
        }
        
        if(curY<60 && getWorld() -> noIcenoBoulder(curX, curY + 1, up) && stepsArray[curX][curY+1] == 9999)
        {
            exitPath.push(Point(curX, curY + 1));
            stepsArray[curX][curY+1] = curSteps;
        }
        
        if(curY>0 && getWorld() -> noIcenoBoulder(curX, curY - 1, down) && stepsArray[curX][curY-1] == 9999)
        {
            exitPath.push(Point(curX, curY - 1));
            stepsArray[curX][curY -1] = curSteps;
        }
        
    }
    int minSteps;
    if(finalX == 0 && finalY == 0)
    {
        minSteps = min(stepsArray[finalX+1][finalY], stepsArray[finalX][finalY+1]);
        if(minSteps == stepsArray[finalX+1][finalY])
            finalDir = right;
        else
            finalDir = up;
    }
    else if (finalX == 0)
    {
        minSteps = min(stepsArray[finalX+1][finalY], min(stepsArray[finalX][finalY+1], stepsArray[finalX][finalY-1]));
        if(minSteps == stepsArray[finalX+1][finalY])
            finalDir = right;
        else if(minSteps == stepsArray[finalX][finalY + 1])
            finalDir = up;
        else
            finalDir = down;
    }
    
    else if (finalY ==0)
    {
        minSteps = min(stepsArray[finalX][finalY + 1], min(stepsArray[finalX - 1][finalY], stepsArray[finalX + 1][finalY]));
        if(minSteps == stepsArray[finalX+1][finalY])
            finalDir = right;
        else if(minSteps == stepsArray[finalX][finalY + 1])
            finalDir = up;
        else
            finalDir = left;
    }
    else
    {
        minSteps = min(min(stepsArray[finalX][finalY + 1], stepsArray[finalX][finalY-1]), min(stepsArray[finalX - 1][finalY], stepsArray[finalX + 1][finalY]));
        if(minSteps == stepsArray[finalX+1][finalY])
            finalDir = right;
        else if(minSteps == stepsArray[finalX][finalY + 1])
            finalDir = up;
        else if(minSteps == stepsArray[finalX-1][finalY])
            finalDir = left;
        else
            finalDir = down;
        
    }
    
    steps = minSteps;
    return rv;
}


void Protesters::takeDamage(int amt)
{
    AnnoyedActor::takeDamage(amt);
    
    if(gethitPoints() > 0)
    {
        GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
        Stunned();
    }
    
    if(gethitPoints() <= 0 && amt == 2) //annoyed due to being squirted
    {
        if(isHardcoreProtester())
            getWorld() -> increaseScore(250);
        else
            getWorld() -> increaseScore(100);
    }
    
    if(gethitPoints() <= 0 && amt == 100) //annoyed due to boulder
    {
        getWorld() -> increaseScore(500);
    }
}

void Protesters::Stunned()
{
    restingTicksLeft = max<unsigned int>(50, 100 - getWorld() -> getLevel() * 10);
    m_state = "rest";
}


void Protesters::giveUp()
{
    setLeaveOilFieldState();
    GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
    
}

void Protesters::shout()
{
    GameController::getInstance().playSound(SOUND_PROTESTER_YELL);
    getWorld()-> Iceman_ptr() -> takeDamage(2);
    nonRestingTicksSinceShouted = 0;
}

void RegularProtesters::Bribed()
{
    GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld() -> increaseScore(25);
    setLeaveOilFieldState();
}

void HardcoreProtesters::Bribed()
{
    GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
    getWorld() -> increaseScore(50);
    Stunned();
}

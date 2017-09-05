#include "StudentWorld.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
    return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp


bool StudentWorld::removeIce(int x, int y)
{
    bool rv = false;
    for (int i = 0; i < nIce; i++)
        if (IcePointers[i] != nullptr)
            if(IcePointers[i] -> getX() >= x && IcePointers[i] -> getX() <= x+3 && IcePointers[i] -> getY() >= y && IcePointers[i] -> getY() <= y+3)
            {
                delete IcePointers[i];
                IcePointers[i] = nullptr;
                rv = true;
            }
    return rv;
}

bool StudentWorld::NearIceman(int x, int y, int amount)
{
    int square = pow(m_iceman -> getX() - x, 2) + pow(m_iceman -> getY() - y, 2);
    if(sqrt(square) <= amount)
        return true;
    
    return false;
}

void StudentWorld::boulderAnnoyActors(int x, int y)
{
    if (m_iceman -> getX() >= x-3 && m_iceman -> getX() <= x + 3  && m_iceman -> getY() >= y - 3 && m_iceman -> getY() <= y+ 3)
        m_iceman -> giveUp();
    
    annoyProtester(x, y, 100);
    
}

bool StudentWorld::annoyProtester(int x, int y, int amt)
{
    bool rv = false;
    std::vector<Actor*>::iterator it;
    for (it = Actors.begin(); it != Actors.end(); it++)
    {
        if((*it) -> getX() >= x-3 && (*it) -> getX() <= x +3 && (*it) -> getY() >= y-3 && (*it) -> getY() <= y+3)
            if((*it)->isProtester() && (*it) -> getState() != "leave-oil-field")
            {
                (*it) -> takeDamage(amt);
                rv = true;
            }
    }
    
    return rv;
}
void StudentWorld::setPositions(int x, int y, char actortype)
{
    for (int i = x; i != x + 4; i++)
        for (int j = y; j != y + 4; j++)
            ActorPositions[i][j] = actortype;
}

void StudentWorld::squirtWater(int x, int y, GraphObject::Direction dir)
{
    Actors.push_back(new Squirt(x, y, this, dir));
    GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
}


void StudentWorld::illuminate(int x, int y)
{
    std::vector<Actor*>::iterator it;
    for (it = Actors.begin(); it != Actors.end(); it++)
    {
        int square = pow((*it) -> getX() - x, 2) + pow((*it) -> getY() - y, 2);
        if(sqrt(square) <= 12)
            (*it) -> setVisible(true);
    }
    GameController::getInstance().playSound(SOUND_SONAR);
    
}

bool StudentWorld::isBoulder(int x, int y, GraphObject:: Direction dir)
{
    switch(dir)
    {
        case GraphObject::right:
            if (getPositions(x + 3, y) == 'B' || getPositions(x +3, y + 3) == 'B')
                return true;
            break;
        case GraphObject::left:
            if (getPositions(x, y) == 'B' || getPositions(x, y + 3) == 'B')
                return true;
            break;
        case GraphObject::up:
            if (getPositions(x, y + 3) == 'B' || getPositions(x + 3, y + 3) == 'B')
                return true;
            break;
        case GraphObject::down:
            if (getPositions(x, y) == 'B' || getPositions(x + 3, y) == 'B')
                return true;
            break;
    }
    return false;
}

bool StudentWorld::isIce(int x, int y, GraphObject::Direction dir)
{
    switch(dir)
    {
        case GraphObject::down:
            for (int i = 0; i < nIce; i++)
                if (IcePointers[i] != nullptr)
                    if(IcePointers[i] -> getX() >= x && IcePointers[i] -> getX() <= x+3 && IcePointers[i] -> getY() == y)
                        return true;
            break;
        case GraphObject::up:
            for (int i = 0; i < nIce; i++)
                if (IcePointers[i] != nullptr)
                    if(IcePointers[i] -> getX() >= x && IcePointers[i] -> getX() <= x+3 && IcePointers[i] -> getY() == y + 3)
                        return true;
            break;
            
        case GraphObject::right:
            for (int i = 0; i < nIce; i++)
                if (IcePointers[i] != nullptr)
                    if(IcePointers[i] -> getY() >= y && IcePointers[i] -> getY() <= y+3 && IcePointers[i] -> getX() == x + 3)
                        return true;
            break;
        case GraphObject:: left:
            for (int i = 0; i < nIce; i++)
                if (IcePointers[i] != nullptr)
                    if(IcePointers[i] -> getY() >= y && IcePointers[i] -> getY() <= y+3 && IcePointers[i] -> getX() == x)
                        return true;
            break;
            
    }
    
    return false;
}


bool StudentWorld::noIcenoBoulder(int x, int y, GraphObject::Direction dir)
{
    if(!isIce(x, y, dir) && !isBoulder(x, y, dir))
        return true;
    return false;
}

bool StudentWorld::canFall(int x, int y)
{
    if (y < 0)
        return false;
    if(!noIcenoBoulder(x, y, GraphObject::down))
        return false;
    return true;
}

bool StudentWorld::canShout(int x, int y, GraphObject::Direction dir)
{
    switch(dir)
    {
        case GraphObject::left:
            if(x<= m_iceman -> getX() + 4 && x >= m_iceman -> getX() && y < m_iceman -> getY() + 4 && y > m_iceman -> getY() - 4)
                return true;
            break;
        case GraphObject::right:
            if(x>= m_iceman -> getX() - 4 && x <= m_iceman -> getX() && y < m_iceman -> getY() + 4 && y > m_iceman -> getY() - 4)
                return true;
            break;
        case GraphObject::up:
            if(y>= m_iceman -> getY() - 4 && y<= m_iceman -> getY() && x < m_iceman -> getX() + 4 && x > m_iceman -> getX() - 4)
                return true;
            break;
        case GraphObject::down:
            if(y<= m_iceman -> getY() + 4  && y >= m_iceman -> getY() && x < m_iceman -> getX() + 4 && x > m_iceman -> getX() - 4)
                return true;
            break;
    }
    return false;
}

bool StudentWorld::finishedLevel()
{
    if (pickedBarrels == nBarrels)
    {
        GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
        return true;
    }
    return false;
}

bool StudentWorld::playerDied()
{
    bool rv = false;
    if (!m_iceman -> isAlive())
    {
        rv = true;
        decLives();
    }
    
    return rv;
}

int StudentWorld::generateNewDir(int x, int y)
{
    int newDir = rand() % 4+1;
    switch (newDir) {
        case 1:
            if(!noIcenoBoulder(x, y+1, GraphObject::up) || y == 60)
                return generateNewDir(x, y);
            else
                return 1;
            break;
            
        case 2:
            if(!noIcenoBoulder(x, y-1, GraphObject::down) || y == 0)
                return generateNewDir(x, y);
            else
                return 2;
            break;
            
        case 3:
            if(!noIcenoBoulder(x-1, y, GraphObject::left) || x == 0)
                return generateNewDir(x, y);
            else
                return 3;
            break;
            
        case 4:
            if(!noIcenoBoulder(x+1, y, GraphObject::right) || x == 60)
                return generateNewDir(x, y);
            else
                return 4;
            break;
    }
    
    return 0;
}

void StudentWorld::pickupBarrel(int x, int y)
{
    setPositions(x, y, 0);
    pickedBarrels++;
}

bool StudentWorld::canFace(int x, int y, GraphObject::Direction& dir)
{
    bool rv = false;
    if(x>= m_iceman -> getX() - 63 && x < m_iceman -> getX() - 4 && y < m_iceman -> getY() + 4 && y > m_iceman -> getY() - 4 && !NearIceman(x, y, 4))
    {
        rv = true;
        dir = GraphObject::right;
        for(int i = x; i != m_iceman -> getX(); i++)
            if(!noIcenoBoulder(i, y, GraphObject::right))
                rv = false;
    }
    
    else if(x<= m_iceman -> getX() + 63 && x > m_iceman -> getX() + 4 && y < m_iceman -> getY() + 4 && y > m_iceman -> getY() - 4 && !NearIceman(x, y, 4))
    {
        rv = true;
        dir = GraphObject::left;
        for(int i = x; i != m_iceman -> getX(); i--)
            if(!noIcenoBoulder(i, y, GraphObject::left))
                rv = false;
    }
    
    else if(y>= m_iceman -> getY() - 63 && y < m_iceman -> getY() - 4 && x < m_iceman -> getX() + 4 && x > m_iceman -> getX() - 4 && !NearIceman(x, y, 4))
    {
        rv = true;
        dir = GraphObject::up;
        for(int i = y; i != m_iceman -> getY(); i++)
            if(!noIcenoBoulder(x, i, GraphObject::up))
                rv = false;
    }
    
    else if(y<= m_iceman -> getY() + 63 &&  y > m_iceman -> getY() + 4 && x < m_iceman -> getX() + 4 && x > m_iceman -> getX() - 4 && !NearIceman(x, y, 4))
    {
        rv = true;
        dir = GraphObject::down;
        for(int i = y; i != m_iceman -> getY(); i--)
            if(!noIcenoBoulder(x, i, GraphObject::down))
                rv = false;
    }
    
    return rv;
}

bool StudentWorld::canAddProtester()
{
    if(ticksSinceLastProtester >= ticksToWaitToAddProtester && nProtesters < min<unsigned int>(15, 2 + getLevel() * 1.5))
    {
        ticksSinceLastProtester = 0;
        nProtesters++;
        return true;
    }
    
    
    ticksSinceLastProtester++;
    return false;
}

bool StudentWorld::canAddWater(int x, int y)
{
    for(int i = 0; i != nIce; i++)
        if(IcePointers[i] != nullptr)
        {
            if(IcePointers[i] -> getX() >= x && IcePointers[i] -> getX() <= x+ 3 && IcePointers[i] -> getY() >= y && IcePointers[i] -> getY() <= y+3)
                return false;
        }
    return true;
}


Actor* StudentWorld::findProtester(int x, int y)
{
    std::vector<Actor*>::iterator it;
    for (it = Actors.begin(); it != Actors.end(); it++)
    {
        if((*it) -> getX() >= x-3 && (*it) -> getX() <= x +3 && (*it) -> getY() >= y-3 && (*it) -> getY() <= y+3)
            if((*it)->isProtester())
                return (*it);
    }
    
    return nullptr;
}


void StudentWorld::removeDeadGameObjects()
{
    std::vector<Actor*>::iterator it;
    
    for (it = Actors.begin(); it != Actors.end();)
        if(!(*it) -> isAlive())
        {
            setPositions((*it) -> getX(), (*it) -> getY(), 0);
            if((*it) -> isProtester())
                nProtesters--;
            delete (*it);
            it = Actors.erase(it);
            
        }
        else
            it++;
}

void StudentWorld::updateDisplayText()
{
    int level = getLevel();
    int lives = getLives();
    int health = m_iceman -> gethitPoints() * 10;
    int squirts = m_iceman -> getSquirts();
    int gold = m_iceman -> getGold();
    int barrelsLeft = nBarrels - pickedBarrels;
    int sonar = m_iceman -> getSonar();
    int score = getScore();
    ostringstream oss;
    oss.setf(ios::fixed);
    oss.precision(0);
    oss << "Lvl: " << setw(2) << level << "  Lives: " << lives << "  Hlth: " << setw(3) << health << "%" << "  Wtr: " << setw(2) << squirts << "  Gld: " << setw(2) << gold << "  Oil Left: " << setw(2) << barrelsLeft << "  Sonar: " << setw(2) << sonar << "  Scr: ";
    oss.fill('0');
    oss << setw(6) << score;
    string text = oss.str();
    setGameStatText(text);
}

bool StudentWorld::noOverlap(int x, int y)
{
    std::vector<Actor*>::iterator it;
    
    for (it = Actors.begin(); it != Actors.end(); it++)
    {
        int square = pow((*it) -> getX() - x, 2) + pow((*it) -> getY() - y, 2);
        if(sqrt(square) <= 6)
            return false;
    }
    
    if (getPositions(x, y) != 0 || getPositions(x+3, y) != 0 || getPositions(x, y+3) != 0 ||getPositions(x+3, y+3) != 0)
        return false;
    
    return true;
}

int StudentWorld::init()
{
    nIce = 3616;
    ticksToWaitToAddProtester = ticksSinceLastProtester = max<unsigned int>(25, 200 - getLevel());
    pickedBarrels = 0;
    nProtesters = 0;
    int i = 0;
    while (i < nIce)
    {for (int x = 0; x < 64; x++)
        for (int y = 0; y < 60; y++)
        {
            if (x >= 30 && x <= 33 && y > 3)
            {;}
            else
            {
                IcePointers[i] = new Ice(x, y, this);
                i++;
            }
        }
    }
    
    for (int x = 30; x != 34; x++)
        for (int y = 4; y != 60; y++)
            ActorPositions[x][y] = 't';
    
    m_iceman = new IceMan(30, 60, this);
    
    
    nBoulders = min<unsigned int>(getLevel()/2 + 2, 9);
    
    for (int j = 0; j < nBoulders; j++)
    {
        int x = rand() % 61;
        int y = rand() % 37 + 20;
        
        if(noOverlap(x, y))
        {
            Actors.push_back(new Boulder(x, y, this));
            removeIce(x, y);
            setPositions(x, y, 'B');
        }
        
        else
            j--;
    }
    
    
    nGold = max<unsigned int>(5 - getLevel()/2, 2);
    
    for (int j = 0; j < nGold; j++)
    {
        int x = rand() % 61;
        int y = rand() % 57;
        if (noOverlap(x,y))
        {
            Actors.push_back(new Gold(x, y, this, false, true));
            setPositions(x, y, 'G');
        }
        
        else
            j--;
        
    }
    
    
    nBarrels = min<unsigned int>(2 + getLevel(), 21);
    
    for (int j = 0; j < nBarrels; j++)
    {
        int x = rand() % 61;
        int y = rand() % 57;
        if (noOverlap(x,y))
        {
            Actors.push_back(new Oil(x, y, this));
            setPositions(x, y, 'O');
        }
        else
            j--;
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    updateDisplayText(); //update display line
    
    int probabilityOfHardcore = min<unsigned int>(90, getLevel() * 10 + 30);
    int G = getLevel()*25 + 300;
    
    //add actors
    if (canAddProtester())
    {
        int n = rand() % 100 + 1;
        if (n <= probabilityOfHardcore)
            Actors.push_back(new HardcoreProtesters(this)); //create hardcore
        else
            Actors.push_back(new RegularProtesters(this));
    }
    
    int n = rand() % G + 1;
    
    if (n <= 1)
    {
        int prob = rand() % 5 + 1;
        if (prob <= 1)
            Actors.push_back(new Sonar(this));
        else if (prob > 1)
        {
            int x = rand() % 61;
            int y = rand() % 61;
            while (!canAddWater(x, y))
            {
                x = rand() % 61;
                y = rand() % 61;
            }
            
            Actors.push_back(new Water(x,y, this));
        }
    }
    
    
    //if each actor is alive, give it a chance to do something and check if player died/finished level
    
    if (m_iceman -> isAlive())
        m_iceman -> doSomething();
    
    std::vector<Actor*>::iterator it;
    
    for (it = Actors.begin(); it != Actors.end(); it++)
    {
        if ((*it) -> isAlive())
            (*it) -> doSomething();
        if (playerDied())
            return GWSTATUS_PLAYER_DIED;
        if(finishedLevel())
            return GWSTATUS_FINISHED_LEVEL;
    }
    
    removeDeadGameObjects();
    
    
    if (playerDied())
        return GWSTATUS_PLAYER_DIED;
    
    //if player finished level return finished level
    
    if(finishedLevel())
        return GWSTATUS_FINISHED_LEVEL;
    
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (int i = 0; i < nIce; i++)
    {
        if (IcePointers[i] != nullptr)
        {
            delete IcePointers[i];
            IcePointers[i] = nullptr;
        }
        
    }
    delete m_iceman;
    std::vector<Actor*>::iterator it;
    
    for (it = Actors.begin(); it != Actors.end();)
    {
        delete (*it);
        it = Actors.erase(it);
    }
    
    for (int x = 0; x!= 64; x++)
        for (int y = 0; y!= 64; y++)
            ActorPositions[x][y]=0;
}

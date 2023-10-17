#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Board.h"
#include "Actor.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    Board getBoard() { return bd; }
    Peach* getPeach() { return peach; }
    Yoshi* getYoshi() { return yoshi; }
    void setBankBalance(int i) { bankBalance = i; }
    int getBankBalance() { return bankBalance; }
    bool isValidPosition(int x, int y); 
    void createVortex(int x, int y, int dir);
    void deleteVortex();
    void createDroppingSquare(int x, int y);
    bool overlaps(Vortex* v);
private:
    Board bd;
    Yoshi* yoshi;
    Peach* peach;
    int bankBalance;
    std::vector<Actor*> actors;
};

#endif // STUDENTWORLD_H_

#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "Board.h"
#include <string>
#include <vector>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath), bankBalance(0)
{
}

int StudentWorld::init()
{
    string board_file = assetPath() + "board0" + to_string(getBoardNumber()) + ".txt";
    Board::LoadResult result = bd.loadBoard(board_file);
    
    if (result == Board::load_fail_bad_format) {
        cerr << "Your board was improperly formatted\n";
    } else if (result == Board::load_fail_file_not_found) {
        cerr << "Could not find board01.txt data file\n";
    } else if (result == Board::load_success) {
        cerr << "Successfully loaded board\n";
    }
    
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
            Board::GridEntry ge = bd.getContentsOf(i, j);
            
            switch (ge) {
                case Board::empty:
                    break;
                case Board::player:
                    peach = new Peach(this, i, j);
                    yoshi = new Yoshi(this, i, j);
                    actors.push_back(new BlueCoinSquare(this, i, j));
                    break;
                case Board::blue_coin_square:
                    actors.push_back(new BlueCoinSquare(this, i, j));
                    break;
                case Board::red_coin_square:
                    actors.push_back(new RedCoinSquare(this, i, j));
                    break;
                case Board::left_dir_square:
                    actors.push_back(new LeftDirectionSquare(this, i, j));
                    break;
                case Board::right_dir_square:
                    actors.push_back(new RightDirectionSquare(this, i, j));
                    break;
                case Board::up_dir_square:
                    actors.push_back(new UpDirectionSquare(this, i, j));
                    break;
                case Board::down_dir_square:
                    actors.push_back(new DownDirectionSquare(this, i, j));
                    break;
                case Board::event_square:
                    actors.push_back(new EventSquare(this, i,j ));
                    break;
                case Board::bank_square:
                    actors.push_back(new BankSquare(this, i, j));
                    break;
                case Board::star_square:
                    actors.push_back(new StarSquare(this, i, j));
                    break;
                case Board::bowser:
                    actors.push_back(new Bowser(this, i, j));
                    actors.push_back(new BlueCoinSquare(this, i, j));
                    break;
                case Board::boo:
                    actors.push_back(new Boo(this, i, j));
                    actors.push_back(new BlueCoinSquare(this, i, j));
                    break;
                    
            }
        }
    }

    startCountdownTimer(99);
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    peach->doSomething();
    yoshi->doSomething();
    for (Actor* a : actors) {
        if (a == nullptr) {
            continue;
        }
        a->doSomething();
    }
    
    ostringstream oss;
    oss << "P1 Roll: " << ((peach->getTicksToMove() + 7) / 8) << " Stars: " << peach->getNumStars() << " $$: " << peach->getNumCoins() << peach->vortexMessage() << " | Time: " << timeRemaining() << " | Bank: " << getBankBalance() << " | P2 Roll: " << ((yoshi->getTicksToMove() + 7) / 8) << " Stars: " << yoshi->getNumStars() << " $$: " << yoshi->getNumCoins();

    setGameStatText(oss.str());
    
    if (timeRemaining() <= 0)
        return GWSTATUS_NOT_IMPLEMENTED;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (Actor* a : actors) {
        delete a;
    }
    actors.clear();
    
    delete peach;
    peach = nullptr;
    delete yoshi;
    yoshi = nullptr;
}

StudentWorld::~StudentWorld() {
    cleanUp();
}

bool StudentWorld::isValidPosition(int x, int y) {
    Board::GridEntry ge = bd.getContentsOf(x / SPRITE_WIDTH, y / SPRITE_HEIGHT);
    
    if (ge == Board::empty) {
        return false;
    } else {
        return true;
    }
}

void StudentWorld::createVortex(int x, int y, int dir) {
    actors.push_back(new Vortex(this, x, y, dir));
}

void StudentWorld::deleteVortex() {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i] == nullptr) {
            continue;
        }
        if (actors[i]->isVortex()) {
            delete actors[i];
            actors[i] = nullptr;
            break;
        }
    }
}

void StudentWorld::createDroppingSquare(int x, int y) {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i] == nullptr) {
            continue;
        }
        if (actors[i]->getX() == x && actors[i]->getY() == y && actors[i]->isSquare()) {
            delete actors[i];
            actors[i] = nullptr;
            actors.push_back(new DroppingSquare(this, x / SPRITE_WIDTH, y / SPRITE_HEIGHT));
            playSound(SOUND_DROPPING_SQUARE_CREATED);
            break;
        }
    }
}

bool StudentWorld::overlaps(Vortex* v) {
    for (int i = 0; i < actors.size(); i++) {
        if (actors[i] == nullptr) {
            continue;
        }
        if ((v->getX() + SPRITE_WIDTH > actors[i]->getX()) && (v->getX() < SPRITE_WIDTH + actors[i]->getX()) && (v->getY() + SPRITE_HEIGHT > actors[i]->getY()) && (v->getY() < SPRITE_HEIGHT + actors[i]->getY()) && actors[i]->isImpactable()) {
            actors[i]->setImpacted(true);
            return true;
        }
    }
    return false;
}

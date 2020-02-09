#ifndef GAME_H
#define GAME_H

#include "ai.h"
#include "board.h"
#include "boardmodel.h"
#include "panelmodel.h"
#include <condition_variable>
#include <mutex>
#include <queue>

class Game : public QObject {
    Q_OBJECT
private:
    BoardModel* bm;
    PanelModel* pm;

    std::mutex inputMutex;
    std::condition_variable cv;
    std::queue<size_t> inputQueue;
    std::atomic_bool userTurn;

protected:
    Board board;
    Team turn = Team::NONE;

protected:
    Game(BoardModel* bm, PanelModel* pm);
    void changeTurn();
    size_t userInput(BitBoard placeable);
    void gameOver();
    void updateModel();

public:
    Team getTurn();
    virtual ~Game();
    void pushIndex(size_t index);
    virtual void run() = 0;
};

#endif // GAME_H

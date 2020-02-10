#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "boardmodel.h"
#include "panelmodel.h"

class Game : public QObject {
    Q_OBJECT
private:
    BoardModel* bm;
    PanelModel* pm;

protected:
    Board board;
    Team turn = Team::NONE;

protected:
    Game(BoardModel* bm, PanelModel* pm);
    void changeTurn();
    void gameOver();
    void updateModel();

public:
    Team getTurn();
    virtual ~Game();
    virtual void run() = 0;
};

#endif // GAME_H

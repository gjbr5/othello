#ifndef LOCALGAME_H
#define LOCALGAME_H

#include "game.h"
#include "player.h"

class LocalGame : public Game {
    Q_OBJECT
private:
    Player* black;
    Player* white;

public:
    LocalGame(BoardModel* bm, PanelModel* pm, Player* player1, Player* player2);
    void run() override;
    ~LocalGame() override;
};

#endif // LOCALGAME_H

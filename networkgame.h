#ifndef NETWORKGAME_H
#define NETWORKGAME_H

#include "game.h"
#include "networkclient.h"
#include "player.h"

class NetworkGame : public Game {
    Q_OBJECT
private:
    NetworkClient client;
    Player* player;
    bool isLocalFirst;
    std::string token;

public:
    NetworkGame(BoardModel* bm, PanelModel* pm, uint32_t ip, uint16_t port, Player* player);
    ~NetworkGame() override;
    void run() override;
};

#endif // NETWORKGAME_H

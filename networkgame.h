#ifndef NETWORKGAME_H
#define NETWORKGAME_H

#include "ai.h"
#include "game.h"
#include "networkclient.h"

class NetworkGame : public Game {
    Q_OBJECT
private:
    NetworkClient client;
    AI* ai;
    bool isLocalFirst;
    std::string token;
    Team team;

public:
    NetworkGame(BoardModel* bm, PanelModel* pm, uint32_t ip, uint16_t port, AI* ai = nullptr);
    ~NetworkGame() override;
    void run() override;
};

#endif // NETWORKGAME_H

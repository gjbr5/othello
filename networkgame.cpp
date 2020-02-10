#include "networkgame.h"

NetworkGame::NetworkGame(BoardModel* bm, PanelModel* pm, uint32_t ip, uint16_t port, AI* ai)
    : Game(bm, pm)
    , client(ip, port)
    , ai(ai)
{
    DataMap initData = client.recv();
    board = initData["board"];
    token = initData["token"];
    if (initData["color"] == "b") {
        if (ai)
            ai->setTeam(Team::BLACK);
        else
            team = Team::BLACK;
        qInfo() << "You are Black.";
        isLocalFirst = true;
    } else {
        if (ai)
            ai->setTeam(Team::WHITE);
        else
            team = Team::WHITE;
        isLocalFirst = false;
        qInfo() << "You are White.";
    }
}

NetworkGame::~NetworkGame()
{
    if (ai)
        delete ai;
}
void NetworkGame::run()
{
    while (true) {
        DataMap data = client.recv();
        std::string code = data["code"];
        if (code == "update") {
            board = data["board"];
            updateModel();
        } else if (code == "turn") {
            size_t nextDisk = ai ? ai->nextDisk(board) : userInput(board.allLegalMove(team));
            client.sendMove(nextDisk, token);
            qDebug().nospace() << "Disk = " << nextDisk << "(" << nextDisk / 8 << ", " << nextDisk % 8 << ")";
        } else if (code == "end") {
            break;
        } else {
            qWarning().noquote() << "Unknown Code:" << QString::fromStdString(code);
            break;
        }
    }
    // Game Over
    gameOver();
}

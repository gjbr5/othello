#include "localgame.h"
#include <QDebug>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>

LocalGame::LocalGame(BoardModel* bm, PanelModel* pm, Player* player1, Player* player2)
    : Game(bm, pm)
    , black(player1)
    , white(player2)
{
}

LocalGame::~LocalGame()
{
    delete black;
    delete white;
}

void LocalGame::run()
{
    qInfo() << "Game Start!";

    std::srand(std::time(nullptr));
    if (std::rand() % 2) {
        black->setTeam(Team::BLACK);
        white->setTeam(Team::WHITE);
        qInfo() << "Player1 is BLACK, Player2 is WHITE";
    } else {
        Player* tmp = black;
        black = white;
        white = tmp;

        black->setTeam(Team::BLACK);
        white->setTeam(Team::WHITE);
        qInfo() << "Player2 is BLACK, Player1 is WHITE";
    }

    turn = Team::BLACK;

    while (true) {
        // Check Next Legal Move
        BitBoard placeable = board.allLegalMove(turn);

        if (placeable.none()) { // Pass
            qInfo() << (turn == Team::BLACK ? "Pass: Black" : "Pass: White");
            changeTurn();
            placeable = board.allLegalMove(turn);
            if (placeable.none()) // Game Over
                break;
        }

        // Set Next Disk
        size_t nextDisk = turn == Team::BLACK ? black->nextDisk(board) : white->nextDisk(board);

        qDebug().nospace() << "Disk = " << nextDisk << "(" << nextDisk / 8 << ", " << nextDisk % 8 << ")";

        board.setDisk(nextDisk, turn);
        updateModel();

        changeTurn();
    }
    // Game Over
    gameOver();
}

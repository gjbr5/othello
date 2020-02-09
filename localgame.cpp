#include "localgame.h"
#include <QDebug>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>

LocalGame::LocalGame(BoardModel* bm, PanelModel* pm, AI* ai)
    : Game(bm, pm)
    , ai(ai)
{
}

LocalGame::~LocalGame()
{
    if (ai)
        delete ai;
}

void LocalGame::run()
{
    qInfo() << "Game Start!";
    bool aiFirst = false;
    if (ai) {
        // Single Play
        std::srand(std::time(nullptr));
        aiFirst = std::rand() % 2 == 0;
        if (aiFirst) {
            ai->setTeam(Team::BLACK);
            qInfo() << "You are White.";
        } else {
            ai->setTeam(Team::WHITE);
            qInfo() << "You are Black.";
        }
    }

    //    // Timeout
    //    QTimer timer;
    //    timer.setInterval(std::chrono::milliseconds(1500));

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
        //        timer.start();
        size_t nextDisk;
        if (ai)
            if (aiFirst)
                nextDisk = turn == Team::BLACK ? ai->nextDisk(board) : userInput(placeable);
            else
                nextDisk = turn == Team::BLACK ? userInput(placeable) : ai->nextDisk(board);
        else
            nextDisk = userInput(placeable);
        qDebug().nospace() << "Disk = " << nextDisk << "(" << nextDisk / 8 << ", " << nextDisk % 8 << ")";
        //        timer.stop();
        board.setDisk(nextDisk, turn);
        updateModel();
        changeTurn();
    }
    // Game Over
    gameOver();
}

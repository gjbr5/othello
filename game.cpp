#include "game.h"
#include <QDebug>
#include <QTimer>

Game::Game(BoardModel* bm, PanelModel* pm)
    : bm(bm)
    , pm(pm)
    , userTurn(false)
{
    qInfo() << "Game Start!";
    bm->startGame(&board);
    pm->startGame();
    updateModel();
}
Team Game::getTurn()
{
    return turn;
}

Game::~Game()
{
    pm->newGame();
    bm->newGame();
}

void Game::changeTurn()
{
    turn = turn == Team::BLACK ? Team::WHITE : Team::BLACK;
}

void Game::pushIndex(size_t index)
{
    if (!userTurn || turn == Team::NONE)
        return;
    inputMutex.lock();
    inputQueue.emplace(index);
    inputMutex.unlock();

    cv.notify_all();
}

size_t Game::userInput(BitBoard placeable)
{
    qDebug() << "User's Turn";
    while (!inputQueue.empty())
        inputQueue.pop();

    userTurn = true;

    bm->setNextAvailable(placeable);
    std::unique_lock<std::mutex> lk(inputMutex);
    size_t next;
    while (true) {
        cv.wait(lk, [&] { return !inputQueue.empty(); });

        next = inputQueue.front();
        inputQueue.pop();

        if (placeable.test(next))
            break;
    }
    userTurn = false;
    bm->setNextAvailable(BitBoard());
    return next;
}

void Game::updateModel()
{
    pm->update(board.count(Team::BLACK), board.count(Team::WHITE));
    bm->update();
}

void Game::gameOver()
{
    qInfo() << "Game Over";
    pm->gameOver(board.count(Team::BLACK), board.count(Team::WHITE));
}

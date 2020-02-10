#include "game.h"
#include <QDebug>

Game::Game(BoardModel* bm, PanelModel* pm)
    : bm(bm)
    , pm(pm)
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

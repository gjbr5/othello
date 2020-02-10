#include "ai.h"
#include <QDebug>
#include <iostream>
#include <queue>
#include <vector>

AI::AI()
    : rand(std::random_device()())
{
}

AI::Evaluation::Evaluation(size_t move)
    : move(move)
{
}

struct AI::Evaluation& AI::Evaluation::operator+=(const int score)
{
    count++;
    if (score > 0)
        wins++;
    return *this;
}

double AI::Evaluation::score()
{
    return double(wins) / double(count);
}

size_t AI::nextDisk(const Board board)
{
    qDebug() << "AI Thinking...";
    BitBoard moves = board.allLegalMove(team);
    size_t moveCnt = moves.count();

    if (moveCnt == 1)
        return moves._Find_first();

    // Initialize
    std::queue<AI::Evaluation> evals;
    for (size_t move = moves._Find_first(); move != 64; move = moves._Find_next(move)) {
        evals.emplace(AI::Evaluation(move));
    }

    // Simulation
    for (size_t i = moveCnt * 5000; i; i--) {
        Evaluation eval = evals.front();
        eval += simulate(board, team, eval.move);
        evals.pop();
        evals.emplace(eval);
    }

    Evaluation best = evals.front();
    qDebug() << best.move << ":" << best.wins << "/" << best.count << "=" << best.score();
    evals.pop();
    while (!evals.empty()) {
        Evaluation& comp = evals.front();
        qDebug() << comp.move << ":" << comp.wins << "/" << comp.count << "=" << comp.score();
        if (comp.score() > best.score())
            best = comp;
        evals.pop();
    }
    return best.move;
}

Team AI::opponent(Team team)
{
    return team == Team::BLACK ? Team::WHITE : Team::BLACK;
}

int AI::simulate(Board board, Team team, size_t next)
{
    if (next < 64)
        board.setDisk(next, team);
    Team op = opponent(team);
    BitBoard moves = board.allLegalMove(op);

    size_t cnt = moves.count();
    if (cnt == 0) {
        if (board.allLegalMove(team).count() == 0) // End Game
            return board.count(opponent(this->team)) - board.count(this->team);
        else // Pass
            return simulate(board, op, -1);
    }

    // Random Location
    size_t location = moves._Find_first();
    for (size_t randBit = std::uniform_int_distribution<size_t>(0, cnt - 1)(rand); randBit; randBit--)
        location = moves._Find_next(location);

    return simulate(board, op, location);
}

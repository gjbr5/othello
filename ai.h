#ifndef AI_H
#define AI_H

#include "board.h"
#include "player.h"
#include <random>

class AI : public Player {
    Q_OBJECT

private:
    std::mt19937_64 rand;

    struct Evaluation {
        size_t move;
        size_t count = 0;
        size_t wins = 0;
        Evaluation(size_t move);
        struct Evaluation& operator+=(const int score);
        double score();
    };

private:
    int simulate(Board board, Team team, size_t next);
    Team opponent(Team team);

public:
    AI();
    size_t nextDisk(const Board board) override;
};

#endif // AI_H

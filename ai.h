#ifndef AI_H
#define AI_H

#include "board.h"
#include <QObject>
#include <random>

class AI : public QObject {
    Q_OBJECT

private:
    Team team;
    std::mt19937_64 rand;

    struct Evaluation {
        size_t move;
        size_t count = 0;
        size_t wins = 0;
        Evaluation(size_t move);
        struct Evaluation& operator+=(const size_t score);
        double score();
    };

public:
    AI();
    void setTeam(Team team);
    size_t nextDisk(const Board board);
    int simulate(Board board, Team team, size_t next);
    Team opponent(Team team);
    //    virtual size_t nextDisk(BitLoc friendly, BitLoc enemy)
};

#endif // AI_H

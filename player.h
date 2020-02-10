#ifndef PLAYER_H
#define PLAYER_H
#include "board.h"
#include <QObject>

class Player : public QObject {
    Q_OBJECT
protected:
    Team team = Team::NONE;

public:
    Player();
    void setTeam(Team team);
    virtual size_t nextDisk(const Board board) = 0;
    virtual ~Player();
};

#endif // PLAYER_H

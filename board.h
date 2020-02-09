#ifndef BOARD_H
#define BOARD_H

#include "team.h"
#include <bitset>
#include <string>

typedef std::bitset<64> BitBoard;

class Board {
private:
    BitBoard black;
    BitBoard white;
    BitBoard legalCheck(BitBoard friendly, BitBoard enemy, BitBoard move, int vector, BitBoard bound) const;

public:
    Board();

    size_t count(Team team) const;
    Board& operator=(std::string strBoard);
    void setDisk(int row, int column, Team team);
    void setDisk(int position, Team team);
    bool isBlack(int row, int column) const;
    bool isWhite(int row, int column) const;
    bool isDisk(int row, int column) const;

    BitBoard legalMove(size_t location, Team team) const;
    BitBoard allLegalMove(Team team) const;
};

#endif // BOARD_H

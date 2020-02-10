#include "board.h"
#include "flip.h"
#include <QDebug>
#include <memory.h>

Board::Board()
    : black(0x0000'0008'1000'0000)
    , white(0x0000'0010'0800'0000)
{
}

size_t Board::count(Team team) const
{
    switch (team) {
    case Team::BLACK:
        return black.count();
    case Team::WHITE:
        return white.count();
    default:
        return 0;
    }
}

Board& Board::operator=(std::string strBoard)
{
    if (strBoard.size() != 64) {
        qWarning().nospace() << "strBoard.size() is not 64 bytes:" << strBoard.size();
        qWarning().noquote() << QString::fromStdString(strBoard);
        return *this;
    }
    black.reset();
    white.reset();
    for (size_t i = 0; i < 64; i++)
        switch (strBoard[i]) {
        case 'b':
            black.set(i);
            break;
        case 'w':
            white.set(i);
            break;
        default:
            break;
        }
    return *this;
}

bool Board::isBlack(int row, int column) const { return black.test(row * 8 + column); }
bool Board::isWhite(int row, int column) const { return white.test(row * 8 + column); }
bool Board::isDisk(int row, int column) const { return isBlack(row, column) || isWhite(row, column); }

void Board::setDisk(int row, int column, Team team) { setDisk(row * 8 + column, team); }
void Board::setDisk(int position, Team team)
{
    BitBoard f;
    switch (team) {
    case Team::BLACK:
        f = Flip::flip(black.to_ullong(), white.to_ullong(), position);
        white = white & ~f;
        black = black | f;
        black.set(position);
        break;
    case Team::WHITE:
        f = Flip::flip(white.to_ullong(), black.to_ullong(), position);
        black = black & ~f;
        white = white | f;
        white.set(position);
        break;
    default:
        break;
    }
}

BitBoard Board::legalCheck(BitBoard friendly, BitBoard enemy, BitBoard move, int vector, BitBoard bound) const
{
    // Out of Bound
    if ((move & ~bound).any())
        return 0;

    BitBoard reverseEnemy = 0;

    // Bit Shift direction
    bool left = false;
    if (vector < 0) {
        vector = -vector;
        left = true;
    }

    left ? bound <<= vector : bound >>= vector;
    BitBoard chk = left ? move << vector : move >> vector;
    if ((chk & enemy).any()) {
        reverseEnemy |= chk & enemy;
        for (left ? chk <<= vector : chk >>= vector; (chk & bound).any(); left ? chk <<= vector : chk >>= vector) {
            if ((chk & enemy).any()) {
                reverseEnemy |= chk & enemy;
                continue;
            } else if ((chk & friendly).any())
                return reverseEnemy;
            else {
                return 0;
            }
        }
    }
    return 0;
}

BitBoard Board::legalMove(size_t location, Team team) const
{
    BitBoard bitLoc;
    bitLoc.set(location);
    if (black.test(location) || white.test(location))
        return 0;

    BitBoard friendly;
    BitBoard enemy;
    switch (team) {
    case Team::BLACK:
        friendly = black;
        enemy = white;
        break;
    case Team::WHITE:
        friendly = white;
        enemy = black;
        break;
    default:
        return 0;
    }

    int vector[] = {
        -9, // Upper Left
        -1, // Left
        7, // Down Left
        -8, // Upper
        8, // Down
        -7, // Upper Right
        1, // Right
        9 // Down Right
    };
    BitBoard bound[] = {
        0x007F'7F7F'7F7F'7F7F, // Upper Left
        0x7F7F'7F7F'7F7F'7F7F, // Left
        0x7F7F'7F7F'7F7F'7F00, // Down Left
        0x00FF'FFFF'FFFF'FFFF, // Upper
        0xFFFF'FFFF'FFFF'FF00, // Down
        0x00FE'FEFE'FEFE'FEFE, // Upper Right
        0xFEFE'FEFE'FEFE'FEFE, // Right
        0xFEFE'FEFE'FEFE'FE00 // Down Right
    };

    for (int i = 0; i < 8; i++) {
        if (legalCheck(friendly, enemy, bitLoc, vector[i], bound[i]).any())
            return bitLoc;
    }
    return 0;
}

BitBoard Board::allLegalMove(Team team) const
{
    BitBoard legal = 0;
    for (size_t location = 0; location < 64; location++) {
        legal |= legalMove(location, team);
    }
    return legal;
}

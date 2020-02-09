#include "boardmodel.h"
#include <QColor>
#include <QVector>

BoardModel::BoardModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    QVector<int> role;
    role << Qt::EditRole;
    qRegisterMetaType(&role);
}

void BoardModel::startGame(Board* board)
{
    if (board != this->board)
        this->board = board;
    update();
}

void BoardModel::update()
{
    emit dataChanged(index(0, 0), index(7, 7), QVector<int>() << Qt::EditRole);
}

void BoardModel::newGame()
{
    board = nullptr;
    update();
}

size_t BoardModel::translateLocation(const QModelIndex& index) const
{
    int r = index.row(), c = index.column();
    if (r >= 8 || c >= 8)
        return -1;
    return r * 8 + c;
}

QColor BoardModel::background(const QModelIndex& index) const
{
    // Last Index
    if (lastIndex == index)
        return QColor(100, 150, 60);
    // Next Available
    else if (nextAvailable.test(index.row() * 8 + index.column()))
        return QColor(140, 190, 90);
    // Default
    else
        return QColor(112, 173, 71);
}

QString BoardModel::text(const QModelIndex& index) const
{
    int row = index.row();
    int column = index.column();
    if (!board) {
        if ((row == 3 && column == 4) || (row == 4 && column == 3) || (row == 3 && column == 3) || (row == 4 && column == 4))
            return "●";
        else
            return "";
    }
    if (board->isDisk(row, column))
        return "●";
    else
        return "";
}

QColor BoardModel::textColor(const QModelIndex& index) const
{
    int row = index.row();
    int column = index.column();
    if (!board) {
        if ((row == 3 && column == 4) || (row == 4 && column == 3))
            return QColor(0, 0, 0);
        else if ((row == 3 && column == 3) || (row == 4 && column == 4))
            return QColor(255, 255, 255);
        else
            return QColor();
    }
    if (board->isBlack(index.row(), index.column()))
        return QColor(0, 0, 0);
    else if (board->isWhite(index.row(), index.column()))
        return QColor(255, 255, 255);
    else
        return QColor();
}

QVariant BoardModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // Align Center
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    // Background Color
    if (role == Qt::BackgroundRole)
        return background(index);

    // Text
    if (role == Qt::DisplayRole)
        return text(index);

    // Text Color
    if (role == Qt::TextColorRole)
        return textColor(index);

    return QVariant();
}

int BoardModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 8;
}

int BoardModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 8;
}

void BoardModel::setNextAvailable(BitBoard nextAvailable)
{
    this->nextAvailable = nextAvailable;
    emit dataChanged(index(0, 0), index(7, 7), QVector<int>() << Qt::EditRole);
}

#ifndef BOARDMODEL_H
#define BOARDMODEL_H
#include "board.h"
#include <QAbstractTableModel>
#include <iostream>

Q_DECLARE_METATYPE(QVector<int>)

class BoardModel : public QAbstractTableModel {
    Q_OBJECT

private:
    QModelIndex lastIndex;
    BitBoard nextAvailable;
    QString text(const QModelIndex& index) const;
    QColor textColor(const QModelIndex& index) const;
    QColor background(const QModelIndex& index) const;
    Board* board = nullptr;

public:
    BoardModel(QObject* parent);
    void startGame(Board* board);
    void update();
    void newGame();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    size_t translateLocation(const QModelIndex& index) const;

    void setNextAvailable(BitBoard nextAvailable);
};

#endif // BOARDMODEL_H

#include "panelmodel.h"
#include <QString>

PanelModel::PanelModel(QObject* parent)
    : QAbstractListModel(parent)
{
    data_entry.reserve(INDEX_COUNT);
    for (size_t i = 0; i < INDEX_COUNT; i++)
        data_entry.push_back(QVariant());
}

int PanelModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return INDEX_COUNT;
}

QVariant PanelModel::data(const QModelIndex& index, int /*role*/) const
{
    if (!index.isValid() || index.row() >= data_entry.count())
        return QVariant();
    return data_entry[index.row()];
}

bool PanelModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (index.row() < data_entry.count() || data(index, role) != value) {
        data_entry[index.row()] = value;
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

void PanelModel::startGame()
{
    setData(index(Index::TITLE_TEXT), "Othello");
    setData(index(Index::SELECT_VISIBLE), false);
    setData(index(Index::SCORE_VISIBLE), true);
    setData(index(Index::SCORE_BLACK_TEXT), QString::number(2));
    setData(index(Index::SCORE_WHITE_TEXT), QString::number(2));
}

void PanelModel::update(size_t cntBlack, size_t cntWhite)
{
    setData(index(Index::SCORE_BLACK_TEXT), QString::number(cntBlack));
    setData(index(Index::SCORE_WHITE_TEXT), QString::number(cntWhite));
}
void PanelModel::gameOver(size_t cntBlack, size_t cntWhite)
{
    if (cntBlack < cntWhite)
        setData(index(Index::TITLE_TEXT), "Black Win!!");
    else if (cntBlack == cntWhite)
        setData(index(Index::TITLE_TEXT), "Draw");
    else
        setData(index(Index::TITLE_TEXT), "White Win!!");

    update(cntBlack, cntWhite);
    setData(index(Index::NEW_GAME_BTN_VISIBLE), true);
}

void PanelModel::newGame()
{
    setData(index(Index::TITLE_TEXT), "Welcome to\nOthello Game!!");
    setData(index(Index::NEW_GAME_BTN_VISIBLE), false);
    setData(index(Index::SELECT_VISIBLE), true);
    setData(index(Index::SCORE_VISIBLE), false);
    setData(index(Index::SCORE_BLACK_TEXT), "");
    setData(index(Index::SCORE_WHITE_TEXT), "");
}

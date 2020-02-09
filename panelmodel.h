#ifndef PANELMODEL_H
#define PANELMODEL_H

#include <QAbstractListModel>

class PanelModel : public QAbstractListModel {
    Q_OBJECT
private:
    static constexpr size_t INDEX_COUNT = 6;

public:
    enum Index {
        TITLE_TEXT,
        SCORE_BLACK_TEXT,
        SCORE_WHITE_TEXT,
        SELECT_VISIBLE,
        SCORE_VISIBLE,
        NEW_GAME_BTN_VISIBLE
    };

public:
    explicit PanelModel(QObject* parent = nullptr);
    // Basic functionality:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

    void startGame();
    void update(size_t cntBlack, size_t cntWhite);
    void gameOver(size_t cntBlack, size_t cntWhite);
    void newGame();

private:
    QVector<QVariant> data_entry;
};

#endif // PANELMODEL_H

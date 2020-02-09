#ifndef LOCALGAME_H
#define LOCALGAME_H

#include "ai.h"
#include "boardmodel.h"
#include "game.h"
#include "mainwindow.h"
#include "panelmodel.h"

class LocalGame : public Game {
    Q_OBJECT
private:
    AI* ai;

public:
    LocalGame(BoardModel* bm, PanelModel* pm, AI* ai = nullptr);
    void run() override;
    ~LocalGame() override;
};

#endif // LOCALGAME_H

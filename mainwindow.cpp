#include "mainwindow.h"
#include "ai.h"
#include "localgame.h"
#include "networkdialog.h"
#include "networkgame.h"
#include "ui_mainwindow.h"
#include "user.h"
#include <QDebug>
#include <QThread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , pm(new PanelModel(this))
    , bm(new BoardModel(this))
    , mapper(new QDataWidgetMapper(this))
{
    ui->setupUi(this);

    // Set Model
    ui->boardTable->setModel(bm);
    mapper->setModel(pm);
    mapper->setOrientation(Qt::Orientation::Vertical);

    // Title Text
    mapper->addMapping(ui->titleLabel, PanelModel::Index::TITLE_TEXT, "text");

    // Score Text
    mapper->addMapping(ui->scoreBlackLbl, PanelModel::Index::SCORE_BLACK_TEXT, "text");
    mapper->addMapping(ui->scoreWhiteLbl, PanelModel::Index::SCORE_WHITE_TEXT, "text");

    // Select Button Visibility
    mapper->addMapping(ui->singlePlayBtn, PanelModel::Index::SELECT_VISIBLE, "visible");
    mapper->addMapping(ui->hotseatPlayBtn, PanelModel::Index::SELECT_VISIBLE, "visible");
    mapper->addMapping(ui->networkPlayBtn, PanelModel::Index::SELECT_VISIBLE, "visible");

    // Score Label Visibility
    mapper->addMapping(ui->blackLbl, PanelModel::Index::SCORE_VISIBLE, "visible");
    mapper->addMapping(ui->whiteLbl, PanelModel::Index::SCORE_VISIBLE, "visible");

    // New Game Button Visibility
    mapper->addMapping(ui->newGameBtn, PanelModel::Index::NEW_GAME_BTN_VISIBLE, "visible");

    mapper->toFirst();

    // Initialize
    pm->newGame();
    bm->newGame();
}

MainWindow::~MainWindow()
{
    if (thread && thread->isRunning())
        thread->terminate();
    if (game)
        delete game;
    delete pm;
    delete bm;
    delete mapper;
    delete ui;
}

void MainWindow::on_boardTable_clicked(const QModelIndex& index)
{
    User::pushIndex(index.row() * 8 + index.column());
}

void MainWindow::startGame(MainWindow* mw, bool isAI, std::pair<uint32_t, uint16_t>* server)
{
    try {
        if (server) {
            if (isAI)
                mw->game = new NetworkGame(mw->bm, mw->pm, server->first, server->second, new AI());
            else
                mw->game = new NetworkGame(mw->bm, mw->pm, server->first, server->second, new User(mw->bm));
        } else {
            if (isAI)
                mw->game = new LocalGame(mw->bm, mw->pm, new User(mw->bm), new AI());
            else
                mw->game = new LocalGame(mw->bm, mw->pm, new User(mw->bm), new User(mw->bm));
        }
        mw->game->run();
    } catch (QAbstractSocket::SocketError) {
        qWarning() << "Socet Error";
    }
}

void MainWindow::on_singlePlayBtn_clicked()
{
    if (thread && thread->isRunning()) {
        thread->terminate();
    }
    thread = QThread::create(startGame, this, true, nullptr);
    thread->start();
}

void MainWindow::on_hotseatPlayBtn_clicked()
{
    if (thread && thread->isRunning()) {
        thread->terminate();
    }
    thread = QThread::create(startGame, this, false, nullptr);
    thread->start();
}

void MainWindow::on_networkPlayBtn_clicked()
{
    NetworkDialog dialog;
    if (dialog.exec() != QDialog::Accepted)
        return;

    std::pair<uint32_t, uint16_t> server = dialog.getServer();
    bool isAI = dialog.isAIPlayer();

    if (thread && thread->isRunning()) {
        thread->terminate();
    }
    thread = QThread::create(startGame, this, isAI, &server);
    thread->start();
}

void MainWindow::on_newGameBtn_clicked()
{
    delete game;
    game = nullptr;
}

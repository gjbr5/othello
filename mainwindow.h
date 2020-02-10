#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "boardmodel.h"
#include "game.h"
#include "panelmodel.h"
#include <QDataWidgetMapper>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    Ui::MainWindow* ui;
    PanelModel* pm;
    BoardModel* bm;
    QDataWidgetMapper* mapper;
    QThread* thread = nullptr;
    Game* game = nullptr;
    static void startGame(MainWindow* mw, bool isAI, std::pair<uint32_t, uint16_t>* server);

private slots:
    void on_boardTable_clicked(const QModelIndex& index);
    void on_singlePlayBtn_clicked();
    void on_hotseatPlayBtn_clicked();
    void on_networkPlayBtn_clicked();
    void on_newGameBtn_clicked();

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
};
#endif // MAINWINDOW_H

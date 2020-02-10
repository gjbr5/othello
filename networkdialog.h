#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QDialog>

namespace Ui {
class NetworkDialog;
}

class NetworkDialog : public QDialog {
    Q_OBJECT

public:
    explicit NetworkDialog(QWidget* parent = nullptr);
    ~NetworkDialog();
    std::pair<uint32_t, uint16_t> getServer() const;
    bool isAIPlayer() const;

private:
    Ui::NetworkDialog* ui;
};

#endif // NETWORKDIALOG_H

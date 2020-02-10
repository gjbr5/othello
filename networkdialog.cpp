#include "networkdialog.h"
#include "ui_networkdialog.h"

NetworkDialog::NetworkDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::NetworkDialog)
{
    ui->setupUi(this);
}

NetworkDialog::~NetworkDialog()
{
    delete ui;
}

std::pair<uint32_t, uint16_t> NetworkDialog::getServer() const
{
    uint8_t ipA = ui->ipASpin->value();
    uint8_t ipB = ui->ipBSpin->value();
    uint8_t ipC = ui->ipCSpin->value();
    uint8_t ipD = ui->ipDSpin->value();

    uint32_t ip = (ipA << 24) | (ipB << 16) | (ipC << 8) | (ipD);
    uint16_t port = ui->portSpin->value();
    return std::make_pair(ip, port);
}

bool NetworkDialog::isAIPlayer() const
{
    return ui->aiCheck->isChecked();
}

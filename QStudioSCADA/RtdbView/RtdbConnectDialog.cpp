#include "RtdbConnectDialog.h"
#include <QDebug>
#include "ConfigUtils.h"
#include "Helper.h"
#include "ui_RtdbConnectDialog.h"

RtdbConnectDialog::RtdbConnectDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::RtdbConnectDialog) {
  ui->setupUi(this);
  setWindowTitle(tr("实时数据库连接"));

  QString strFile = Helper::AppDir() + "/Config/Current.ini";
  QString ip = ConfigUtils::getCfgStr(strFile, "Rtdb", "IP", "127.0.0.1");
  ui->editAddress->setText(ip);

  mOpt = -1;
}

RtdbConnectDialog::~RtdbConnectDialog() { delete ui; }

void RtdbConnectDialog::on_editAddress_textChanged(const QString &arg1) {}

void RtdbConnectDialog::on_btnConnect_clicked() {
  WriteConfig();
  this->accept();
  mOpt = 1;
}

void RtdbConnectDialog::on_btnDisConnect_clicked() {
  this->reject();
  mOpt = 0;
}

void RtdbConnectDialog::on_btnExit_clicked() {
  WriteConfig();
  mOpt = -1;
  this->reject();
}

void RtdbConnectDialog::closeEvent(QCloseEvent * /*event*/) {
  qDebug() << "close";
}

void RtdbConnectDialog::WriteConfig() {
  QString strFile = Helper::AppDir() + "/Config/Current.ini";
  ConfigUtils::setCfgStr(strFile, "Rtdb", "IP", ui->editAddress->text());
}

QString RtdbConnectDialog::GetIPAddress() {
  QString ip = ui->editAddress->text();
  if (ip.isEmpty()) ip = "127.0.0.1";
  return ip;
}

int RtdbConnectDialog::GetOption() { return mOpt; }

void RtdbConnectDialog::SetConnectStatus(QString s) { ui->lblMsg->setText(s); }

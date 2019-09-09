#include "MainWindow.h"
#include "ConfigUtils.h"

#include <QApplication>
#include <QString>

#include <QDebug>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString sProjectPath = "";
    if(argc == 2)
    {
        sProjectPath = argv[1];
    }

  if (sProjectPath == "") {
    QString strFile = QCoreApplication::applicationDirPath() + "/lastpath.ini";
    sProjectPath = ConfigUtils::getCfgStr(strFile, "PathInfo", "Path", "C:/");
  }

  if (sProjectPath == "") qApp->exit();

  MainWindow w(sProjectPath);
  w.show();
  w.ShowFirstPage();

  return a.exec();
}

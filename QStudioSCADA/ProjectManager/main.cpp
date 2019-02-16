#include "MainWindow.h"
#include "VersionInfo.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setOrganizationName("JasonWangCtd");
    app.setApplicationName("SCADA ProjectManager");
    app.setApplicationDisplayName("SCADA ProjectManager");
    app.setApplicationVersion(QString("V%1").arg(VER_FILE));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QFont font;
    font.setFamily("宋体");
    font.setPointSize(10);
    app.setFont(font);

    MainWindow win;
    win.show();

    return app.exec();
}

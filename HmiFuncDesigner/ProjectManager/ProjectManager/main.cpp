#include "MainWindow.h"
#include "VersionInfo.h"
#include "qtsingleapplication.h"
#include "ConfigUtils.h"
#include "PluginManager.h"
#include "../../libs/shared/pluginloader.h"
#include "../../libs/shared/qbaseinit.h"
#include "../../libs/core/qbaseinit.h"
#include "../../libs/core/qsoftcore.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QtSingleApplication app(argc, argv);

    if (app.isRunning())
        return !app.sendMessage("project manager start " + app.applicationDirPath());

    app.setOrganizationName("JasonWangCtd");
    app.setApplicationName("ProjectManager");
    app.setApplicationDisplayName("ProjectManager");
    app.setApplicationVersion(QString("V%1").arg(VER_FILE));
    app.setWindowIcon(QIcon(":/images/appicon.png"));

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    QTranslator translator;
    if(translator.load("chinese.qm", QApplication::applicationDirPath()))
        qApp->installTranslator(&translator);

    QFont font;
    font.setFamily("宋体");
    font.setPointSize(10);
    app.setFont(font);

    qRegisterBaseProperty();
    qRegisterBasePropertyEditor();
    qRegisterBaseHost();
    qRegisterBaseDriver();

    PluginLoader::load_plugin(app.applicationDirPath() + "/plugins.xml");
    // 加载元素插件
    PluginManager::getInstance()->loadPlugin();

    MainWindow win;
    app.setActivationWindow(&win);
    win.show();

    int ret = app.exec();

    // 释放插件
    PluginManager::getInstance()->releasePlugin();

    return ret;
}

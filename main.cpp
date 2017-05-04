
#include <QApplication>
#include "logmanager.h"
#include "Logger.h"
//#include "widget.h"
#include "./view/mainwidget.h"
#include <QRect>
#include <QPoint>
#include <QFile>
#include <QLocale>
#include "singleapplication.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

int main(int argc, char *argv[])
{
    //Singlentan process
    SingleApplication app(argc,argv);
    app.setQuitOnLastWindowClosed(false);

    //Logger handle
    LogManager::instance()->debug_log_console_on();

    app.setOrganizationName("deepin");
    app.setApplicationName(QObject::tr("Deepin Shortcut Viewer"));
    app.setApplicationVersion("v1.0");



    QString uniqueKey = app.applicationName();
    bool isSingleApplication = app.setSingleInstance(uniqueKey);

    //Handle singlelentan process communications;
    if (isSingleApplication) {
        SingleApplication::processArgs(app.arguments());

        return app.exec();
    } else {
        QByteArray array;

        for (const QString &arg : app.arguments())
            array.append(arg.toLocal8Bit()).append('\0');

        app.newClientProcess(uniqueKey, array);

        return 0;
    }
}

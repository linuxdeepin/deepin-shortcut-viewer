// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include <QApplication>
#include <DLog>
//#include "widget.h"
#include "./view/mainwidget.h"
#include <DGuiApplicationHelper>
#include <QRect>
#include <QPoint>
#include <QFile>
#include <QLocale>
#include "singleapplication.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    //Singlentan process
    SingleApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);

    app.setOrganizationName("deepin");
    app.setApplicationName("deepin-shortcut-viewer");
    app.setProductName(QObject::tr("Deepin Shortcut Viewer"));
    app.setApplicationDisplayName(QObject::tr("Deepin Shortcut Viewer"));
    app.setApplicationVersion("v1.0");

    // The viewer is a transient popup: its theme must follow the system theme
    // or the --theme passed by the caller, but must never persist that
    // transient choice to the user's DTK theme preference (org.deepin.dtk.
    // preference). Without this, a caller passing --theme=light would write
    // "light" to the dconfig and the viewer would later start in light theme
    // regardless of the actual system theme. This also keeps the
    // setPaletteType() call in MainWidget::setThemeName non-persistent.
    DGuiApplicationHelper::setAttribute(DGuiApplicationHelper::DontSaveApplicationTheme, true);

    //Logger handle
    DLogManager::registerConsoleAppender();

    QString uniqueKey = app.applicationName();
    bool isSingleApplication = app.setSingleInstance(uniqueKey);

    //Handle singlelentan process communications;
    if (isSingleApplication) {
        app.processArgs(app.arguments());

        return app.exec();
    } else {
        QByteArray array;

        for (const QString &arg : app.arguments())
            array.append(arg.toLocal8Bit()).append('\0');

        app.newClientProcess(uniqueKey, array);

        return 0;
    }
}

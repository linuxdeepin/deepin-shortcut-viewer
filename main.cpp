/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <DLog>
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

DCORE_USE_NAMESPACE

int main(int argc, char *argv[])
{
    //Singlentan process
    SingleApplication::loadDXcbPlugin();
    SingleApplication app(argc,argv);
    app.setQuitOnLastWindowClosed(false);

    //Logger handle
    DLogManager::registerConsoleAppender();

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

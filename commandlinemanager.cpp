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

#include "commandlinemanager.h"
#include <QFile>
#include <QRect>
#include <QDesktopWidget>
#include <QDebug>
CommandLineManager::CommandLineManager():
    m_posOption(QStringList()<<"p"<<"pos",
                 QCoreApplication::translate("main","Expose your window's central position(x,y) info where the deepin-shorcut-viwer can be center int your window relatively, otherwise it will be center in the current screen. eg: -p=100,50 or --pos=900,50"),
                 " "),
    m_jsonDataOption(QStringList()<<"j"<<"json-data",
                     QCoreApplication::translate("main","Directly convert a json data to this program. See https://github.com/linuxdeepin/deepin-shortcut-viewer or docs provided with the program for more information of this decscription"),
                     " ")
{
    m_commandLineParser.setApplicationDescription("Test helper");
    m_commandLineParser.addHelpOption();
    m_commandLineParser.addVersionOption();
    m_commandLineParser.addOption(m_posOption);
    m_commandLineParser.addOption(m_jsonDataOption);
    m_commandLineParser.addOption(QCommandLineOption(QStringList() << "b" << "bypass", "Enable bypass window manager hint"));
}
void CommandLineManager::process(const QCoreApplication &app)
{
    m_commandLineParser.process(app);
}

void CommandLineManager::process(const QStringList &list)
{
    m_commandLineParser.process(list);
}

QString CommandLineManager::jsonData(){
    return m_commandLineParser.value(m_jsonDataOption);
}

bool CommandLineManager::enableBypassWindowManagerHint() const
{
    return m_commandLineParser.isSet("b");
}

QPoint CommandLineManager::pos(){
    QString posStr = m_commandLineParser.value(m_posOption);

    QStringList posStrs = posStr.split(",");

    QPoint pos(qApp->desktop()->width()/2,qApp->desktop()->height()/2);
    if(posStrs.length()==2){
        pos.setX(QString(posStrs[0]).toInt());
        pos.setY(QString(posStrs[1]).toInt());
    }
    return pos;
}

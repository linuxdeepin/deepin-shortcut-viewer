#include "commandlinemanager.h"
#include <QFile>
#include <QRect>
#include <QDesktopWidget>
#include <QDebug>
CommandLineManager::CommandLineManager():
    m_targetOption(QStringList()<<"t"<<"target",
                   QCoreApplication::translate("main", "Your Application name .eg: -t=dde-file-manager/--target=dde-file-manager"),
                   QCoreApplication::translate("main", " ")),
    m_rectOption(QStringList()<<"r"<<"rect",
                 QCoreApplication::translate("main","Expose your  window's rect(x,y,w,h) info where the deepin-shorcut-viwer can center int your window relativly,otherwise it will center in the current screen. eg: -r=100,50,900,500/--rect=100,50,900,50"),
                 " "),
    m_jsonDataOption(QStringList()<<"j"<<"json-data",
                     QCoreApplication::translate("main","Directly convert a json data to this program ,see https://github.com/tommego/deepin-shortcut-viewer  for more information of this decscription"),
                     " ")
{
    m_commandLineParser.setApplicationDescription("Test helper");
    m_commandLineParser.addHelpOption();
    m_commandLineParser.addVersionOption();
    m_commandLineParser.addOption(m_targetOption);
    m_commandLineParser.addOption(m_rectOption);
    m_commandLineParser.addOption(m_jsonDataOption);
}
void CommandLineManager::process(QApplication &app){
    m_commandLineParser.process(app);
}

QString CommandLineManager::dir(){

    qDebug()<<"target option value:"<<m_commandLineParser.value(m_targetOption);
    qDebug()<<"rect option value:"<<m_commandLineParser.value(m_rectOption);

    return m_commandLineParser.value(m_targetOption);
}
QString CommandLineManager::jsonData(){
    return m_commandLineParser.value(m_jsonDataOption);
}

QPoint CommandLineManager::pos(){
    QString posStr = m_commandLineParser.value(m_rectOption);

    QStringList posStrs = posStr.split(",");

    QRect *rect=NULL;
    if(posStrs.length()==4)
    rect=new QRect(QString(posStrs[0]).toInt(),
                   QString(posStrs[1]).toInt(),
                   QString(posStrs[2]).toInt(),
                   QString(posStrs[3]).toInt());
    QPoint pos;
    if(rect){
        pos.setX(rect->x()+rect->width()/2);
        pos.setY(rect->y()+rect->height()/2);
    }
    else{
        pos.setX(qApp->desktop()->width()/2);
        pos.setY(qApp->desktop()->height()/2);
    }
    return pos;
}

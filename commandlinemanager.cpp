#include "commandlinemanager.h"
#include <QFile>
#include <QRect>
#include <QDesktopWidget>

CommandLineManager::CommandLineManager()
{
    m_commandLineParser.setApplicationDescription("Test helper");
    m_commandLineParser.addHelpOption();
    m_commandLineParser.addVersionOption();
    m_commandLineParser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to startup.eg:/home/deepin/shortcut.txt"));
    m_commandLineParser.addPositionalArgument("window state", QCoreApplication::translate("main", "Use format [x,y,w,h] to localize shortcut's position.eg:100,50,900,500"));

}
void CommandLineManager::process(QApplication &app){
    m_commandLineParser.process(app);
    args();
}
QStringList CommandLineManager::args(){
    m_strls = m_commandLineParser.positionalArguments();
    if(m_strls.length()>0)
        m_dir=m_strls[0];
    return m_commandLineParser.positionalArguments();
}
QString CommandLineManager::dir(){
    return m_dir;
}
QPoint CommandLineManager::pos(){
    QString posStr;
    if(m_strls.length()==2)
        posStr=m_strls.at(1);

    QStringList posStrs=posStr.split(",");

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

#include "shortcutscene.h"
#include <QtMath>

ShortcutScene::ShortcutScene(QObject *parent)
    :QGraphicsScene(parent)
{
    initData();
}
void ShortcutScene::initUI(){
    for(int a = 0; a<6;a++){
        m_listTextItems[a] = new QList<QGraphicsTextItem*>();
    }

    //init text items and cols data
    for(int i = 0 ;i<3;i++){
        int nl=0;
        int vl=0;
        foreach (Shortcut data, *m_displayShortcutLists[i]) {
            QGraphicsTextItem *nametextItem = new QGraphicsTextItem;
            QGraphicsTextItem *valueTextItem = new QGraphicsTextItem;
            if(data.value=="type"){
                QFont font;
                font.setWeight(54);
                font.setPixelSize(18);
                nametextItem->setFont(font);
                valueTextItem->setFont(font);
                valueTextItem->setPlainText("");
            }
            else{
                QFont font;
                font.setPixelSize(13);
                nametextItem->setFont(font);
                valueTextItem->setFont(font);
                valueTextItem->setPlainText(data.value);
            }

            nametextItem->setPlainText(data.name);
            nametextItem->setDefaultTextColor(Qt::white);
            valueTextItem->setDefaultTextColor(Qt::white);
            addItem(nametextItem);
            addItem(valueTextItem);
            m_listTextItems[i*2]->append(nametextItem);
            m_listTextItems[i*2+1]->append(valueTextItem);
            if(data.nameLength>nl)
                nl=data.nameLength;
            if(data.valueLength>vl)
                vl=data.valueLength;
//            qDebug()<<"nl:"<<data.nameLength<<", vl:"<<data.valueLength;
        }
        m_cols[i*2].width = nl;
        m_cols[i*2+1].width = vl;
//        qDebug()<<nl<<","<<vl;
    }

    //allocation text items
    m_cols[0].x=m_startx;
    for(int a=1;a<6;a++){
        if(a%2==0)
            m_cols[a].x=m_cols[a-1].x+m_cols[a-1].width+40;
        else
            m_cols[a].x=m_cols[a-1].x+m_cols[a-1].width+35;
    }

    for(int a=0;a<6;a++){
        int index=0;
        foreach (QGraphicsTextItem *item, *m_listTextItems[a]) {
            int x = m_cols[a].x;
            int y=m_displayShortcutLists[(int)a/2*1]->at(index).y;
            item->setPos(x,y);
            index++;
        }
    }
//    setSceneRect(44,44,m_maxContentWidth,m_maxContentHeigth);

}
void ShortcutScene::initData(){
    m_maxContentHeigth = qApp->desktop()->height()*0.8-99;
    m_maxContentWidth = qApp->desktop()->width()*0.8-99;

    m_displayShortcutLists[0] = new QList<Shortcut>();
    m_displayShortcutLists[1] = new QList<Shortcut>();
    m_displayShortcutLists[2] = new QList<Shortcut>();
}
void ShortcutScene::sortData(){
    if(!m_shortcutList.count()>0)
        return;
    int index = 0;

    for(int i=0;i<m_shortcutList.count();i++){
        if(index==3){//out of col range
            qDebug()<<"out of range";
            return;
            }
//        qDebug()<<index;
        m_displayShortcutLists[index]->append(m_shortcutList.at(i));

        //caculate current col's length
        int colLength=0;
        for (int j=0;j<m_displayShortcutLists[index]->length();j++) {
            Shortcut data=m_displayShortcutLists[index]->at(j);
            if(data.value=="type"&&j==0){
                colLength+=18;
                data.y=m_starty;
                m_displayShortcutLists[index]->replace(j,data);
            }

            else if(data.value=="type"&&j>0){
                colLength+=58;
                data.y=m_starty+colLength-18;
                m_displayShortcutLists[index]->replace(j,data);
            }
            else if(data.value!="type"&&j==0){
                colLength+=13;
                data.y=m_starty;
                m_displayShortcutLists[index]->replace(j,data);
            }
            else if(data.value!="type"&&j>0){
                colLength+=27;
                data.y=m_starty+colLength-13;
                m_displayShortcutLists[index]->replace(j,data);
            }
        }

        if(colLength==m_maxContentHeigth){
            colLength=0;
            index++;
        }
        else if(colLength>m_maxContentHeigth){
            colLength=0;
            index++;
        }
    }

}

void ShortcutScene::loadFile(QString file){
    QString str;
    QFile f(file);

    if(f.exists()){
        if(f.open(QIODevice::ReadOnly))
            str=f.readAll();
        else
            str="[Tips] \n"
                "Setp 1:=Shortcut folder should be named as your-app-name \n"
                "Setp 2:=Please copy shortcut file to /usr/share/dde-shortcut-viewer/your-app-name \n"
                "Setp 3:=Different language files should be copied to related folder eg(chinise)./ \n"
                " *=your-app-name/ZH_ch/shortcut.txt,folder archs is: \n"
                " =your-app-name/ZH_ch/shortcut.txt \n"
                " =your-app-name/EN_us/shortcut.txt \n"
                " =your-app-name/.../shorcut.txt \n"
                "Step 4:=To run this app,just use command line :dde-shortcut-viewer your-app-name \n"
                " =eg:dde-shortcut-viewer dde-file-manager (defaut it will be center in the screen) \n"
                " =or dde-shortcut-viewer  your-app-name your-app-rect-to-center-in \n "
                " =eg:dde-shortcut-viewer dde-file-manager 300,200,700,500 (it will center mustomized) \n"
                " *=About your-app-rect-to-center-in is a format as:x,y,w,h eg:10,10,20,20 \n";    }
    else
        str="[Tips] \n"
            "Setp 1:=Shortcut folder should be named as your-app-name \n"
            "Setp 2:=Please copy shortcut file to /usr/share/dde-shortcut-viewer/your-app-name \n"
            "Setp 3:=Different language files should be copied to related folder eg(chinise)./ \n"
            " *=your-app-name/ZH_ch/shortcut.txt,folder archs is: \n"
            " =your-app-name/ZH_ch/shortcut.txt \n"
            " =your-app-name/EN_us/shortcut.txt \n"
            " =your-app-name/.../shorcut.txt \n"
            "Step 4:=To run this app,just use command line :dde-shortcut-viewer your-app-name \n"
            " =eg:dde-shortcut-viewer dde-file-manager (defaut it will be center in the screen) \n"
            " =or dde-shortcut-viewer  your-app-name your-app-rect-to-center-in \n "
            " =eg:dde-shortcut-viewer dde-file-manager 300,200,700,500 (it will center mustomized) \n"
            " *=About your-app-rect-to-center-in is a format as:x,y,w,h eg:10,10,20,20 \n";

    f.close();
    QStringList strls=str.split("\n");

    foreach (QString it, strls) {
        if(it.contains("[")){
            QString str=it.mid(it.indexOf("[")+1,it.indexOf("]")-it.indexOf("[")-1);
            Shortcut sc;
            sc.name=str;
            sc.value="type";
            QFont font;
            font.setPixelSize(18);
            QFontMetrics fm(font);
            sc.nameLength=fm.width(str);
            sc.valueLength=0;
            m_shortcutList.append(sc);
        }
        else if(it.contains("=")){
            QStringList data=it.split("=");
            if(data.length()==2){
                Shortcut sc;
                sc.name=data[0];
                sc.value=data[1];
                QFont font;
                font.setPixelSize(13);
                QFontMetrics fm(font);
                sc.nameLength=fm.width(sc.name);
                sc.valueLength=fm.width(sc.value);
                m_shortcutList.append(sc);
            }
        }
    }
    foreach (Shortcut sc, m_shortcutList) {
        if(sc.value!="type")
            continue;
        qDebug()<<"nl:"<<sc.nameLength<<", vl:"<<sc.valueLength<<",n:"<<sc.name<<",v:"<<sc.value;
    }
    sortData();
    initUI();
}
void ShortcutScene::focusOutEvent(QFocusEvent *event){
    if(event->lostFocus())
        qApp->quit();
}

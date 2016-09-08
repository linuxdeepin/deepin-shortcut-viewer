#include "shortcutscene.h"
#include <QtMath>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QJsonObject>

ShortcutScene::ShortcutScene(QObject *parent)
    :QGraphicsScene(parent)
{
    initData();
//    setFocus(Qt::MouseFocusReason);
}
ShortcutScene::ShortcutScene(QObject *parent, QString data ,int flag)
    :QGraphicsScene(parent){
    if(flag == 0){
        QString dir = data;
        initData();
//        setFocus(Qt::MouseFocusReason);
        loadFile(dir);
    }
    else if (flag ==1){
        QString jsonData = data;
        initData();
        setFocus(Qt::MouseFocusReason);
        QByteArray data;
        data.append(jsonData);
        QJsonParseError jsError;
        QJsonDocument doc = QJsonDocument::fromJson(data,&jsError);
        QJsonObject obj;
        QJsonArray shortcuts;
        if(jsError.error == QJsonParseError::NoError){
            if(doc.isObject()){
                obj = doc.object();
                if(obj.contains("shortcut")){
                    QJsonValue value;
                    value = obj.value("shortcut");
                    if(value.isArray()){
                        shortcuts = value.toArray();
                    }
                }
            }
        }
        else {
            qDebug()<<"Json data parsing error:"<<jsError.error<<jsError.errorString();
            return;
        }

        foreach (QJsonValue value, shortcuts) {
            QJsonObject obj = value.toObject();
            Shortcut sc;
            sc.name=obj["groupName"].toString();
            sc.value="type";
            QFont font;
            font.setPixelSize(18);
            QFontMetrics fm(font);
            sc.nameLength=fm.width(sc.name);
            sc.valueLength=0;
            m_shortcutList.append(sc);
            QJsonArray items = obj["groupItems"].toArray();
            foreach(QJsonValue itemvalue ,items){
                QJsonObject item = itemvalue.toObject();
                Shortcut sc1;
                sc1.name = item["name"].toString();
                sc1.value = item["value"].toString();
                QFont font;
                font.setPixelSize(13);
                QFontMetrics fm(font);
                sc1.nameLength=fm.width(sc1.name);
                sc1.valueLength=fm.width(sc1.value);
                m_shortcutList.append(sc1);
            }
        }

        sortData();
        initUI();
        setSceneRect(0,0,this->sceneRect().width(),this->sceneRect().height());
    }
    else
        return;


}
//ShortcutScene::ShortcutScene(QObject *parent, QString url)
//    :QGraphicsScene(parent){
//    initData();
//    setFocus(Qt::MouseFocusReason);
//    loadFile(url);
//}

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
            valueTextItem->setDefaultTextColor(QColor(255,255,255,0.8*255));
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
//    setSceneRect(0,0,m_maxContentWidth,m_maxContentHeigth);


}
void ShortcutScene::initData(){
    m_maxContentHeigth = qApp->desktop()->height()*0.8;
    m_maxContentWidth = qApp->desktop()->width()*0.8-150;

    m_displayShortcutLists[0] = new QList<Shortcut>();
    m_displayShortcutLists[1] = new QList<Shortcut>();
    m_displayShortcutLists[2] = new QList<Shortcut>();
}
void ShortcutScene::sortData(){
    int offset = m_shortcutList.count()%3;
    int average = (int)m_shortcutList.count()/3;

    int i=0;
    int colLsength[3]={0,0,0};
    for(auto it:m_shortcutList){
        Shortcut preData;
        if(i%average>0)
            preData=m_shortcutList.at(i-1);
        //prehandle first column of whether the last item is a type,and tansfer it to next column
        if(i==(average+offset-1)&&it.value=="type"){
            colLsength[1]+=18;
            it.y=m_starty;
            m_displayShortcutLists[1]->append(it);
        }
        else{
            if(i<average+offset){//first col
                if(i==0&&it.value=="type"){
                    colLsength[0]+=18;
                    it.y=m_starty;
                    m_displayShortcutLists[0]->append(it);
                }
                else if(it.value=="type"&&i>0){
                    colLsength[0]+=58;
                    it.y=m_starty+colLsength[0]-18;
                    m_displayShortcutLists[0]->append(it);
                }
                else if(it.value!="type"&&i==0){
                    colLsength[0]+=13;
                    it.y=m_starty;
                    m_displayShortcutLists[0]->append(it);
                }
                else if(it.value!="type"&&i>0&&preData.value!="type"){
                    colLsength[0]+=29;
                    it.y=m_starty+colLsength[0]-13;
                    m_displayShortcutLists[0]->append(it);
                }
                else if(it.value!="type"&&i>0&&preData.value=="type"){
                    colLsength[0]+=33;
                    it.y=m_starty+colLsength[0]-13;
                    m_displayShortcutLists[0]->append(it);
                }
            }
            else{
                //prehandle second column of whether the last item is a type,and transfer it ot next column
                if((i-offset)%average==(average-1)&&it.value=="type"){
                    colLsength[2]+=18;
                    it.y=m_starty;
                    m_displayShortcutLists[2]->append(it);
                }
                else{
                    if((i-offset)%average==0&&it.value=="type"){
                        colLsength[(i-offset)/average]+=18;
                        it.y=m_starty;
                        m_displayShortcutLists[(i-offset)/average]->append(it);
                    }
                    else if(it.value=="type"&&(i-offset)%average>0){
                        colLsength[(i-offset)/average]+=58;
                        it.y=m_starty+colLsength[(i-offset)/average]-18;
                        m_displayShortcutLists[(i-offset)/average]->append(it);
                    }
                    else if(it.value!="type"&&(i-offset)%average==0){
                        if(colLsength[(i-offset)/average]>0){
                            colLsength[(i-offset)/average]+=29;
                            it.y=m_starty+colLsength[(i-offset)/average]-13;
                        }
                        else{
                            it.y=m_starty;
                            colLsength[(i-offset)/average]+=13;
                        }

                        m_displayShortcutLists[(i-offset)/average]->append(it);
                    }
                    else if(it.value!="type"&&(i-offset)%average>0&&preData.value!="type"){
                        colLsength[(i-offset)/average]+=29;
                        it.y=m_starty+colLsength[(i-offset)/average]-13;
                        m_displayShortcutLists[(i-offset)/average]->append(it);
                    }
                    else if(it.value!="type"&&(i-offset)%average>0&&preData.value=="type"){
                        colLsength[(i-offset)/average]+=33;
                        it.y=m_starty+colLsength[(i-offset)/average]-13;
                        m_displayShortcutLists[(i-offset)/average]->append(it);
                    }
                }

            }

        }

        i++;
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
                "Shortcut path: = /usr/share/deepin-shortcut-viewer\n"
                "Command: = deepin-shortcut-viewer <-h> appName order rect\n"
                "appName:= is your install app name\n"
                "order: = open or close \n"
                "rect: =x,y,w,h eg:10,10,100,100\n";
    }
    else
        str="[file not found] \n"
            "[Tips] \n"
            "Shortcut path: = /usr/share/deepin-shortcut-viewer\n"
            "Command: = deepin-shortcut-viewer <-h> appName order rect\n"
            "appName:= is your install app name\n"
            "order: = open or close \n"
            "rect: =x,y,w,h eg:10,10,100,100\n";

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
    }
    sortData();
    initUI();
    setSceneRect(0,0,this->sceneRect().width(),this->sceneRect().height());
}

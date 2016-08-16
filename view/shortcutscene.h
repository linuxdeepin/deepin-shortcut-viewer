#ifndef SHORTCUTSCENE_H
#define SHORTCUTSCENE_H
#include <QGraphicsScene>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QList>
#include <QGraphicsTextItem>
#include <QFontMetrics>
#include <QFile>
#include <QFocusEvent>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

struct Shortcut{
    QString name;
    QString value;
    int nameLength;
    int valueLength;
    int y;
};
struct Column{
    int x=0;
    int width=0;
};

class ShortcutScene : public QGraphicsScene
{
public:
    ShortcutScene(QObject *parent);
    ShortcutScene(QObject *parent , QString data , int flag);
    void        initUI();
    void        initData();
    void        sortData();
    void        loadFile(QString file);

private:
    QList<Shortcut>*            m_displayShortcutLists[3];
    QList<Shortcut>             m_shortcutList;
    int                         m_maxContentHeigth=0;
    int                         m_maxContentWidth=0;
    QList<QGraphicsTextItem*>*  m_listTextItems[6];
    Column                      m_cols[6];
    const int                   m_startx=0;
    const int                   m_starty=0;
};

#endif // SHORTCUTSCENE_H

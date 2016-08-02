#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsView>
#include <QVBoxLayout>
#include "shortcutscene.h"
#include <QPixmap>
#include <QImage>
#include <QDialog>
//#include <QMargins>
//#include "dwi"

class MainWidget : public QDialog
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    explicit MainWidget(QWidget *parent,QString url);

signals:

protected:
    void paintEvent(QPaintEvent*) override;
    bool event(QEvent *e);
public slots:
private:
    void initUI();
    QImage drawShadow(const QPixmap &px, qreal radius, const QColor &color = Qt::black, QSize size = QSize());
    void drawShadowPixmap();
    QGraphicsView *m_mainView;
    QVBoxLayout *m_mainLayout;
    ShortcutScene *m_scene;
    QString m_url;
    QPixmap m_shadowPixmap;
    int m_shadowRadius=10;
//    QMargins m_margins;

};

#endif // MAINWIDGET_H

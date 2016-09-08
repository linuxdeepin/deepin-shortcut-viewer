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
#define CONTENT_MARGINS 20

class MainWidget : public QDialog
{
    Q_OBJECT
public:
     MainWidget(QWidget *parent = 0);
     MainWidget(QWidget *parent = 0 ,QString data = "" ,int flag = 0);

signals:

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void focusInEvent(QFocusEvent *e);

private:
    void initUI();
    QImage              drawShadow(const QPixmap &px, qreal radius, const QColor &color = Qt::black, QSize size = QSize());
    void                drawShadowPixmap();
    QGraphicsView       *m_mainView;
    QVBoxLayout         *m_mainLayout;
    ShortcutScene       *m_scene;
    QPixmap             m_shadowPixmap;
    int                 m_shadowRadius=20;

};

#endif // MAINWIDGET_H

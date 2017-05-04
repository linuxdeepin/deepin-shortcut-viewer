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

    void setJsonData(const QString &data, int flag = 0);

protected:
    void paintEvent(QPaintEvent*) override;

    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

protected:
    void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;

private:
    void initUI();
    QImage drawShadow(const QPixmap &px, qreal radius, const QColor &color = Qt::black, QSize size = QSize());
    void drawShadowPixmap();
    QGraphicsView *m_mainView;
    QVBoxLayout *m_mainLayout;
    ShortcutScene *m_scene = Q_NULLPTR;
    QPixmap m_shadowPixmap;
    int m_shadowRadius=20;

};

#endif // MAINWIDGET_H

// SPDX-FileCopyrightText: 2022 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <dabstractdialog.h>

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
#define CONTENT_MARGINS 0

DWIDGET_USE_NAMESPACE

class MainWidget : public DAbstractDialog
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);

    void setJsonData(const QString &data, int flag = 0);

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
    void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;

protected:
    void showEvent(QShowEvent *e) Q_DECL_OVERRIDE;
    void hideEvent(QHideEvent *e) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *e) override;

private:
    void initUI();

    QGraphicsView *m_mainView;
    QVBoxLayout *m_mainLayout;
    ShortcutScene *m_scene = Q_NULLPTR;
};

#endif // MAINWIDGET_H

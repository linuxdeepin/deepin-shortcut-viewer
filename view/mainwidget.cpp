#include "mainwidget.h"

#include <DPlatformWindowHandle>
#include <DApplication>

MainWidget::MainWidget(QWidget *parent) : DAbstractDialog(parent)
{
    initUI();
}

void MainWidget::setJsonData(const QString &data, int flag)
{
    if (m_scene)
        m_scene->deleteLater();

    m_scene = new ShortcutScene(this, data, flag);
    m_mainView->setScene(m_scene);
    m_mainView->resize(m_scene->sceneRect().width()+88,m_scene->sceneRect().height()+80);
    setFixedSize(m_mainView->size().width()+CONTENT_MARGINS*2,m_mainView->size().height()+CONTENT_MARGINS*2);
}

void MainWidget::initUI(){

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainView = new QGraphicsView(this);

    m_mainView->setStyleSheet("background: transparent;");
    m_mainView->setFrameShape(QFrame::NoFrame);
    m_mainView->setObjectName("MainView");
    m_mainLayout->addWidget(m_mainView);

//    m_scene=new ShortcutScene(this);
//    m_scene->loadFile(m_url);

    setLayout(m_mainLayout);
    setContentsMargins(CONTENT_MARGINS,CONTENT_MARGINS,CONTENT_MARGINS,CONTENT_MARGINS);
    setBackgroundColor(QColor(0, 0, 0, 255 * 0.7));

    if (DApplication::isDXcbPlatform()) {
        DPlatformWindowHandle handle(this);

        handle.setBorderWidth(2);
        handle.setBorderColor(QColor(255, 255, 255, 255 * 0.15));
    }
}

void MainWidget::mousePressEvent(QMouseEvent *e){
    hide();
    DAbstractDialog::mousePressEvent(e);
}

void MainWidget::keyReleaseEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Control || e->key() == Qt::Key_Shift) {
        releaseKeyboard();
        DAbstractDialog::keyReleaseEvent(e);
        hide();
    }
}

void MainWidget::focusInEvent(QFocusEvent *e)
{
    grabKeyboard();
    DAbstractDialog::focusInEvent(e);
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Escape || e->key() == Qt::Key_Back)
        hide();
}

void MainWidget::showEvent(QShowEvent *e)
{
    DAbstractDialog::showEvent(e);

    setFocus(Qt::MouseFocusReason);
    grabKeyboard();
}

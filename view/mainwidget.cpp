#include "mainwidget.h"
#include <QtMath>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    initUI();
}
MainWidget::MainWidget(QWidget *parent, QString url):QWidget(parent){
    m_url=url;
    initUI();


}

void MainWidget::initUI(){
    setMaximumWidth(qApp->desktop()->width()*0.8);
    setMaximumHeight(qApp->desktop()->height()*0.8);
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground, true);
    resize(maximumSize());

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->setMargin(0);
    m_mainView = new QGraphicsView(this);

    m_mainView->setObjectName("MainView");
    m_mainView->resize(size());
    m_mainLayout->addWidget(m_mainView);

    m_scene=new ShortcutScene(this);
    m_scene->loadFile(m_url);
    m_mainView->setScene(m_scene);
    m_mainView->resize(m_scene->sceneRect().width()+88,m_scene->sceneRect().height()+88);
    resize(m_mainView->size());
    setLayout(m_mainLayout);
    this->setStyleSheet("QGraphicsView#MainView{"
                            "background:rgba(0,0,0,0.7);"
                            "border-radius: 4px 4px 4px 4px;"
                            "border: 2px solid rgba(255,255,255,0.4);"
                        "}");
    setContentsMargins(11,11,11,11);
    drawShadowPixmap();
    qDebug()<<"*****************************"<< (m_scene->sceneRect());
}


QT_BEGIN_NAMESPACE
//extern Q_WIDGETS_EXPORT void qt_blurImage(QImage &blurImage, qreal radius, bool quality, int transposed = 0);
extern Q_WIDGETS_EXPORT void qt_blurImage(QPainter *p, QImage &blurImage, qreal radius, bool quality, bool alphaOnly, int transposed = 0);
QT_END_NAMESPACE
QImage MainWidget::drawShadow(const QPixmap &px, qreal radius, const QColor &color, QSize size){
    if (px.isNull())
        return QImage();

    if (!size.isValid())
        size = px.size();

    QImage tmp(size, QImage::Format_ARGB32_Premultiplied);
    tmp.fill(0);
    QPainter tmpPainter(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_Source);
    tmpPainter.drawPixmap(QPoint(radius, radius), px.scaled(QSize(size.width() - 2 * radius, size.height() - 2 * radius)));
    tmpPainter.end();

    // blur the alpha channel
    QImage blurred(tmp.size(), QImage::Format_ARGB32_Premultiplied);
    blurred.fill(0);
    QPainter blurPainter(&blurred);
    qt_blurImage(&blurPainter, tmp, radius, false, true);
    blurPainter.end();

    if (color == QColor(Qt::black))
        return blurred;

    tmp = blurred;

    // blacken the image...
    tmpPainter.begin(&tmp);
    tmpPainter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    tmpPainter.fillRect(tmp.rect(), color);
    tmpPainter.end();

    return tmp;
}

void MainWidget::drawShadowPixmap(){
    QPixmap pixmap(size());

    pixmap.fill(QColor(0,0,0,127));

    m_shadowPixmap = QPixmap::fromImage(drawShadow(pixmap, m_shadowRadius));

}
void MainWidget::paintEvent(QPaintEvent *){

    QPainter painter(this);
    painter.drawPixmap(0, 2, m_shadowPixmap);

}

#include "mainwidget.h"
#include <QtMath>

MainWidget::MainWidget(QWidget *parent) : QDialog(parent)
{
    initUI();
}
MainWidget::MainWidget(QWidget *parent, QString url):QDialog(parent){
    m_url=url;
    initUI();


}

void MainWidget::initUI(){
//    setMaximumWidth(qApp->desktop()->width()*0.8);
//    setMaximumHeight(qApp->desktop()->height()*0.8);
    setWindowFlags(Qt::FramelessWindowHint|Qt::SplashScreen|Qt::Popup|Qt::WindowStaysOnTopHint);
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
    m_mainView->resize(m_scene->sceneRect().width()+88,m_scene->sceneRect().height()+80);    this->resize(m_mainView->size().width()+22,m_mainView->size().height()+22);
    setLayout(m_mainLayout);
    this->setStyleSheet("QGraphicsView#MainView{"
                            "background:rgba(0,0,0,0.7);"
                            "border-radius: 4px 4px 4px 4px;"
//                            "border: 2px solid rgba(255,255,255,0.15);"
                        "}");
    setContentsMargins(11,11,11,11);
    drawShadowPixmap();
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
    QPixmap pixmap(size()+QSize(6,3));

    pixmap.fill(QColor(0,0,0,80));

    m_shadowPixmap = QPixmap::fromImage(drawShadow(pixmap, m_shadowRadius));

}
void MainWidget::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.drawPixmap(-3, 0, m_shadowPixmap);
    QPen pen;
    pen.setWidth(2);
    QColor color(255,255,255,255*0.15);
    pen.setColor(color);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setStyle(Qt::SolidLine);
    pen.setCosmetic(true);
    painter.setPen(pen);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.drawRoundedRect(10,10,m_mainView->width()+2,m_mainView->height()+2,5,5);

}

bool MainWidget::event(QEvent *e){
    QDialog::event(e);
    if(e->type()==QEvent::WindowDeactivate)
        qApp->quit();
}

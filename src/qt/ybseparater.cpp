#include "ybseparater.h"

#include <QPainter>

YbSeparater::YbSeparater(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedHeight(2);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background, QColor(255, 255, 255));
    this->setPalette(pa);
}

void YbSeparater::paintEvent(QPaintEvent *e)
{
    drawLine();
}

void YbSeparater::drawLine()
{
    QPainter painter(this);
    QPen pen;
    pen.setColor(QColor(211, 211, 211));
    painter.setPen(pen);
    QPoint p1(0, this->height() / 2);
    QPoint p2(this->width(), this->height() / 2);
    QLine line(p1, p2);
    painter.drawLine(line);
}

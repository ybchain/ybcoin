#include "ybstackedwidget.h"

YbStackedWidget::YbStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);
}

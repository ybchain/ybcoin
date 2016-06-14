#include "ybstackedwidget.h"
#include "yboverviewpage.h"

YbStackedWidget::YbStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    this->setMinimumHeight(500);
    this->setMinimumWidth(700);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);

    createWidget();
    this->setCurrentWidget(overviewPage);
}

void YbStackedWidget::createWidget()
{
    overviewPage = new YbOverviewPage;
    addWidget(overviewPage);
}

#ifndef YBSTACKEDWIDGET_H
#define YBSTACKEDWIDGET_H

#include <QStackedWidget>

class YbOverviewPage;

class YbStackedWidget : public QStackedWidget
{
public:
    YbStackedWidget(QWidget *parent = 0);

private:
    YbOverviewPage *overviewPage;

    void createWidget();
};

#endif // YBSTACKEDWIDGET_H

#include "ybhelppage.h"
#include "ybtabwidget.h"

#include <QVBoxLayout>

YbHelpPage::YbHelpPage(QWidget *parent) :
    QWidget(parent)
{
    ybTabWidget = new YbTabWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(ybTabWidget);
    setLayout(mainLayout);
}

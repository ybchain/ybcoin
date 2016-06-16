#include "ybhelppage.h"
#include "ybtabwidget.h"
#include "ybinformation.h"

#include <QVBoxLayout>

YbHelpPage::YbHelpPage(QWidget *parent) :
    QWidget(parent)
{
    information = new YbInformation;
    QWidget *leftWidget = new QWidget;
    QHBoxLayout *lefthLayout = new QHBoxLayout;
    lefthLayout->setMargin(0);
    lefthLayout->addSpacerItem(new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    lefthLayout->addWidget(information);
    lefthLayout->addSpacerItem(new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    leftWidget->setLayout(lefthLayout);

    ybTabWidget = new YbTabWidget(leftWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(ybTabWidget);
    setLayout(mainLayout);
}

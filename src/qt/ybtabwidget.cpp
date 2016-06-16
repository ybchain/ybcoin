#include "ybtabwidget.h"
#include "ybtabbutton.h"
#include "ybstackedwidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

YbTabWidget::YbTabWidget(QWidget *parent) :
    QWidget(parent)
{
    leftTabButton = new YbTabButton(tr("信息"), true);
    rightTabButton = new YbTabButton(tr("控制台"), false);
    stackedWidget = new YbStackedWidget;
    connect(leftTabButton, SIGNAL(clicked(bool)), this, SLOT(leftTabButtonChecked(bool)));
    connect(rightTabButton, SIGNAL(clicked(bool)), this, SLOT(rightTabButtonChecked(bool)));
    QHBoxLayout *tabButtonLayout = new QHBoxLayout;
    tabButtonLayout->setSpacing(0);
    tabButtonLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    tabButtonLayout->addWidget(leftTabButton);
    tabButtonLayout->addWidget(rightTabButton);
    tabButtonLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(tabButtonLayout);
    mainLayout->addSpacing(20);
    mainLayout->addWidget(stackedWidget);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(mainLayout);
}

void YbTabWidget::leftTabButtonChecked(bool is)
{
    if(is){
        rightTabButton->setChecked(false);
    }else{
        leftTabButton->setChecked(true);
    }
}

void YbTabWidget::rightTabButtonChecked(bool is)
{
    if(is){
        leftTabButton->setChecked(false);
    }else{
        rightTabButton->setChecked(true);
    }
}


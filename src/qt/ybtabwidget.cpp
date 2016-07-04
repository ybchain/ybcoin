#include "ybtabwidget.h"
#include "ybtabbutton.h"
#include "ybstackedwidget.h"
#include "ybinformation.h"
#include "ybconsole.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

YbTabWidget::YbTabWidget(YbInformation *leftWidget, YbConsole *rightWidget, QWidget *parent) :
    QWidget(parent)
{
    leftTabButton = new YbTabButton(tr("信息"), true);
    rightTabButton = new YbTabButton(tr("控制台"), false);
    stackedWidget = new YbStackedWidget;
    m_leftWidget = leftWidget;
    m_rightWidget = rightWidget;
    if(m_leftWidget != NULL){
        stackedWidget->addWidget(m_leftWidget);
        stackedWidget->setCurrentWidget(m_leftWidget);
    }
    if(m_rightWidget != NULL){
        stackedWidget->addWidget(m_rightWidget);
    }

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
    mainLayout->addLayout(tabButtonLayout);
    mainLayout->addWidget(stackedWidget);
    setLayout(mainLayout);
}

void YbTabWidget::leftTabButtonChecked(bool is)
{
    if(is){
        rightTabButton->setChecked(false);
        if(m_leftWidget != NULL){
            stackedWidget->setCurrentWidget(m_leftWidget);
        }
    }else{
        leftTabButton->setChecked(true);
    }
}

void YbTabWidget::rightTabButtonChecked(bool is)
{
    if(is){
        leftTabButton->setChecked(false);
        if(m_rightWidget != NULL){
            stackedWidget->setCurrentWidget(m_rightWidget);
            m_rightWidget->setLineEditFocus();
        }
    }else{
        rightTabButton->setChecked(true);
    }
}

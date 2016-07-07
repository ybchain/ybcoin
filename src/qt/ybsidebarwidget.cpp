#include "ybsidebarwidget.h"
#include "ybtoolbutton.h"

#include <QVBoxLayout>
#include <QPalette>
#include <QLabel>
#include <QFont>
#include <QDebug>

YbSideBarWidget::YbSideBarWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setMinimumWidth(200);
    this->setMinimumHeight(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(245, 255, 240));
    this->setPalette(pa);
    mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(20);
    mainLayout->setMargin(0);
    titleLabel = new QLabel;
    titleLabel->setMinimumHeight(60);
    titleLabel->setMinimumWidth(250);
    titleLabel->setStyleSheet("background-color:black; font-size:60px; color:white;");
    titleLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(titleLabel);
    setLayout(mainLayout);
}

void YbSideBarWidget::setTitle(const QString &title, const int &height, const int &width)
{
    titleLabel->setAlignment(Qt::AlignHCenter);
    titleLabel->setMinimumHeight(height);
    titleLabel->setMinimumWidth(width);
    titleLabel->setText(title);
}

void YbSideBarWidget::addToolButton(YbToolButton *button)
{
    mainLayout->addWidget(button);
    buttonList.append(button);
    connect(button, SIGNAL(toolClicked()), this, SLOT(oneButtonClicked()));
}

void YbSideBarWidget::addSpacerItem(QSpacerItem *spacerItem)
{
    mainLayout->addSpacerItem(spacerItem);
}

void YbSideBarWidget::oneButtonClicked()
{
    foreach(YbToolButton *button, buttonList){
        if(button != sender()){
            button->setDefaultState();
        }
    }
}

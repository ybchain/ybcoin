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
    this->setFixedWidth(180);
    this->setMinimumHeight(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(245, 255, 240));
    this->setPalette(pa);
    mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(20);
    mainLayout->setMargin(0);
    titleLabel = new QLabel;
    titleLabel->setFixedHeight(60);
    titleLabel->setFixedWidth(180);
    titleLabel->setStyleSheet("background-color:black; font-size:60px; color:white;");
    titleLabel->setAlignment(Qt::AlignHCenter);
    mainLayout->addWidget(titleLabel);
    setLayout(mainLayout);
}

void YbSideBarWidget::setTitle(const QString &title, const int &height, const int &width)
{
    titleLabel->setFixedHeight(height);
    titleLabel->setFixedWidth(width);
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

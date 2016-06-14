#include "ybtoolbarwidget.h"
#include "ybseparater.h"

#include <QLabel>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QFont>
#include <QPushButton>

YbToolBarWidget::YbToolBarWidget(QWidget *parent) :
    QWidget(parent)
{
    boldFont.setBold(true);
    wholeLayout = new QVBoxLayout;
    separaterLayout = new QHBoxLayout;
    mainLayout = new QHBoxLayout;
    leftLayout = new QVBoxLayout;
    rightLayout = new QVBoxLayout;
    ybcLayout = new QHBoxLayout;
    pushButtonLayout = new QHBoxLayout;
    separater = new YbSeparater;

    this->setFixedHeight(100);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background, QColor(255, 255, 255));
    this->setPalette(pa);

    myWalletTextLabel = new QLabel;
    myWalletTextLabel->setFixedHeight(30);
    myWalletTextLabel->setMinimumWidth(400);
    myWalletTextLabel->setFont(boldFont);
    myWalletTextLabel->setStyleSheet("font-size:30px;");
    myWalletTextLabel->setText(tr("My wallet"));

    ybcNumberLabel = new QLabel;
    ybcNumberLabel->setFixedHeight(20);
    ybcNumberLabel->setFont(boldFont);
    ybcNumberLabel->setStyleSheet("font-size:20px;");
    ybcNumber = 0;
    ybcNumberLabel->setText(QString::number(ybcNumber));

    ybcTextLabel = new QLabel;
    ybcTextLabel->setFixedHeight(20);
    ybcTextLabel->setFont(boldFont);
    ybcTextLabel->setStyleSheet("font-size:20px;");
    ybcTextLabel->setText("YBC");

    leftLayout->addWidget(myWalletTextLabel);
    leftLayout->addSpacing(5);
    leftLayout->addLayout(pushButtonLayout);
    QSpacerItem *lvSpacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    leftLayout->addSpacerItem(lvSpacerItem);

    ybcLayout->addWidget(ybcNumberLabel);
    ybcLayout->addWidget(ybcTextLabel);

    rightLayout->addSpacing(20);
    rightLayout->addLayout(ybcLayout);
    QSpacerItem *rvSpacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    rightLayout->addSpacerItem(rvSpacerItem);

    mainLayout->addSpacing(20);
    mainLayout->addLayout(leftLayout);
    QSpacerItem *hSpacerItem = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLayout->addSpacerItem(hSpacerItem);
    mainLayout->addLayout(rightLayout);
    mainLayout->addSpacing(20);

    separaterLayout->addSpacing(20);
    separaterLayout->addWidget(separater);
    separaterLayout->addSpacing(20);

    wholeLayout->addLayout(mainLayout);
    wholeLayout->addLayout(separaterLayout);

    this->setLayout(wholeLayout);
}

void YbToolBarWidget::setYbcNumber(double number)
{
    ybcNumber = number;
    ybcNumberLabel->setText(QString::number(ybcNumber));
}

double YbToolBarWidget::getYbcNumber()
{
    return ybcNumber;
}

void YbToolBarWidget::addPushButton(QPushButton *pushButton)
{
    pushButtonLayout->addWidget(pushButton);
}

void YbToolBarWidget::addSpacerItem(QSpacerItem *spacerItem)
{
    pushButtonLayout->addSpacerItem(spacerItem);
}

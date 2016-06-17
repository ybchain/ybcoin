#include "ybaddressbookpage.h"
#include "ybpushbutton.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

YbAddressBookPage::YbAddressBookPage(QWidget *parent) :
    QWidget(parent)
{
    QPixmap addPix(":icons/add");
    QColor color(99, 184, 255);
    newAddressButton = new YbPushButton(addPix, tr("新地址"), color, color, 3, 95, 25);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addSpacing(20);
    hlayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout->addWidget(newAddressButton);
    hlayout->addSpacing(20);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(hlayout);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    setLayout(mainLayout);
}

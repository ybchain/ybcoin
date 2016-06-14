#include "yboverviewpage.h"
#include "ybseparater.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

YbOverviewPage::YbOverviewPage(QWidget *parent) :
    QWidget(parent)
{
    setStyleSheet("QLabel{font-size:20px;}");
    balanceLayout = new QHBoxLayout;
    balanceText = new QLabel(tr("Balance:"));
    balanceValue = new QLabel(tr("0.00YBC"));
    balanceLayout->addWidget(balanceText);
    balanceLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    balanceLayout->addWidget(balanceValue);

    stakeLayout = new QHBoxLayout;
    stakeText = new QLabel(tr("Stake:"));
    stakeValue = new QLabel(tr("0.00YBC"));
    stakeLayout->addWidget(stakeText);
    stakeLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    stakeLayout->addWidget(stakeValue);

    unconfirmedLayout = new QHBoxLayout;
    unconfirmedText = new QLabel(tr("Unconfirmed:"));
    unconfirmedValue = new QLabel(tr("0.00YBC"));
    unconfirmedLayout->addWidget(unconfirmedText);
    unconfirmedLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    unconfirmedLayout->addWidget(unconfirmedValue);

    transactionsLayout = new QHBoxLayout;
    transactionsText = new QLabel(tr("Number of transactions:"));
    transactionsValue = new QLabel("0");
    transactionsLayout->addWidget(transactionsText);
    transactionsLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    transactionsLayout->addWidget(transactionsValue);

    infoLayout = new QVBoxLayout;
    infoLayout->addLayout(balanceLayout);
    infoLayout->addWidget(new YbSeparater);
    infoLayout->addLayout(stakeLayout);
    infoLayout->addWidget(new YbSeparater);
    infoLayout->addLayout(unconfirmedLayout);
    infoLayout->addWidget(new YbSeparater);
    infoLayout->addLayout(transactionsLayout);

    upperLayout = new QHBoxLayout;
    upperLayout->addSpacing(20);
    upperLayout->addLayout(infoLayout);
    QSpacerItem *uhSpacerItem = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    upperLayout->addSpacerItem(uhSpacerItem);

    recentLayout = new QVBoxLayout;
    rencentText = new QLabel(tr("Recent transactions:"));
    recentLayout->addWidget(rencentText);

    downLayout = new QHBoxLayout;
    downLayout->addSpacing(20);
    downLayout->addLayout(recentLayout);

    mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(10);
    mainLayout->addLayout(upperLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(downLayout);
    QSpacerItem *dvSpacerItem = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addSpacerItem(dvSpacerItem);

    setLayout(mainLayout);
}

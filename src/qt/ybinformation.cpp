#include "ybinformation.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QCheckBox>
#include <QFont>

YbInformation::YbInformation(QWidget *parent) :
    QWidget(parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(245, 255, 240));
    this->setPalette(pa);

    QFont font;
    font.setBold(true);
    versionLabel = new QLabel(tr("版本"));
    versionLabel->setStyleSheet("font-size:15px;");
    versionLabel->setFont(font);
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(versionLabel);
    hlayout1->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    clientNameLabel = new QLabel(tr("客户端名称"));
    clientName = new QLabel(tr("N/A"));
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(clientNameLabel);
    hlayout2->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout2->addWidget(clientName);
    hlayout2->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    clientVersionLabel = new QLabel(tr("客户端版本"));
    clientVersion = new QLabel(tr("N/A"));
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addWidget(clientVersionLabel);
    hlayout3->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout3->addWidget(clientVersion);
    hlayout3->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    buildDateLabel = new QLabel(tr("创建时间"));
    buildDate = new QLabel(tr("N/A"));
    QHBoxLayout *hlayout4 = new QHBoxLayout;
    hlayout4->addWidget(buildDateLabel);
    hlayout4->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout4->addWidget(buildDate);
    hlayout4->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    networkLabel = new QLabel(tr("网络"));
    networkLabel->setFont(font);
    networkLabel->setStyleSheet("font-size:15px;");
    QHBoxLayout *hlayout5 = new QHBoxLayout;
    hlayout5->addWidget(networkLabel);
    hlayout5->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    connectionsNumberLabel = new QLabel(tr("连接数"));
    numberOfConnections = new QLabel(tr("N/A"));
    QHBoxLayout *hlayout6 = new QHBoxLayout;
    hlayout6->addWidget(connectionsNumberLabel);
    hlayout6->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout6->addWidget(numberOfConnections);
    hlayout6->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    onTestnetLabel = new QLabel(tr("测试网"));
    isTestNet = new QCheckBox;
    QHBoxLayout *hlayout7 = new QHBoxLayout;
    hlayout7->addWidget(onTestnetLabel);
    hlayout7->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout7->addWidget(isTestNet);
    hlayout7->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    blockChainLabel = new QLabel(tr("数据链"));
    blockChainLabel->setFont(font);
    blockChainLabel->setStyleSheet("font-size:15px;");
    QHBoxLayout *hlayout8 = new QHBoxLayout;
    hlayout8->addWidget(blockChainLabel);
    hlayout8->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    currentNumOfBlocksLabel = new QLabel(tr("当前数据块数量"));
    numberOfBlocks = new QLabel(tr("N/A"));
    QHBoxLayout *hlayout9 = new QHBoxLayout;
    hlayout9->addWidget(currentNumOfBlocksLabel);
    hlayout9->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout9->addWidget(numberOfBlocks);
    hlayout9->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    estimatedTotalBlocksLabel = new QLabel(tr("预计数据块数量"));
    totalBlocks = new QLabel(tr("N/A"));
    QHBoxLayout *hlayout10 = new QHBoxLayout;
    hlayout10->addWidget(estimatedTotalBlocksLabel);
    hlayout10->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout10->addWidget(totalBlocks);
    hlayout10->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    lastBlockTimeLabel = new QLabel(tr("上一数据块时间"));
    lastBlockTime = new QLabel("N/A");
    QHBoxLayout *hlayout11 = new QHBoxLayout;
    hlayout11->addWidget(lastBlockTimeLabel);
    hlayout11->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout11->addWidget(lastBlockTime);
    hlayout11->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->setSpacing(10);
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout4);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout5);
    vlayout->addLayout(hlayout6);
    vlayout->addLayout(hlayout7);
    vlayout->addSpacing(20);
    vlayout->addLayout(hlayout8);
    vlayout->addLayout(hlayout9);
    vlayout->addLayout(hlayout10);
    vlayout->addLayout(hlayout11);
    vlayout->addSpacing(30);
    vlayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addSpacerItem(new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addLayout(vlayout);
    mainLayout->addSpacerItem(new QSpacerItem(100, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    setLayout(mainLayout);
}

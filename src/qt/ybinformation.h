#ifndef YBINFORMATION_H
#define YBINFORMATION_H

#include <QWidget>

class QLabel;
class QCheckBox;

class YbInformation : public QWidget
{
    Q_OBJECT
public:
    explicit YbInformation(QWidget *parent = 0);

private:
    QLabel *versionLabel;
    QLabel *clientNameLabel;
    QLabel *clientName;
    QLabel *clientVersionLabel;
    QLabel *clientVersion;
    QLabel *buildDateLabel;
    QLabel *buildDate;

    QLabel *networkLabel;
    QLabel *connectionsNumberLabel;
    QLabel *numberOfConnections;
    QLabel *onTestnetLabel;
    QCheckBox *isTestNet;

    QLabel *blockChainLabel;
    QLabel *currentNumOfBlocksLabel;
    QLabel *numberOfBlocks;
    QLabel *estimatedTotalBlocksLabel;
    QLabel *totalBlocks;
    QLabel *lastBlockTimeLabel;
    QLabel *lastBlockTime;
};

#endif // YBINFORMATION_H

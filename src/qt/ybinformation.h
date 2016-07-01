#ifndef YBINFORMATION_H
#define YBINFORMATION_H

#include <QWidget>

class ClientModel;
class QLabel;
class QCheckBox;

class YbInformation : public QWidget
{
    Q_OBJECT
public:
    explicit YbInformation(QWidget *parent = 0);

    void setClientModel(ClientModel *model);

public slots:
    /** Set number of connections shown in the UI */
    void setNumConnections(int count);
    /** Set number of blocks shown in the UI */
    void setNumBlocks(int count);

private:
    void createWidget();

    ClientModel *clientModel;

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

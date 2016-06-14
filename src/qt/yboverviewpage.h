#ifndef YBOVERVIEWPAGE_H
#define YBOVERVIEWPAGE_H

#include <QWidget>

class QLabel;
class QHBoxLayout;
class QVBoxLayout;

class YbOverviewPage : public QWidget
{
    Q_OBJECT
public:
    explicit YbOverviewPage(QWidget *parent = 0);

private:
    QHBoxLayout *upperLayout;
    QVBoxLayout *infoLayout;
    QHBoxLayout *balanceLayout;
    QLabel *balanceText;
    QLabel *balanceValue;
    QHBoxLayout *stakeLayout;
    QLabel *stakeText;
    QLabel *stakeValue;
    QHBoxLayout *unconfirmedLayout;
    QLabel *unconfirmedText;
    QLabel *unconfirmedValue;
    QHBoxLayout *transactionsLayout;
    QLabel *transactionsText;
    QLabel *transactionsValue;
    QHBoxLayout *downLayout;
    QVBoxLayout *recentLayout;
    QLabel *rencentText;
    QVBoxLayout *mainLayout;
};

#endif // YBOVERVIEWPAGE_H

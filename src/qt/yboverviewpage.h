#ifndef YBOVERVIEWPAGE_H
#define YBOVERVIEWPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QModelIndex;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QListView;
QT_END_NAMESPACE

class WalletModel;
class TxViewDelegate;


/** Overview ("home") page widget */
class YbOverviewPage : public QWidget
{
    Q_OBJECT
public:
    explicit YbOverviewPage(QWidget *parent = 0);
    ~YbOverviewPage();

    void setModel(WalletModel *model);

public slots:
    void setBalance(qint64 balance, qint64 stake, qint64 unconfirmedBalance);
    void setNumTransactions(int count);

signals:
    void transactionClicked(const QModelIndex &index);
    void setToolBarBalance(const QString &str);

private:
    void createWidget();

    QLabel *balanceText;
    QLabel *balanceValue;
    QLabel *stakeText;
    QLabel *stakeValue;
    QLabel *unconfirmedText;
    QLabel *unconfirmedValue;
    QLabel *transactionsText;
    QLabel *transactionsValue;
    QLabel *recentText;

    WalletModel *model;
    QListView *listTransactions;
    qint64 currentBalance;
    qint64 currentStake;
    qint64 currentUnconfirmedBalance;

    TxViewDelegate *txdelegate;

private slots:
    void displayUnitChanged();
};

#endif // YBOVERVIEWPAGE_H

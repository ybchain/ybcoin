#include "yboverviewpage.h"
#include "ybseparater.h"
#include "walletmodel.h"
#include "bitcoinunits.h"
#include "optionsmodel.h"
#include "transactiontablemodel.h"
#include "transactionfilterproxy.h"
#include "guiutil.h"
#include "guiconstants.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QAbstractItemDelegate>
#include <QPainter>
#include <QListView>

#define DECORATION_SIZE 64
#define NUM_ITEMS 3

class TxViewDelegate : public QAbstractItemDelegate
{
    Q_OBJECT
public:
    TxViewDelegate(): QAbstractItemDelegate(), unit(BitcoinUnits::BTC)
    {

    }

    inline void paint(QPainter *painter, const QStyleOptionViewItem &option,
                      const QModelIndex &index ) const
    {
        painter->save();

        QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
        QRect mainRect = option.rect;
        QRect decorationRect(mainRect.topLeft(), QSize(DECORATION_SIZE, DECORATION_SIZE));
        int xspace = DECORATION_SIZE + 8;
        int ypad = 6;
        int halfheight = (mainRect.height() - 2*ypad)/2;
        QRect amountRect(mainRect.left() + xspace, mainRect.top()+ypad, mainRect.width() - xspace, halfheight);
        QRect addressRect(mainRect.left() + xspace, mainRect.top()+ypad+halfheight, mainRect.width() - xspace, halfheight);
        icon.paint(painter, decorationRect);

        QDateTime date = index.data(TransactionTableModel::DateRole).toDateTime();
        QString address = index.data(Qt::DisplayRole).toString();
        qint64 amount = index.data(TransactionTableModel::AmountRole).toLongLong();
        bool confirmed = index.data(TransactionTableModel::ConfirmedRole).toBool();
        QVariant value = index.data(Qt::ForegroundRole);
        QColor foreground = option.palette.color(QPalette::Text);
#if QT_VERSION < 0x050000
        if(qVariantCanConvert<QColor>(value))
#else
        if(value.canConvert(QMetaType::QColor))
#endif
        {
            foreground = qvariant_cast<QColor>(value);
        }

        painter->setPen(foreground);
        painter->drawText(addressRect, Qt::AlignLeft|Qt::AlignVCenter, address);

        if(amount < 0)
        {
            foreground = COLOR_NEGATIVE;
        }
        else if(!confirmed)
        {
            foreground = COLOR_UNCONFIRMED;
        }
        else
        {
            foreground = option.palette.color(QPalette::Text);
        }
        painter->setPen(foreground);
        QString amountText = BitcoinUnits::formatWithUnit(unit, amount, true);
        if(!confirmed)
        {
            amountText = QString("[") + amountText + QString("]");
        }
        painter->drawText(amountRect, Qt::AlignRight|Qt::AlignVCenter, amountText);

        painter->setPen(option.palette.color(QPalette::Text));
        painter->drawText(amountRect, Qt::AlignLeft|Qt::AlignVCenter, GUIUtil::dateTimeStr(date));

        painter->restore();
    }

    inline QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        return QSize(DECORATION_SIZE, DECORATION_SIZE);
    }

    int unit;

};
#include "yboverviewpage.moc"

YbOverviewPage::YbOverviewPage(QWidget *parent) :
    currentBalance(-1),
    currentStake(0),
    currentUnconfirmedBalance(-1),
    txdelegate(new TxViewDelegate()),
    listTransactions(new QListView),
    QWidget(parent)
{
    createWidget();

    // Balance: <balance>
    balanceValue->setFont(QFont("Monospace", -1, QFont::Bold));
    balanceValue->setToolTip(tr("Your current balance"));
    balanceValue->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);

    // peercoin: stake: <stake>
    stakeValue->setFont(QFont("Monospace", -1, QFont::Bold));
    stakeValue->setToolTip(tr("Your current stake"));
    stakeValue->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);

    // Unconfirmed balance: <balance>
    unconfirmedValue->setFont(QFont("Monospace", -1, QFont::Bold));
    unconfirmedValue->setToolTip(tr("Total of transactions that have yet to be confirmed, and do not yet count toward the current balance"));
    unconfirmedValue->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);

    transactionsValue->setToolTip(tr("Total number of transactions in wallet"));

    // Recent transactions
    //ui->listTransactions->setStyleSheet("QListView { background:transparent }");
    listTransactions->setItemDelegate(txdelegate);
    listTransactions->setIconSize(QSize(DECORATION_SIZE, DECORATION_SIZE));
    listTransactions->setSelectionMode(QAbstractItemView::NoSelection);
    listTransactions->setMinimumHeight(NUM_ITEMS * (DECORATION_SIZE + 2));
    listTransactions->setAttribute(Qt::WA_MacShowFocusRect, false);

    connect(listTransactions, SIGNAL(clicked(QModelIndex)), this, SIGNAL(transactionClicked(QModelIndex)));
}

YbOverviewPage::~YbOverviewPage()
{
    if(balanceText != NULL){
        delete balanceText;
    }
    if(balanceValue != NULL){
        delete balanceValue;
    }
    if(stakeText != NULL){
        delete stakeText;
    }
    if(stakeValue != NULL){
        delete stakeValue;
    }
    if(unconfirmedText != NULL){
        delete unconfirmedText;
    }
    if(unconfirmedValue != NULL){
        delete unconfirmedValue;
    }
    if(transactionsText != NULL){
        delete transactionsText;
    }
    if(transactionsValue != NULL){
        delete transactionsValue;
    }
    if(recentText != NULL){
        delete recentText;
    }
    if(listTransactions != NULL){
        delete listTransactions;
    }
}

void YbOverviewPage::setModel(WalletModel *model)
{
    this->model = model;
    if(model)
    {
        // Set up transaction list
        TransactionFilterProxy *filter = new TransactionFilterProxy();
        filter->setSourceModel(model->getTransactionTableModel());
        filter->setLimit(NUM_ITEMS);
        filter->setDynamicSortFilter(true);
        filter->setSortRole(Qt::EditRole);
        filter->sort(TransactionTableModel::Date, Qt::DescendingOrder);

        listTransactions->setModel(filter);
        listTransactions->setModelColumn(TransactionTableModel::ToAddress);

        // Keep up to date with wallet
        setBalance(model->getBalance(), model->getStake(), model->getUnconfirmedBalance());
        connect(model, SIGNAL(balanceChanged(qint64, qint64, qint64)), this, SLOT(setBalance(qint64, qint64, qint64)));

        setNumTransactions(model->getNumTransactions());
        connect(model, SIGNAL(numTransactionsChanged(int)), this, SLOT(setNumTransactions(int)));

        connect(model->getOptionsModel(), SIGNAL(displayUnitChanged(int)), this, SLOT(displayUnitChanged()));
    }
}

void YbOverviewPage::setBalance(qint64 balance, qint64 stake, qint64 unconfirmedBalance)
{
    int unit = model->getOptionsModel()->getDisplayUnit();
    currentBalance = balance;
    currentStake = stake;
    currentUnconfirmedBalance = unconfirmedBalance;
    balanceValue->setText(BitcoinUnits::formatWithUnit(unit, balance));
    stakeValue->setText(BitcoinUnits::formatWithUnit(unit, stake));
    unconfirmedValue->setText(BitcoinUnits::formatWithUnit(unit, unconfirmedBalance));
}

void YbOverviewPage::setNumTransactions(int count)
{
    transactionsValue->setText(QLocale::system().toString(count));
}

void YbOverviewPage::createWidget()
{
    setStyleSheet("QListView{border: 2px groove rgb(211, 211, 211)} QLabel{font-size:20px;}");
    QHBoxLayout *balanceLayout = new QHBoxLayout;
    balanceText = new QLabel(tr("Balance:"));
    balanceValue = new QLabel(tr("0.00YBC"));
    balanceLayout->addWidget(balanceText);
    balanceLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    balanceLayout->addWidget(balanceValue);

    QHBoxLayout *stakeLayout = new QHBoxLayout;
    stakeText = new QLabel(tr("Stake:"));
    stakeValue = new QLabel(tr("0.00YBC"));
    stakeLayout->addWidget(stakeText);
    stakeLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    stakeLayout->addWidget(stakeValue);

    QHBoxLayout *unconfirmedLayout = new QHBoxLayout;
    unconfirmedText = new QLabel(tr("Unconfirmed:"));
    unconfirmedValue = new QLabel(tr("0.00YBC"));
    unconfirmedLayout->addWidget(unconfirmedText);
    unconfirmedLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    unconfirmedLayout->addWidget(unconfirmedValue);

    QHBoxLayout *transactionsLayout = new QHBoxLayout;
    transactionsText = new QLabel(tr("Number of transactions:"));
    transactionsValue = new QLabel("0");
    transactionsLayout->addWidget(transactionsText);
    transactionsLayout->addSpacerItem(new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    transactionsLayout->addWidget(transactionsValue);

    QVBoxLayout *infoLayout = new QVBoxLayout;
    infoLayout->addLayout(balanceLayout);
    infoLayout->addWidget(new YbSeparater);
    infoLayout->addLayout(stakeLayout);
    infoLayout->addWidget(new YbSeparater);
    infoLayout->addLayout(unconfirmedLayout);
    infoLayout->addWidget(new YbSeparater);
    infoLayout->addLayout(transactionsLayout);

    QHBoxLayout *upperLayout = new QHBoxLayout;
    upperLayout->addSpacing(20);
    upperLayout->addLayout(infoLayout);
    QSpacerItem *uhSpacerItem = new QSpacerItem(200, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    upperLayout->addSpacerItem(uhSpacerItem);

    QVBoxLayout *recentLayout = new QVBoxLayout;
    recentText = new QLabel(tr("Recent transactions:"));
    recentLayout->addWidget(recentText);
    recentLayout->addWidget(listTransactions);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addSpacing(20);
    downLayout->addLayout(recentLayout);
    downLayout->addSpacing(20);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(10);
    mainLayout->addLayout(upperLayout);
    mainLayout->addSpacing(30);
    mainLayout->addLayout(downLayout);

    setLayout(mainLayout);
}

void YbOverviewPage::displayUnitChanged()
{
    if(!model || !model->getOptionsModel())
        return;
    if(currentBalance != -1)
        setBalance(currentBalance, currentStake, currentUnconfirmedBalance);

    txdelegate->unit = model->getOptionsModel()->getDisplayUnit();
    listTransactions->update();
}

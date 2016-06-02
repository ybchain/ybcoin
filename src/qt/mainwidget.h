#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>

class ClientModel;
class WalletModel;
class TitleWidget;
class ToolWidget;
class StackedWidget;
class QVBoxLayout;
class QStatusBar;
class TransactionTableModel;
class TransactionView;
class OverviewPage;
class AddressBookPage;
class SendCoinsDialog;
class MessagePage;
class Notificator;
class RPCConsole;

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTableView;
class QAbstractItemModel;
class QModelIndex;
class QProgressBar;
class QStackedWidget;
class QUrl;
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    /** Set the client model.
        The client model represents the part of the core that communicates with the P2P network, and is wallet-agnostic.
    */
    void setClientModel(ClientModel *clientModel);
    /** Set the wallet model.
        The wallet model represents a bitcoin wallet, and offers access to the list of transactions, address book and sending
        functionality.
    */
    void setWalletModel(WalletModel *walletModel);

protected:
    void paintEvent(QPaintEvent *e);
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    ClientModel *clientModel;
    WalletModel *walletModel;

    OverviewPage *overviewPage;
    QWidget *transactionsPage;
    AddressBookPage *addressBookPage;
    AddressBookPage *receiveCoinsPage;
    SendCoinsDialog *sendCoinsPage;
    MessagePage *messagePage;

    QLabel *labelEncryptionIcon;
    QLabel *labelConnectionsIcon;
    QLabel *labelBlocksIcon;
    QLabel *progressBarLabel;
    QProgressBar *progressBar;

    QAction *receiveCoinsAction;
    QAction *sendCoinsAction;
    QAction *messageAction;

    QAction *quitAction;
    QAction *aboutAction;
    QAction *optionsAction;
    QAction *toggleHideAction;
    QAction *exportAction;
    QAction *exportPeercoinKeysAction;
    QAction *encryptWalletAction;
    QAction *unlockForMintingAction;
    QAction *backupWalletAction;
    QAction *changePassphraseAction;
    QAction *aboutQtAction;
    QAction *openRPCConsoleAction;
    QAction *distributeDividendsAction;

    QMenu *menu;
    QSystemTrayIcon *trayIcon;
    Notificator *notificator;
    TransactionView *transactionView;
    RPCConsole *rpcConsole;

    QMovie *syncIconMovie;

    QString bkPicName;
    QPoint normalTopLeft;

    TitleWidget *titleWidget;
    ToolWidget *toolWidget;
    StackedWidget *centralWidget;
    QStatusBar *statusBar;
    QVBoxLayout *mainLayout;

    /** Create the main UI actions. */
    void createActions();
    /** Create the menu bar and submenus. */
    void createMenu();
    /** Create system tray (notification) icon */
    void createTrayIcon();

    void createTitleWidget();
    void createToolWidget();
    void createCentralWidget();
    void createStatusBar();
    void createMainLayout();

    void createToolWidgetConnections();

public slots:
    /** Set number of connections shown in the UI */
    void setNumConnections(int count);
    /** Set number of blocks shown in the UI */
    void setNumBlocks(int count);
    /** Set the encryption status as shown in the UI.
       @param[in] status            current encryption status
       @see WalletModel::EncryptionStatus
    */
    void setEncryptionStatus(int status);

    /** Notify the user of an error in the network or transaction handling code. */
    void error(const QString &title, const QString &message, bool modal);
    /** Asks the user whether to pay the transaction fee or to cancel the transaction.
       It is currently not possible to pass a return value to another thread through
       BlockingQueuedConnection, so an indirected pointer is used.
       http://bugreports.qt.nokia.com/browse/QTBUG-10440

      @param[in] nFeeRequired       the required fee
      @param[out] payFee            true to pay the fee, false to not pay the fee
    */
    void askFee(qint64 nFeeRequired, bool *payFee);
    void handleURI(QString strURI);

    void gotoMessagePage();
    void gotoMessagePage(QString addr);

private slots:
    void showMax();
    void showMin();
    void showSkinWidget();

    /** Switch to overview (home) page */
    void gotoOverviewPage();
    /** Switch to history (transactions) page */
    void gotoHistoryPage();
    /** Switch to address book page */
    void gotoAddressBookPage();
    /** Switch to receive shares page */
    void gotoReceiveCoinsPage();
    /** Switch to send shares page */
    void gotoSendCoinsPage();

    /** Show configuration dialog */
    void optionsClicked();
    /** Show about dialog */
    void aboutClicked();
    /** Show distribute dividends dialog */
    void distributeDividendsClicked();
#ifndef Q_WS_MAC
    /** Handle tray icon clicked */
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
#endif
    /** Show incoming transaction notification for new transactions.

        The new items are those between start and end inclusive, under the given parent item.
    */
    void incomingTransaction(const QModelIndex & parent, int start, int end);
    /** Encrypt the wallet */
    void encryptWallet(bool status);
    /** Decrypt wallet for minting only */
    void unlockForMinting(bool status);
    /** Backup the wallet */
    void backupWallet();
    /** Change encrypted wallet passphrase */
    void changePassphrase();
    /** Ask for pass phrase to unlock wallet temporarily */
    void unlockWallet();

    /** Export the keys to peercoin */
    void exportPeercoinKeys();

    /** Show window if hidden, unminimize when minimized */
    void showNormalIfMinimized();
    /** Hide window if visible, show if hidden */
    void toggleHidden();
};

#endif // MAINWIDGET_H

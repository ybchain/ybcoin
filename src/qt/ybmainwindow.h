#ifndef YBMAINWINDOW_H
#define YBMAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class ClientModel;
class WalletModel;
class YbToolButton;
class YbSideBarWidget;
class YbToolBarWidget;
class YbPushButton;
class YbStackedWidget;
class YbOverviewPage;
class TransactionView;
class YbAddressBookPage;
class YbSettingPage;
class YbHelpPage;
class YbSendCoinsDialog;
class YbReceiveCoinsDialog;
class YbSendSignDialog;
class Notificator;
class YbConsole;

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTableView;
class QAbstractItemModel;
class QModelIndex;
class QProgressBar;
class QStackedWidget;
class QUrl;
class QStatusBar;
QT_END_NAMESPACE

class YbMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit YbMainWindow(QWidget *parent = 0);
    ~YbMainWindow();

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
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    void createWidgets();
    void createButton();

    ClientModel *clientModel;
    WalletModel *walletModel;

    YbSideBarWidget *sideBar;
    YbToolBarWidget *toolBar;
    YbStackedWidget *stackedWidget;

    YbOverviewPage *overviewPage;
    QWidget *historyPage;
    TransactionView *transactionView;
    YbAddressBookPage *addressPage;
    YbAddressBookPage *receiveCoinsPage;
    YbSettingPage *settingPage;
    YbHelpPage *helpPage;
    YbSendSignDialog *messagePage;

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

    QMenuBar *appMenuBar;
    QSystemTrayIcon *trayIcon;
    Notificator *notificator;
    YbConsole *rpcConsole;

    QMovie *syncIconMovie;

    QString bkPicName;
    QPoint normalTopLeft;

    YbToolButton *overviewButton;
    YbToolButton *historyButton;
    YbToolButton *addressBookButton;
    YbToolButton *settingButton;
    YbToolButton *helpButton;
    YbPushButton *sendPushButton;
    YbPushButton *receivePushButton;
    YbPushButton *exportPushButton;
    YbSendCoinsDialog *sendCoinsDialog;
    YbReceiveCoinsDialog *receiveCoinsDialog;

    /** Create the main UI actions. */
    void createActions();
    /** Create the menu bar and submenus. */
    void createMenuBar();
    /** Create system tray (notification) icon */
    void createTrayIcon();

    void createStatusBar();

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

    void showSendSignDialog();
    void showSendSignDialog(QString addr);

private slots:
    /** Switch to overview (home) page */
    void gotoOverviewPage();
    /** Switch to history (transactions) page */
    void gotoHistoryPage();
    /** Switch to receive shares page */
    void gotoReceiveCoinsPage();
    /** Switch to receive shares page */
    void gotoSettingPage();
    /** Switch to send shares page */
    void gotoHelpPage();

    void showSendCoinsDialog();
    void showReceiveCoinsDialog();

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

#endif // YBMAINWINDOW_H

#include "ybmainwindow.h"
#include "ybtoolbutton.h"
#include "ybpushbutton.h"
#include "ybsidebarwidget.h"
#include "ybtoolbarwidget.h"
#include "ybstackedwidget.h"
#include "yboverviewpage.h"
#include "ybaddressbookpage.h"
#include "ybsettingpage.h"
#include "ybhelppage.h"
#include "transactionview.h"
#include "ybsendcoinsdialog.h"
#include "ybreceivecoinsdialog.h"
#include "ybsendsigndialog.h"
#include "transactiontablemodel.h"
#include "optionsdialog.h"
#include "aboutdialog.h"
#include "clientmodel.h"
#include "walletmodel.h"
#include "editaddressdialog.h"
#include "optionsmodel.h"
#include "transactiondescdialog.h"
#include "addresstablemodel.h"
#include "bitcoinunits.h"
#include "guiconstants.h"
#include "askpassphrasedialog.h"
#include "notificator.h"
#include "guiutil.h"
#include "ybconsole.h"
#include "wallet.h"
#include "distributedivdialog.h"
#include "ybencryptiondialog.h"

#ifdef Q_OS_MAC
#include "macdockiconhandler.h"
#endif

#include <QApplication>
#include <QVBoxLayout>
#include <QDesktopWidget>
#include <QStatusBar>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QAction>
#include <QToolBar>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLocale>
#include <QMessageBox>
#include <QProgressBar>
#include <QStackedWidget>
#include <QDateTime>
#include <QMovie>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTimer>
#include <QMimeData>
#include <QBitmap>
#include <QPainter>
#include <qmath.h>
#include <QGraphicsDropShadowEffect>

#include <QDragEnterEvent>
#if QT_VERSION < 0x050000
#include <QUrl>
#endif
#include <iostream>

YbMainWindow::YbMainWindow(QWidget *parent) :
    messagePage(0), sendCoinsDialog(0), receiveCoinsDialog(0),
    QMainWindow(parent)
{
    bkPicName = "";
    resize(850,600);

    createActions();
    createMenuBar();
    createWidgets();
    createStatusBar();

    // Accept D&D of URIs
    setAcceptDrops(true);
    // Create the tray icon (or setup the dock icon)
    createTrayIcon();

    syncIconMovie = new QMovie(":/movies/update_spinner", "mng", this);

    rpcConsole = new YbConsole(this);
    connect(openRPCConsoleAction, SIGNAL(triggered()), rpcConsole, SLOT(show()));

    gotoOverviewPage();
}

YbMainWindow::~YbMainWindow()
{
}

void YbMainWindow::setClientModel(ClientModel *clientModel)
{
    this->clientModel = clientModel;
    if(clientModel)
    {
        if(clientModel->isTestNet())
        {
            QString title_testnet = windowTitle() + QString(" ") + tr("[testnet]");
            setWindowTitle(title_testnet);
#ifndef Q_OS_MAC
            setWindowIcon(QIcon(":icons/peershares_testnet"));
#else
            MacDockIconHandler::instance()->setIcon(QIcon(":icons/peershares_testnet"));
#endif
            if(trayIcon)
            {
                trayIcon->setToolTip(title_testnet);
                trayIcon->setIcon(QIcon(":/icons/toolbar_testnet"));
            }
        }

        // Keep up to date with client
        setNumConnections(clientModel->getNumConnections());
        connect(clientModel, SIGNAL(numConnectionsChanged(int)), this, SLOT(setNumConnections(int)));

        setNumBlocks(clientModel->getNumBlocks());
        connect(clientModel, SIGNAL(numBlocksChanged(int)), this, SLOT(setNumBlocks(int)));

        // Report errors from network/worker thread
        connect(clientModel, SIGNAL(error(QString,QString, bool)), this, SLOT(error(QString,QString,bool)));

        helpPage->setClientModel(clientModel);
    }
}

void YbMainWindow::setWalletModel(WalletModel *walletModel)
{
    this->walletModel = walletModel;
    if(walletModel)
    {
        // Report errors from wallet thread
        connect(walletModel, SIGNAL(error(QString,QString,bool)), this, SLOT(error(QString,QString,bool)));

        // Put transaction list in tabs
        transactionView->setModel(walletModel);

        overviewPage->setModel(walletModel);
        receiveCoinsPage->setModel(walletModel->getAddressTableModel());
        //receiveCoinsDialog->setModel(walletModel->getAddressTableModel());
        sendCoinsDialog->setModel(walletModel);
        messagePage->setModel(walletModel);

        setEncryptionStatus(walletModel->getEncryptionStatus());
        connect(walletModel, SIGNAL(encryptionStatusChanged(int)), this, SLOT(setEncryptionStatus(int)));

        // Balloon popup for new transaction
        connect(walletModel->getTransactionTableModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
                this, SLOT(incomingTransaction(QModelIndex,int,int)));

        // Ask for passphrase if needed
        connect(walletModel, SIGNAL(requireUnlock()), this, SLOT(unlockWallet()));
    }
}

void YbMainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
#ifndef Q_OS_MAC // Ignored on Mac
    if(e->type() == QEvent::WindowStateChange)
    {
        if(clientModel && clientModel->getOptionsModel()->getMinimizeToTray())
        {
            QWindowStateChangeEvent *wsevt = static_cast<QWindowStateChangeEvent*>(e);
            if(!(wsevt->oldState() & Qt::WindowMinimized) && isMinimized())
            {
                QTimer::singleShot(0, this, SLOT(hide()));
                e->ignore();
            }
        }
    }
#endif
}

void YbMainWindow::closeEvent(QCloseEvent *event)
{
    if(clientModel)
    {
#ifndef Q_OS_MAC // Ignored on Mac
        if(!clientModel->getOptionsModel()->getMinimizeToTray() &&
           !clientModel->getOptionsModel()->getMinimizeOnClose())
        {
            qApp->quit();
        }
#endif
    }
    QWidget::closeEvent(event);
}

void YbMainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // Accept only URIs
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void YbMainWindow::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        showSendCoinsDialog();
        QList<QUrl> uris = event->mimeData()->urls();
        foreach(const QUrl &uri, uris)
        {
            sendCoinsDialog->handleURI(uri.toString());
        }
    event->acceptProposedAction();
    }
}

void YbMainWindow::createWidgets()
{
    //setMinimumHeight(675);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *sideBarLayout = new QVBoxLayout;
    sideBar = new YbSideBarWidget(this);
    createButton();
    sideBar->setTitle(tr("YBC"));
    sideBar->addToolButton(overviewButton);
    sideBar->addToolButton(historyButton);
    sideBar->addToolButton(addressBookButton);
    sideBar->addToolButton(settingButton);
    sideBar->addToolButton(helpButton);
    sideBar->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    sideBarLayout->setMargin(0);
    sideBarLayout->addWidget(sideBar);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    toolBar = new YbToolBarWidget(this);
    toolBar->addPushButton(sendPushButton);
    toolBar->addPushButton(receivePushButton);
    toolBar->addPushButton(exportPushButton);
    toolBar->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    rightLayout->addWidget(toolBar);

    stackedWidget = new YbStackedWidget(this);
    overviewPage = new YbOverviewPage(this);
    historyPage = new QWidget(this);
#ifndef Q_OS_MAC
    historyPage->setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QComboBox{border: 2px groove rgb(211, 211, 211);}");
#endif
    QHBoxLayout *hbox = new QHBoxLayout();
    transactionView = new TransactionView(this);
    hbox->addSpacing(20);
    hbox->addWidget(transactionView);
    hbox->addSpacing(20);
    historyPage->setLayout(hbox);
    receiveCoinsPage = new YbAddressBookPage(YbAddressBookPage::ForEditing, YbAddressBookPage::ReceivingTab);
    settingPage = new YbSettingPage(this);
    helpPage = new YbHelpPage(this);
    stackedWidget->addWidget(overviewPage);
    stackedWidget->addWidget(historyPage);
    stackedWidget->addWidget(receiveCoinsPage);
    stackedWidget->addWidget(settingPage);
    stackedWidget->addWidget(helpPage);
    rightLayout->addWidget(stackedWidget);

    messagePage = new YbSendSignDialog(this);
    sendCoinsDialog = new YbSendCoinsDialog(this);
    receiveCoinsDialog = new YbReceiveCoinsDialog;

    connect(overviewPage, SIGNAL(setToolBarBalance(QString)), toolBar, SLOT(setYbcNumber(QString)));
    connect(receiveCoinsDialog, SIGNAL(showSendSign(QString)), this, SLOT(showSendSignDialog(QString)));
    connect(receiveCoinsPage, SIGNAL(showSignDialog(QString)), this, SLOT(showSendSignDialog(QString)));

    // Clicking on a transaction on the overview page simply sends you to transaction history page
    connect(overviewPage, SIGNAL(transactionClicked(QModelIndex)), this, SLOT(gotoHistoryPage()));
    // Doubleclicking on a transaction on the transaction history page shows details
    connect(transactionView, SIGNAL(doubleClicked(QModelIndex)), transactionView, SLOT(showDetails()));

    connect(settingPage, SIGNAL(button1Clicked(bool)), this, SLOT(encryptWallet(bool)));
    connect(settingPage, SIGNAL(button2Clicked(bool)), this, SLOT(unlockForMinting(bool)));
    connect(settingPage, SIGNAL(button3Clicked()), this, SLOT(changePassphrase()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->setSpacing(0);
    mainLayout->addLayout(sideBarLayout);
    mainLayout->addLayout(rightLayout);
    mainLayout->setMargin(0);

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}

void YbMainWindow::createButton()
{
    QPixmap overviewPix(":icons/grayhome");
    QPixmap checkedOverviewPix(":icons/bluehome");
    overviewButton = new YbToolButton(overviewPix, checkedOverviewPix, tr("Overview"), this);
    overviewButton->setToolTip(tr("Show general overview of holdings"));
    overviewButton->setChecked(true);

    QPixmap historyPix(":icons/grayhistory");
    QPixmap checkedHistoryPix(":icons/bluehistory");
    historyButton = new YbToolButton(historyPix, checkedHistoryPix, tr("Transactions"), this);
    historyButton->setToolTip(tr("Browse transaction history"));

    QPixmap addressPix(":icons/grayaddress");
    QPixmap checkedAddressPix(":icons/blueaddress");
    addressBookButton = new YbToolButton(addressPix, checkedAddressPix, tr("Address Book"), this);
    addressBookButton->setToolTip(tr("Edit the list of stored addresses and labels"));

    QPixmap settingPix(":icons/graysetting");
    QPixmap checkedSettingPix(":icons/bluesetting");
    settingButton = new YbToolButton(settingPix, checkedSettingPix, tr("Setting"), this);
    settingButton->setToolTip(tr("Setting"));

    QPixmap helpPix(":icons/grayhelp");
    QPixmap checkedHelpPix(":icons/bluehelp");
    helpButton = new YbToolButton(helpPix, checkedHelpPix, tr("Help"), this);
    helpButton->setToolTip(tr("Help"));

    QPixmap sendPix(":icons/ybsend");
    sendPushButton = new YbPushButton(sendPix, tr("Send"), this);

    QPixmap receivePix(":icons/receive");
    receivePushButton = new YbPushButton(receivePix, tr("Receive"), this);
    receivePushButton->hide();

    QPixmap exportPix(":icons/grayexport");
    exportPushButton = new YbPushButton(exportPix, tr("Export"), this);

    connect(overviewButton, SIGNAL(toolClicked()), this, SLOT(showNormalIfMinimized()));
    connect(overviewButton, SIGNAL(toolClicked()), this, SLOT(gotoOverviewPage()));
    connect(historyButton, SIGNAL(toolClicked()), this, SLOT(showNormalIfMinimized()));
    connect(historyButton, SIGNAL(toolClicked()), this, SLOT(gotoHistoryPage()));
    connect(addressBookButton, SIGNAL(toolClicked()), this, SLOT(showNormalIfMinimized()));
    connect(addressBookButton, SIGNAL(toolClicked()), this, SLOT(gotoReceiveCoinsPage()));
    connect(settingButton, SIGNAL(toolClicked()), this, SLOT(showNormalIfMinimized()));
    connect(settingButton, SIGNAL(toolClicked()), this, SLOT(gotoSettingPage()));
    connect(helpButton, SIGNAL(toolClicked()), this, SLOT(showNormalIfMinimized()));
    connect(helpButton, SIGNAL(toolClicked()), this, SLOT(gotoHelpPage()));
    connect(sendPushButton, SIGNAL(clicked()), this, SLOT(showSendCoinsDialog()));
    connect(receivePushButton, SIGNAL(clicked()), this, SLOT(showReceiveCoinsDialog()));
}

void YbMainWindow::createActions()
{
    receiveCoinsAction = new QAction(QIcon(":/icons/receiving_addresses"), tr("&Receive YBC"), this);
    receiveCoinsAction->setToolTip(tr("Show the list of addresses for receiving payments"));
    receiveCoinsAction->setCheckable(true);
    receiveCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_3));

    sendCoinsAction = new QAction(QIcon(":/icons/send"), tr("&Send YBC"), this);
    sendCoinsAction->setToolTip(tr("Send YBC to a ybcoin address"));
    sendCoinsAction->setCheckable(true);
    sendCoinsAction->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));

    messageAction = new QAction(QIcon(":/icons/edit"), tr("Sign &message"), this);
    messageAction->setToolTip(tr("Prove you control an address"));
#ifdef FIRST_CLASS_MESSAGING
    messageAction->setCheckable(true);
#endif

    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(receiveCoinsAction, SIGNAL(triggered()), this, SLOT(showReceiveCoinsDialog()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(sendCoinsAction, SIGNAL(triggered()), this, SLOT(showSendCoinsDialog()));
    connect(messageAction, SIGNAL(triggered()), this, SLOT(showNormalIfMinimized()));
    connect(messageAction, SIGNAL(triggered()), this, SLOT(showSendSignDialog()));

    quitAction = new QAction(QIcon(":/icons/quit"), tr("E&xit"), this);
    quitAction->setToolTip(tr("Quit application"));
    quitAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAction->setMenuRole(QAction::QuitRole);
    aboutAction = new QAction(QIcon(":/icons/ybcoin"), tr("&About %1").arg(qApp->applicationName()), this);
    aboutAction->setToolTip(tr("Show information about ybcoin"));
    aboutAction->setMenuRole(QAction::AboutRole);
    aboutQtAction = new QAction(tr("About &Qt"), this);
    aboutQtAction->setToolTip(tr("Show information about Qt"));
    aboutQtAction->setMenuRole(QAction::AboutQtRole);
    optionsAction = new QAction(QIcon(":/icons/options"), tr("&Options..."), this);
    optionsAction->setToolTip(tr("Modify configuration options for ybcoin"));
    optionsAction->setMenuRole(QAction::PreferencesRole);
    toggleHideAction = new QAction(QIcon(":/icons/ybcoin"), tr("Show/Hide &ybcoin"), this);
    toggleHideAction->setToolTip(tr("Show or hide the ybcoin window"));
    exportAction = new QAction(QIcon(":/icons/export"), tr("&Export..."), this);
    exportAction->setToolTip(tr("Export the data in the current tab to a file"));
    encryptWalletAction = new QAction(QIcon(":/icons/lock_closed"), tr("&Encrypt Portfolio"), this);
    encryptWalletAction->setToolTip(tr("Encrypt or decrypt portfolio"));
    encryptWalletAction->setCheckable(true);
    unlockForMintingAction = new QAction(QIcon(":/icons/lock_closed"), tr("&Unlock Wallet for Minting Only"), this);
    unlockForMintingAction->setToolTip(tr("Unlock wallet only for minting. Sending coins will still require the passphrase."));
    unlockForMintingAction->setCheckable(true);
    backupWalletAction = new QAction(QIcon(":/icons/filesave"), tr("&Backup Wallet"), this);
    backupWalletAction->setToolTip(tr("Backup portfolio to another location"));
    changePassphraseAction = new QAction(QIcon(":/icons/key"), tr("&Change Passphrase"), this);
    changePassphraseAction->setToolTip(tr("Change the passphrase used for portfolio encryption"));
    openRPCConsoleAction = new QAction(tr("&Debug window"), this);
    openRPCConsoleAction->setToolTip(tr("Open debugging and diagnostic console"));
    exportPeercoinKeysAction = new QAction(QIcon(":/icons/export"), tr("&Export Bitcoin keys"), this);
    exportPeercoinKeysAction->setToolTip(tr("Export the bitcoin keys associated with the ybcoin addresses to bitcoin via RPC"));
    distributeDividendsAction = new QAction(tr("&Distribute dividends"), this);
    distributeDividendsAction->setToolTip(tr("Distribute dividends to share holders"));

    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(optionsAction, SIGNAL(triggered()), this, SLOT(optionsClicked()));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutClicked()));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(toggleHideAction, SIGNAL(triggered()), this, SLOT(toggleHidden()));
    connect(encryptWalletAction, SIGNAL(triggered(bool)), this, SLOT(encryptWallet(bool)));
    connect(unlockForMintingAction, SIGNAL(triggered(bool)), this, SLOT(unlockForMinting(bool)));
    connect(backupWalletAction, SIGNAL(triggered()), this, SLOT(backupWallet()));
    connect(changePassphraseAction, SIGNAL(triggered()), this, SLOT(changePassphrase()));
    connect(exportPeercoinKeysAction, SIGNAL(triggered()), this, SLOT(exportPeercoinKeys()));
    connect(distributeDividendsAction, SIGNAL(triggered()), this, SLOT(distributeDividendsClicked()));
}

void YbMainWindow::createMenuBar()
{
#ifdef Q_OS_MAC
    // Create a decoupled menu bar on Mac which stays even if the window is closed
    appMenuBar = new QMenuBar();
#else
    // Get the main window's menu bar on other platforms
    appMenuBar = menuBar();
#endif
    // Configure the menus
    QMenu *file = appMenuBar->addMenu(tr("&File"));
    file->addAction(backupWalletAction);
    file->addAction(exportAction);
#ifndef FIRST_CLASS_MESSAGING
    file->addAction(messageAction);
#endif
    file->addSeparator();
    file->addAction(quitAction);

//    QMenu *shares = appMenuBar->addMenu(tr("S&hares"));
//    //shares->addAction(exportPeercoinKeysAction);
//    shares->addAction(distributeDividendsAction);

    QMenu *settings = appMenuBar->addMenu(tr("&Settings"));
//    settings->addAction(encryptWalletAction);
//    settings->addAction(unlockForMintingAction);
//    settings->addAction(changePassphraseAction);
//    settings->addSeparator();
    settings->addAction(optionsAction);

    QMenu *about = appMenuBar->addMenu(tr("&About"));
//    help->addAction(openRPCConsoleAction);
//    help->addSeparator();
    about->addAction(aboutAction);
    about->addAction(aboutQtAction);
}

void YbMainWindow::createTrayIcon()
{
    QMenu *trayIconMenu;
#ifndef Q_OS_MAC
    trayIcon = new QSystemTrayIcon(this);
    trayIconMenu = new QMenu(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setToolTip(tr("ybcoin client"));
    trayIcon->setIcon(QIcon(":/icons/toolbar"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(trayIconActivated(QSystemTrayIcon::ActivationReason)));
    trayIcon->show();
#else
    // Note: On Mac, the dock icon is used to provide the tray's functionality.
    MacDockIconHandler *dockIconHandler = MacDockIconHandler::instance();
    trayIconMenu = dockIconHandler->dockMenu();
#endif

    // Configuration of the tray icon (or dock icon) icon menu
    trayIconMenu->addAction(toggleHideAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(messageAction);
#ifndef FIRST_CLASS_MESSAGING
    trayIconMenu->addSeparator();
#endif
    trayIconMenu->addAction(receiveCoinsAction);
    trayIconMenu->addAction(sendCoinsAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(optionsAction);
#ifndef Q_OS_MAC // This is built-in on Mac
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
#endif

    notificator = new Notificator(tr("ybcoin-qt"), trayIcon);
}

void YbMainWindow::createStatusBar()
{
    statusBar();
    statusBar()->setMinimumWidth(600);

    // Status bar notification icons
    QFrame *frameBlocks = new QFrame();
    frameBlocks->setContentsMargins(0,0,0,0);
    frameBlocks->setMinimumWidth(56);
    frameBlocks->setMaximumWidth(56);
    QHBoxLayout *frameBlocksLayout = new QHBoxLayout(frameBlocks);
    frameBlocksLayout->setContentsMargins(3,0,3,0);
    frameBlocksLayout->setSpacing(3);
    labelEncryptionIcon = new QLabel();
    labelConnectionsIcon = new QLabel();
    labelBlocksIcon = new QLabel();
    frameBlocksLayout->addStretch();
    frameBlocksLayout->addWidget(labelEncryptionIcon);
    frameBlocksLayout->addStretch();
    frameBlocksLayout->addWidget(labelConnectionsIcon);
    frameBlocksLayout->addStretch();
    frameBlocksLayout->addWidget(labelBlocksIcon);
    frameBlocksLayout->addStretch();

    // Progress bar and label for blocks download
    progressBarLabel = new QLabel();
    progressBarLabel->setVisible(false);
    progressBar = new QProgressBar();
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setVisible(false);

    statusBar()->addWidget(progressBarLabel);
    statusBar()->addWidget(progressBar);
    statusBar()->addPermanentWidget(frameBlocks);
}

void YbMainWindow::setNumConnections(int count)
{
    QString icon;
    switch(count)
    {
    case 0: icon = ":/icons/connect_0"; break;
    case 1: case 2: case 3: icon = ":/icons/connect_1"; break;
    case 4: case 5: case 6: icon = ":/icons/connect_2"; break;
    case 7: case 8: case 9: icon = ":/icons/connect_3"; break;
    default: icon = ":/icons/connect_4"; break;
    }
    labelConnectionsIcon->setPixmap(QIcon(icon).pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
    labelConnectionsIcon->setToolTip(tr("%n active connection(s) to ybcoin network", "", count));
}

void YbMainWindow::setNumBlocks(int count)
{
    // don't show / hide progressBar and it's label if we have no connection(s) to the network
    if (!clientModel || clientModel->getNumConnections() == 0)
    {
        progressBarLabel->setVisible(false);
        progressBar->setVisible(false);

        return;
    }

    int nTotalBlocks = clientModel->getNumBlocksOfPeers();
    QString tooltip;

    if(count < nTotalBlocks)
    {
        int nRemainingBlocks = nTotalBlocks - count;
        float nPercentageDone = count / (nTotalBlocks * 0.01f);

        if (clientModel->getStatusBarWarnings() == "")
        {
            progressBarLabel->setText(tr("Synchronizing with network..."));
            progressBarLabel->setVisible(true);
            progressBar->setFormat(tr("~%n block(s) remaining", "", nRemainingBlocks));
            progressBar->setMaximum(nTotalBlocks);
            progressBar->setValue(count);
            progressBar->setVisible(true);
        }
        else
        {
            progressBarLabel->setText(clientModel->getStatusBarWarnings());
            progressBarLabel->setVisible(true);
            progressBar->setVisible(false);
        }
        tooltip = tr("Downloaded %1 of %2 blocks of transaction history (%3% done).").arg(count).arg(nTotalBlocks).arg(nPercentageDone, 0, 'f', 2);
    }
    else
    {
        if (clientModel->getStatusBarWarnings() == "")
            progressBarLabel->setVisible(false);
        else
        {
            progressBarLabel->setText(clientModel->getStatusBarWarnings());
            progressBarLabel->setVisible(true);
        }
        progressBar->setVisible(false);
        tooltip = tr("Downloaded %1 blocks of transaction history.").arg(count);
    }

    QDateTime now = QDateTime::currentDateTime();
    QDateTime lastBlockDate = clientModel->getLastBlockDate();
    int secs = lastBlockDate.secsTo(now);
    QString text;

    // Represent time from last generated block in human readable text
    if(secs <= 0)
    {
        // Fully up to date. Leave text empty.
    }
    else if(secs < 60)
    {
        text = tr("%n second(s) ago","",secs);
    }
    else if(secs < 60*60)
    {
        text = tr("%n minute(s) ago","",secs/60);
    }
    else if(secs < 24*60*60)
    {
        text = tr("%n hour(s) ago","",secs/(60*60));
    }
    else
    {
        text = tr("%n day(s) ago","",secs/(60*60*24));
    }

    // Set icon state: spinning if catching up, tick otherwise
    if(secs < 90*60 && count >= nTotalBlocks)
    {
        tooltip = tr("Up to date") + QString(".\n") + tooltip;
        labelBlocksIcon->setPixmap(QIcon(":/icons/synced").pixmap(STATUSBAR_ICONSIZE, STATUSBAR_ICONSIZE));
    }
    else
    {
        tooltip = tr("Catching up...") + QString("\n") + tooltip;
        labelBlocksIcon->setMovie(syncIconMovie);
        syncIconMovie->start();
    }

    if(!text.isEmpty())
    {
        tooltip += QString("\n");
        tooltip += tr("Last received block was generated %1.").arg(text);
    }

    labelBlocksIcon->setToolTip(tooltip);
    progressBarLabel->setToolTip(tooltip);
    progressBar->setToolTip(tooltip);
}

void YbMainWindow::setEncryptionStatus(int status)
{
    switch(status)
    {
    case WalletModel::Unencrypted:
        labelEncryptionIcon->hide();
        encryptWalletAction->setChecked(false);
        settingPage->setButton1Checked(false);
        changePassphraseAction->setEnabled(false);
        settingPage->setButton3Enabled(false);
        encryptWalletAction->setEnabled(true);
        settingPage->setButton1Enabled(true);
        unlockForMintingAction->setEnabled(false);
        settingPage->setButton2Enabled(false);
        unlockForMintingAction->setChecked(false);
        settingPage->setButton2Checked(false);
        break;
    case WalletModel::Unlocked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_open").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(fWalletUnlockMintOnly? tr("Wallet is <b>encrypted</b> and currently <b>unlocked for block minting only</b>") : tr("Wallet is <b>encrypted</b> and currently <b>unlocked</b>"));
        encryptWalletAction->setChecked(true);
        settingPage->setButton1Checked(true);
        changePassphraseAction->setEnabled(true);
        settingPage->setButton3Enabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        settingPage->setButton1Enabled(false);
        unlockForMintingAction->setEnabled(fWalletUnlockMintOnly);
        settingPage->setButton2Enabled(fWalletUnlockMintOnly);
        unlockForMintingAction->setChecked(fWalletUnlockMintOnly);
        settingPage->setButton2Checked(fWalletUnlockMintOnly);
        break;
    case WalletModel::Locked:
        labelEncryptionIcon->show();
        labelEncryptionIcon->setPixmap(QIcon(":/icons/lock_closed").pixmap(STATUSBAR_ICONSIZE,STATUSBAR_ICONSIZE));
        labelEncryptionIcon->setToolTip(tr("Wallet is <b>encrypted</b> and currently <b>locked</b>"));
        encryptWalletAction->setChecked(true);
        settingPage->setButton1Checked(true);
        changePassphraseAction->setEnabled(true);
        settingPage->setButton3Enabled(true);
        encryptWalletAction->setEnabled(false); // TODO: decrypt currently not supported
        settingPage->setButton1Enabled(false);
        unlockForMintingAction->setEnabled(true);
        settingPage->setButton2Enabled(true);
        unlockForMintingAction->setChecked(false);
        settingPage->setButton2Checked(false);
        break;
    }
}

void YbMainWindow::error(const QString &title, const QString &message, bool modal)
{
    // Report errors from network/worker thread
    if(modal)
    {
        QMessageBox::critical(this, title, message, QMessageBox::Ok, QMessageBox::Ok);
    } else {
        notificator->notify(Notificator::Critical, title, message);
    }
}

void YbMainWindow::askFee(qint64 nFeeRequired, bool *payFee)
{
    QString strMessage =
        tr("This transaction is over the size limit.  You can still send it for a fee of %1, "
          "which goes to the nodes that process your transaction and helps to support the network.  "
          "Do you want to pay the fee?").arg(
                BitcoinUnits::formatWithUnit(BitcoinUnits::BTC, nFeeRequired));
    QMessageBox::StandardButton retval = QMessageBox::question(
          this, tr("Sending..."), strMessage,
          QMessageBox::Yes|QMessageBox::Cancel, QMessageBox::Yes);
    *payFee = (retval == QMessageBox::Yes);
}

void YbMainWindow::handleURI(QString strURI)
{
    showSendCoinsDialog();
    sendCoinsDialog->handleURI(strURI);

    if(!isActiveWindow())
        activateWindow();

    showNormalIfMinimized();
}

void YbMainWindow::gotoOverviewPage()
{
    stackedWidget->setCurrentWidget(overviewPage);
    exportPushButton->setEnabled(false);
    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
}

void YbMainWindow::gotoHistoryPage()
{
    if(overviewButton->isChecked()){
        overviewButton->setChecked(false);
        historyButton->setChecked(true);
    }
    stackedWidget->setCurrentWidget(historyPage);
    exportPushButton->setEnabled(true);
    exportAction->setEnabled(true);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), transactionView, SLOT(exportClicked()));
    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
    connect(exportPushButton, SIGNAL(clicked()), transactionView, SLOT(exportClicked()));
}

void YbMainWindow::gotoReceiveCoinsPage()
{
    receiveCoinsAction->setChecked(true);
    stackedWidget->setCurrentWidget(receiveCoinsPage);
    exportPushButton->setEnabled(true);
    exportAction->setEnabled(true);
    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
    connect(exportPushButton, SIGNAL(clicked()), receiveCoinsPage, SLOT(exportClicked()));
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    connect(exportAction, SIGNAL(triggered()), receiveCoinsPage, SLOT(exportClicked()));
}

void YbMainWindow::gotoSettingPage()
{
    stackedWidget->setCurrentWidget(settingPage);
    exportPushButton->setEnabled(false);
    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
}

void YbMainWindow::gotoHelpPage()
{
    stackedWidget->setCurrentWidget(helpPage);
    exportPushButton->setEnabled(false);
    exportAction->setEnabled(false);
    disconnect(exportAction, SIGNAL(triggered()), 0, 0);
    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
}

void YbMainWindow::showSendCoinsDialog()
{
    sendCoinsDialog->exec();
//    exportPushButton->setEnabled(false);
//    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
}

void YbMainWindow::showReceiveCoinsDialog()
{
    receiveCoinsDialog->show();
    receiveCoinsDialog->setLineEditFocus();
//    exportPushButton->setEnabled(true);
//    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
//    connect(exportPushButton, SIGNAL(clicked()), receiveCoinsDialog, SLOT(exportClicked()));
}

void YbMainWindow::optionsClicked()
{
    if(!clientModel || !clientModel->getOptionsModel())
        return;
    OptionsDialog dlg;
    dlg.setModel(clientModel->getOptionsModel());
    dlg.exec();
}

void YbMainWindow::aboutClicked()
{
    AboutDialog dlg;
    dlg.setModel(clientModel);
    dlg.exec();
}

void YbMainWindow::distributeDividendsClicked()
{
    DistributeDivDialog dd(this);
    dd.exec();
}

#ifndef Q_OS_MAC
void YbMainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
    {
        // Click on system tray icon triggers "show/hide ybcoin"
        toggleHideAction->trigger();
    }
}
#endif

void YbMainWindow::incomingTransaction(const QModelIndex &parent, int start, int end)
{
    if(!walletModel || !clientModel)
        return;
    TransactionTableModel *ttm = walletModel->getTransactionTableModel();
    qint64 amount = ttm->index(start, TransactionTableModel::Amount, parent)
                    .data(Qt::EditRole).toULongLong();
    if(!clientModel->inInitialBlockDownload())
    {
        // On new transaction, make an info balloon
        // Unless the initial block download is in progress, to prevent balloon-spam
        QString date = ttm->index(start, TransactionTableModel::Date, parent)
                        .data().toString();
        QString type = ttm->index(start, TransactionTableModel::Type, parent)
                        .data().toString();
        QString address = ttm->index(start, TransactionTableModel::ToAddress, parent)
                        .data().toString();
        QIcon icon = qvariant_cast<QIcon>(ttm->index(start,
                            TransactionTableModel::ToAddress, parent)
                        .data(Qt::DecorationRole));

        notificator->notify(Notificator::Information,
                            (amount)<0 ? tr("Sent transaction") :
                                         tr("Incoming transaction"),
                              tr("Date: %1\n"
                                 "Amount: %2\n"
                                 "Type: %3\n"
                                 "Address: %4\n")
                              .arg(date)
                              .arg(BitcoinUnits::formatWithUnit(walletModel->getOptionsModel()->getDisplayUnit(), amount, true))
                              .arg(type)
                              .arg(address), icon);
    }
}

void YbMainWindow::encryptWallet(bool status)
{
    if(!walletModel)
        return;
    YbEncryptionDialog dlg(status ? YbEncryptionDialog::Encrypt:
                                     YbEncryptionDialog::Decrypt, this);
    dlg.setModel(walletModel);
    dlg.exec();

    setEncryptionStatus(walletModel->getEncryptionStatus());
}

void YbMainWindow::unlockForMinting(bool status)
{
    if(!walletModel)
        return;

    if (status)
    {
        if(walletModel->getEncryptionStatus() != WalletModel::Locked)
            return;

        YbEncryptionDialog dlg(YbEncryptionDialog::Unlock, this);
        dlg.setModel(walletModel);
        dlg.exec();

        if(walletModel->getEncryptionStatus() != WalletModel::Unlocked)
            return;

        fWalletUnlockMintOnly = true;
    }
    else
    {
        if(walletModel->getEncryptionStatus() != WalletModel::Unlocked)
            return;

        if (!fWalletUnlockMintOnly)
            return;

        walletModel->setWalletLocked(true);
        fWalletUnlockMintOnly = false;
    }
}

void YbMainWindow::backupWallet()
{
#if QT_VERSION < 0x050000
    QString saveDir = QDesktopServices::storageLocation(QDesktopServices::DocumentsLocation);
#else
    QString saveDir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
#endif
    QString filename = QFileDialog::getSaveFileName(this, tr("Backup Portfolio"), saveDir, tr("Portfolio Data (*.dat)"));
    if(!filename.isEmpty()) {
        if(!walletModel->backupWallet(filename)) {
            QMessageBox::warning(this, tr("Backup Failed"), tr("There was an error trying to save the portfolio data to the new location."));
        }
    }
}

void YbMainWindow::changePassphrase()
{
    YbEncryptionDialog dlg(YbEncryptionDialog::ChangePass, this);
    dlg.setModel(walletModel);
    dlg.exec();
}

void YbMainWindow::unlockWallet()
{
    if(!walletModel)
        return;
    // Unlock wallet when requested by wallet model
    if(walletModel->getEncryptionStatus() == WalletModel::Locked)
    {
        YbEncryptionDialog dlg(YbEncryptionDialog::Unlock, this);
        dlg.setModel(walletModel);
        dlg.exec();
    }
}

void YbMainWindow::exportPeercoinKeys()
{
    try {
        int iExportedCount, iErrorCount;
        walletModel->ExportPeercoinKeys(iExportedCount, iErrorCount);
        QMessageBox::information(this,
                tr("Bitcoin keys export"),
                tr("%1 key(s) were exported to Bitcoin.\n%2 key(s) were either already known or invalid.")
                  .arg(iExportedCount)
                  .arg(iErrorCount)
                );
    }
    catch (std::runtime_error &e) {
        QMessageBox::critical(this,
                tr("Bitcoin keys export"),
                tr("Error: %1").arg(e.what()));
    }
}

void YbMainWindow::showSendSignDialog()
{
#ifdef FIRST_CLASS_MESSAGING
    messageAction->setChecked(true);
    exportPushButton->setEnabled(false);
    disconnect(exportPushButton, SIGNAL(clicked()), 0, 0);
#else
    messagePage->show();
    messagePage->setFocus();
#endif
}

void YbMainWindow::showSendSignDialog(QString addr)
{
    showSendSignDialog();
    messagePage->setAddress(addr);
}

void YbMainWindow::showNormalIfMinimized()
{
    if(!isVisible()) // Show, if hidden
        show();
    if(isMinimized()) // Unminimize, if minimized
        showNormal();
}

void YbMainWindow::toggleHidden()
{
    // activateWindow() (sometimes) helps with keyboard focus on Windows
    if (isHidden())
    {
        show();
        activateWindow();
    }
    else if (isMinimized())
    {
        showNormal();
        activateWindow();
    }
    else if (GUIUtil::isObscured(this))
    {
        raise();
        activateWindow();
    }
    else
        hide();
}

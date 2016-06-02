#include "toolwidget.h"
#include "toolbutton.h"

#include <QHBoxLayout>
#include <QSpacerItem>

ToolWidget::ToolWidget(QWidget *parent) :
    QWidget(parent)
{
    createButtons();
    createConnections();
    createMainLayout();
    setLayout(mainLayout);
    setFixedHeight(60);
}

void ToolWidget::setExportButtonEnabled(bool is)
{
    exportButton->setEnabled(is);
}

void ToolWidget::createButtons()
{
    QPixmap overviewPix(":icons/overview");
    overviewButton = new ToolButton(overviewPix, tr("Overview"));
    overviewButton->setToolTip(tr("Show general overview of holdings"));
//    overviewButton->setCheckable(true);
//    overviewButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_1));
    overviewButton->setChecked(true);

    QPixmap sendPix(":icons/send");
    sendCoinsButton = new ToolButton(sendPix, tr("Send shares"));
    sendCoinsButton->setToolTip(tr("Send shares to a ybcoin address"));
//    sendCoinsButton->setCheckable(true);
//    sendCoinsButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_2));

    QPixmap receivePix(":icons/receiving_addresses");
    receiveCoinsButton = new ToolButton(receivePix, tr("Receive shares"));
    receiveCoinsButton->setToolTip(tr("Show the list of addresses for receiving payments"));
//    receiveCoinsButton->setCheckable(true);
//    receiveCoinsButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_3));

    QPixmap historyPix(":icons/history");
    historyButton = new ToolButton(historyPix, tr("Transactions"));
    historyButton->setToolTip(tr("Browse transaction history"));
//    historyButton->setCheckable(true);
//    historyButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_4));

    QPixmap addressPix(":icons/address-book");
    addressBookButton = new ToolButton(addressPix, tr("Address Book"));
    addressBookButton->setToolTip(tr("Edit the list of stored addresses and labels"));
//    addressBookButton->setCheckable(true);
//    addressBookButton->setShortcut(QKeySequence(Qt::ALT + Qt::Key_5));

    QPixmap messagePix(":icons/edit");
    messageButton = new ToolButton(messagePix, tr("Sign message"));
    messageButton->setToolTip(tr("Prove you control an address"));
#ifdef FIRST_CLASS_MESSAGING
    messageButton->setCheckable(true);
#endif

    QPixmap exportPix(":icons/export");
    exportButton = new ToolButton(exportPix, tr("Export..."), 0, true);
    exportButton->setToolTip(tr("Export the data in the current tab to a file"));
}

void ToolWidget::createMainLayout()
{
    mainLayout = new QHBoxLayout;
    mainLayout->setContentsMargins(5,0,5,0);
    mainLayout->addWidget(overviewButton);
    mainLayout->addWidget(sendCoinsButton);
    mainLayout->addWidget(receiveCoinsButton);
    mainLayout->addWidget(historyButton);
    mainLayout->addWidget(addressBookButton);
#ifdef FIRST_CLASS_MESSAGING
    mainLayout->addWidget(messageButton);
#endif
    mainLayout->addWidget(exportButton);
    QSpacerItem* hSpacer = new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    mainLayout->addSpacerItem(hSpacer);

}

void ToolWidget::createConnections()
{
    connect(overviewButton, SIGNAL(toolClicked()), this, SLOT(overviewButtonChecked()));
    connect(sendCoinsButton, SIGNAL(toolClicked()), this, SLOT(sendCoinsButtonChecked()));
    connect(receiveCoinsButton, SIGNAL(toolClicked()), this, SLOT(receiveCoinsButtonChecked()));
    connect(historyButton, SIGNAL(toolClicked()), this, SLOT(historyButtonChecked()));
    connect(addressBookButton, SIGNAL(toolClicked()), this, SLOT(addressBookButtonChecked()));
    connect(messageButton, SIGNAL(toolClicked()), this, SLOT(messageButtonChecked()));
    connect(exportButton, SIGNAL(toolClicked()), this, SLOT(exportButtonChecked()));
}

void ToolWidget::overviewButtonChecked()
{
    sendCoinsButton->setChecked(false);
    receiveCoinsButton->setChecked(false);
    historyButton->setChecked(false);
    addressBookButton->setChecked(false);
    messageButton->setChecked(false);
    emit overviewTriggered();
}

void ToolWidget::sendCoinsButtonChecked()
{
    overviewButton->setChecked(false);
    receiveCoinsButton->setChecked(false);
    historyButton->setChecked(false);
    addressBookButton->setChecked(false);
    messageButton->setChecked(false);
    emit sendCoinsTriggered();
}

void ToolWidget::receiveCoinsButtonChecked()
{
    overviewButton->setChecked(false);
    sendCoinsButton->setChecked(false);
    historyButton->setChecked(false);
    addressBookButton->setChecked(false);
    messageButton->setChecked(false);
    emit receiveCoinsTriggered();
}

void ToolWidget::historyButtonChecked()
{
    overviewButton->setChecked(false);
    sendCoinsButton->setChecked(false);
    receiveCoinsButton->setChecked(false);
    addressBookButton->setChecked(false);
    messageButton->setChecked(false);
    emit historyTriggered();
}

void ToolWidget::addressBookButtonChecked()
{
    overviewButton->setChecked(false);
    sendCoinsButton->setChecked(false);
    receiveCoinsButton->setChecked(false);
    historyButton->setChecked(false);
    messageButton->setChecked(false);
    emit addressTriggered();
}

void ToolWidget::messageButtonChecked()
{
    overviewButton->setChecked(false);
    sendCoinsButton->setChecked(false);
    receiveCoinsButton->setChecked(false);
    historyButton->setChecked(false);
    addressBookButton->setChecked(false);
    emit messageTriggered();
}

void ToolWidget::exportButtonChecked()
{
    emit exportTriggered();
}

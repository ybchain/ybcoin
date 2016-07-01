#include "ybaddressbookpage.h"
#include "ybpushbutton.h"
#include "addresstablemodel.h"
#include "ybmainwindow.h"
#include "editaddressdialog.h"
#include "csvmodelwriter.h"
#include "guiutil.h"

#include <QSortFilterProxyModel>
#include <QClipboard>
#include <QMessageBox>
#include <QMenu>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QTableView>
#include <QHeaderView>

#ifdef USE_QRCODE
#include "qrcodedialog.h"
#endif

YbAddressBookPage::YbAddressBookPage(Mode mode, Tabs tab, QWidget *parent) :
    model(0),
    mode(mode),
    tab(tab),
    QDialog(parent)
{
    createWidget();

#ifdef Q_WS_MAC // Icons on push buttons are very uncommon on Mac
    newAddressButton->setIcon(QIcon());
    copyAddressButton->setIcon(QIcon());
    deleteAddressButton->setIcon(QIcon());
#endif

#ifndef USE_QRCODE
    showQRCodeButton->setVisible(false);
#endif

    switch(mode)
    {
    case ForSending:
        connect(tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(accept()));
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView->setFocus();
        break;
    case ForEditing:
        //ui->buttonBox->setVisible(false);
        break;
    }
    switch(tab)
    {
    case SendingTab:
        //ui->labelExplanation->setVisible(false);
        deleteAddressButton->setVisible(true);
        signMessageButton->setVisible(false);
        break;
    case ReceivingTab:
        deleteAddressButton->setVisible(false);
        signMessageButton->setVisible(true);
        break;
    }
    tableView->setTabKeyNavigation(false);
    tableView->setContextMenuPolicy(Qt::CustomContextMenu);

    // Context menu actions
    QAction *copyAddressAction = new QAction(tr("Copy address"), this);
    QAction *copyLabelAction = new QAction(tr("Copy label"), this);
    QAction *editAction = new QAction(tr("Edit"), this);
    deleteAction = new QAction(tr("Delete"), this);

    contextMenu = new QMenu();
    contextMenu->addAction(copyAddressAction);
    contextMenu->addAction(copyLabelAction);
    contextMenu->addAction(editAction);
    contextMenu->addAction(deleteAction);

    connect(copyAddressAction, SIGNAL(triggered()), this, SLOT(copyAddress()));
    connect(copyLabelAction, SIGNAL(triggered()), this, SLOT(onCopyLabelAction()));
    connect(editAction, SIGNAL(triggered()), this, SLOT(onEditAction()));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteAddress()));

    connect(tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextualMenu(QPoint)));

    // Pass through accept action from button box
    //connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
}

YbAddressBookPage::~YbAddressBookPage()
{
    if(newAddressButton != NULL){
        delete newAddressButton;
    }
    if(deleteAddressButton != NULL){
        delete deleteAddressButton;
    }
    if(copyAddressButton != NULL){
        delete copyAddressButton;
    }
    if(showQRCodeButton != NULL){
        delete showQRCodeButton;
    }
    if(signMessageButton != NULL){
        delete signMessageButton;
    }
    if(tableView != NULL){
        delete tableView;
    }
}

void YbAddressBookPage::setModel(AddressTableModel *model)
{
    this->model = model;
    if(!model)
        return;

    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    proxyModel->setDynamicSortFilter(true);
    switch(tab)
    {
    case ReceivingTab:
        // Receive filter
        proxyModel->setFilterRole(AddressTableModel::TypeRole);
        proxyModel->setFilterFixedString(AddressTableModel::Receive);
        break;
    case SendingTab:
        // Send filter
        proxyModel->setFilterRole(AddressTableModel::TypeRole);
        proxyModel->setFilterFixedString(AddressTableModel::Send);
        break;
    }
    tableView->setModel(proxyModel);
    tableView->sortByColumn(0, Qt::AscendingOrder);

    // Set column widths
    tableView->horizontalHeader()->resizeSection(
            AddressTableModel::Address, 320);
    tableView->horizontalHeader()->resizeSection(
            AddressTableModel::DividendAddress, 320);
#if QT_VERSION < 0x050000
    tableView->horizontalHeader()->setResizeMode(
            AddressTableModel::Label, QHeaderView::Stretch);
#else
    tableView->horizontalHeader()->setSectionResizeMode(
            AddressTableModel::Label, QHeaderView::Stretch);
#endif
    connect(tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(selectionChanged()));

    selectionChanged();
}

void YbAddressBookPage::done(int retval)
{
    QTableView *table = tableView;
    if(!table->selectionModel() || !table->model())
        return;
    // When this is a tab/widget and not a model dialog, ignore "done"
    if(mode == ForEditing)
        return;

    // Figure out which address was selected, and return it
    QModelIndexList indexes = table->selectionModel()->selectedRows(AddressTableModel::Address);

    foreach (QModelIndex index, indexes)
    {
        QVariant address = table->model()->data(index);
        returnValue = address.toString();
    }

    if(returnValue.isEmpty())
    {
        // If no address entry selected, return rejected
        retval = Rejected;
    }

    QDialog::done(retval);
}

void YbAddressBookPage::exportClicked()
{
    // CSV is currently the only supported format
    QString filename = GUIUtil::getSaveFileName(
            this,
            tr("Export Address Book Data"), QString(),
            tr("Comma separated file (*.csv)"));

    if (filename.isNull()) return;

    CSVModelWriter writer(filename);

    // name, column, role
    writer.setModel(proxyModel);
    writer.addColumn("Label", AddressTableModel::Label, Qt::EditRole);
    writer.addColumn("Address", AddressTableModel::Address, Qt::EditRole);

    if(!writer.write())
    {
        QMessageBox::critical(this, tr("Error exporting"), tr("Could not write to file %1.").arg(filename),
                              QMessageBox::Abort, QMessageBox::Abort);
    }
}

void YbAddressBookPage::createWidget()
{
    setStyleSheet("QTableView{border: 2px groove rgb(211, 211, 211)}");
    QPixmap addPix(":icons/add");
    QPixmap copyPix(":icons/copy");
    QPixmap deletePix(":icons/delete");
    QPixmap qrcodePix(":images/qrcode");
    QPixmap signPix(":icons/edit");
    QColor color(99, 184, 255);
    newAddressButton = new YbPushButton(addPix, tr("新地址"), color, color, 3, 95, 25);
    copyAddressButton = new YbPushButton(copyPix, tr("复制"), color, color, 3, 95, 25);
    deleteAddressButton = new YbPushButton(deletePix, tr("删除"), color, color, 3, 95, 25);
    showQRCodeButton = new YbPushButton(qrcodePix, tr("二维码"), color, color, 3, 95, 25);
    signMessageButton = new YbPushButton(signPix, tr("签名"), color, color, 3, 95, 25);
    connect(newAddressButton, SIGNAL(clicked()), this, SLOT(newAddress()));
    connect(copyAddressButton, SIGNAL(clicked()), this, SLOT(copyAddress()));
    connect(deleteAddressButton, SIGNAL(clicked()), this, SLOT(deleteAddress()));
    connect(showQRCodeButton, SIGNAL(clicked()), this, SLOT(showQRCode()));
    connect(signMessageButton, SIGNAL(clicked()), this, SLOT(signMessage()));

    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setSpacing(10);
    hlayout->addSpacing(20);
    hlayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hlayout->addWidget(newAddressButton);
    hlayout->addWidget(copyAddressButton);
    hlayout->addWidget(deleteAddressButton);
    hlayout->addWidget(showQRCodeButton);
    hlayout->addWidget(signMessageButton);
    hlayout->addSpacing(20);
    tableView = new QTableView(this);
    QHBoxLayout *viewhLayout = new QHBoxLayout;
    viewhLayout->addSpacing(20);
    viewhLayout->addWidget(tableView);
    viewhLayout->addSpacing(20);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setSpacing(10);
    mainLayout->addLayout(hlayout);
    mainLayout->addLayout(viewhLayout);
    setLayout(mainLayout);
}

void YbAddressBookPage::deleteAddress()
{
    QTableView *table = tableView;
    if(!table->selectionModel())
        return;
    QModelIndexList indexes = table->selectionModel()->selectedRows();
    if(!indexes.isEmpty())
    {
        table->model()->removeRow(indexes.at(0).row());
    }
}

void YbAddressBookPage::newAddress()
{
    if(!model)
        return;
    EditAddressDialog dlg(
            tab == SendingTab ?
            EditAddressDialog::NewSendingAddress :
            EditAddressDialog::NewReceivingAddress);
    dlg.setModel(model);
    if(dlg.exec())
    {
        // Select row for newly created address
        QString address = dlg.getAddress();
        QModelIndexList lst = proxyModel->match(proxyModel->index(0,
                          AddressTableModel::Address, QModelIndex()),
                          Qt::EditRole, address, 1, Qt::MatchExactly);
        if(!lst.isEmpty())
        {
            tableView->setFocus();
            tableView->selectRow(lst.at(0).row());
        }
    }
}

void YbAddressBookPage::copyAddress()
{
    GUIUtil::copyEntryData(tableView, AddressTableModel::Address);
}

void YbAddressBookPage::signMessage()
{
    QTableView *table = tableView;
    QModelIndexList indexes = table->selectionModel()->selectedRows(AddressTableModel::Address);
    QString addr;

    foreach (QModelIndex index, indexes)
    {
        QVariant address = index.data();
        addr = address.toString();
    }

    emit showSignDialog(addr);
}

void YbAddressBookPage::selectionChanged()
{
    // Set button states based on selected tab and selection
    QTableView *table = tableView;
    if(!table->selectionModel())
        return;

    if(table->selectionModel()->hasSelection())
    {
        switch(tab)
        {
        case SendingTab:
            // In sending tab, allow deletion of selection
            deleteAddressButton->setEnabled(true);
            deleteAddressButton->setVisible(true);
            deleteAction->setEnabled(true);
            signMessageButton->setEnabled(false);
            signMessageButton->setVisible(false);
            break;
        case ReceivingTab:
            // Deleting receiving addresses, however, is not allowed
            deleteAddressButton->setEnabled(false);
            deleteAddressButton->setVisible(false);
            deleteAction->setEnabled(false);
            signMessageButton->setEnabled(true);
            signMessageButton->setVisible(true);
            break;
        }
        copyAddressButton->setEnabled(true);
        showQRCodeButton->setEnabled(true);
    }
    else
    {
        deleteAddressButton->setEnabled(false);
        showQRCodeButton->setEnabled(false);
        copyAddressButton->setEnabled(false);
        signMessageButton->setEnabled(false);
    }
}

void YbAddressBookPage::showQRCode()
{
#ifdef USE_QRCODE
    QTableView *table = tableView;
    QModelIndexList indexes = table->selectionModel()->selectedRows(AddressTableModel::Address);

    foreach (QModelIndex index, indexes)
    {
        QString address = index.data().toString(), label = index.sibling(index.row(), 0).data(Qt::EditRole).toString();

        QRCodeDialog *dialog = new QRCodeDialog(address, label, tab == ReceivingTab, this);
        dialog->show();
    }
#endif
}

void YbAddressBookPage::contextualMenu(const QPoint &point)
{
    QModelIndex index = tableView->indexAt(point);
    if(index.isValid())
    {
        contextMenu->exec(QCursor::pos());
    }
}

void YbAddressBookPage::onCopyLabelAction()
{
    GUIUtil::copyEntryData(tableView, AddressTableModel::Label);
}

void YbAddressBookPage::onEditAction()
{
    if(!tableView->selectionModel())
        return;
    QModelIndexList indexes = tableView->selectionModel()->selectedRows();
    if(indexes.isEmpty())
        return;

    EditAddressDialog dlg(
            tab == SendingTab ?
            EditAddressDialog::EditSendingAddress :
            EditAddressDialog::EditReceivingAddress);
    dlg.setModel(model);
    QModelIndex origIndex = proxyModel->mapToSource(indexes.at(0));
    dlg.loadRow(origIndex.row());
    dlg.exec();
}

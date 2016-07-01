#include "ybhelppage.h"
#include "ybtabwidget.h"
#include "ybinformation.h"
#include "ybconsole.h"

#include <QVBoxLayout>

YbHelpPage::YbHelpPage(QWidget *parent) :
    QWidget(parent)
{
    createWidget();
}

void YbHelpPage::setClientModel(ClientModel *model)
{
    information->setClientModel(model);
}

void YbHelpPage::createWidget()
{
    information = new YbInformation;
    console = new YbConsole;

    ybTabWidget = new YbTabWidget(information, console);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addWidget(ybTabWidget);
    setLayout(mainLayout);
}

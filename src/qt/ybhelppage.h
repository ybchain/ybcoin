#ifndef YBHELPPAGE_H
#define YBHELPPAGE_H

#include <QWidget>

class ClientModel;
class YbTabWidget;
class YbInformation;
class YbConsole;

class YbHelpPage : public QWidget
{
    Q_OBJECT
public:
    explicit YbHelpPage(QWidget *parent = 0);

    void setClientModel(ClientModel *model);

private:
    void createWidget();

    YbInformation *information;
    YbTabWidget *ybTabWidget;
    YbConsole *console;
};

#endif // YBHELPPAGE_H

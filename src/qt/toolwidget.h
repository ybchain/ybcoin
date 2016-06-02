#ifndef TOOLWIDGET_H
#define TOOLWIDGET_H

#include <QWidget>

class QHBoxLayout;
class ToolButton;

class ToolWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolWidget(QWidget *parent = 0);

    void setExportButtonEnabled(bool is);

private:
    ToolButton *overviewButton;
    ToolButton *sendCoinsButton;
    ToolButton *receiveCoinsButton;
    ToolButton *historyButton;
    ToolButton *addressBookButton;
    ToolButton *messageButton;
    ToolButton *exportButton;
    QHBoxLayout *mainLayout;

    void createButtons();
    void createMainLayout();
    void createConnections();

signals:
    void overviewTriggered();
    void sendCoinsTriggered();
    void receiveCoinsTriggered();
    void historyTriggered();
    void addressTriggered();
    void messageTriggered();
    void exportTriggered();

private slots:
    void overviewButtonChecked();
    void sendCoinsButtonChecked();
    void receiveCoinsButtonChecked();
    void historyButtonChecked();
    void addressBookButtonChecked();
    void messageButtonChecked();
    void exportButtonChecked();
};

#endif // TOOLWIDGET_H

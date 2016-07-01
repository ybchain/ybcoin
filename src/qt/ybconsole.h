#ifndef YBCONSOLE_H
#define YBCONSOLE_H

#include <QDialog>

class QLineEdit;
class QTextEdit;
class YbPushButton;

class YbConsole : public QDialog
{
    Q_OBJECT
public:
    explicit YbConsole(QWidget *parent = 0);
    ~YbConsole();

    enum MessageClass {
        MC_ERROR,
        MC_DEBUG,
        CMD_REQUEST,
        CMD_REPLY,
        CMD_ERROR
    };

    void setLineEditFocus();

protected:
    virtual bool eventFilter(QObject* obj, QEvent *event);

private slots:
    void lineEditPressed();

public slots:
    void clear();
    void message(int category, const QString &message, bool html = false);
    /** Go forward or back in history */
    void browseHistory(int offset);
    /** Scroll console view to end */
    void scrollToEnd();

signals:
    // For RPC command executor
    void stopExecutor();
    void cmdRequest(const QString &command);

private:
    void createWidget();

    QTextEdit *messagesWidget;
    QLineEdit *lineEdit;
    YbPushButton *clearButton;
    QStringList history;
    int historyPtr;

    void startExecutor();
};

#endif // YBCONSOLE_H

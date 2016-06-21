#ifndef YBMESSAGEDIALOGTITLE_H
#define YBMESSAGEDIALOGTITLE_H

#include <QWidget>

class QLabel;

class YbMessageDialogTitle : public QWidget
{
    Q_OBJECT
public:
    explicit YbMessageDialogTitle(QPixmap pixmap, QString title, QString titleInfo, QWidget *parent = 0);
    explicit YbMessageDialogTitle(QPixmap pixmap, QString title, QWidget *parent = 0);
    explicit YbMessageDialogTitle(QString title, QWidget *parent = 0);
    ~YbMessageDialogTitle();
    void setTitle(const QString &title);
    void setTitleInfo(const QString &titleInfo);

private:
    QFont boldFont;
    QLabel *title;
    QLabel *titleInfo;
};

#endif // YBMESSAGEDIALOGTITLE_H

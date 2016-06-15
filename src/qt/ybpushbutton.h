#ifndef YBPUSHBUTTON_H
#define YBPUSHBUTTON_H

#include <QPushButton>

class YbPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit YbPushButton(QPixmap pixmap, QString text, QWidget *parent = 0);
    explicit YbPushButton(QString text, QWidget *parent = 0);
};

#endif // YBPUSHBUTTON_H

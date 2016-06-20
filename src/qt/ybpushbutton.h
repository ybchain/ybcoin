#ifndef YBPUSHBUTTON_H
#define YBPUSHBUTTON_H

#include <QPushButton>

class YbPushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit YbPushButton(QPixmap pixmap, QString text, QWidget *parent = 0);
    explicit YbPushButton(QString text, QWidget *parent = 0);
    explicit YbPushButton(QString text, int width, int height, bool hasBackColor, QWidget *parent = 0);
    explicit YbPushButton(QPixmap pixmap, QWidget *parent = 0);
    explicit YbPushButton(QPixmap pixmap, QString text, QColor borderColor, QColor textColor, int radius, int width = 60, int height = 40, QWidget *parent = 0);
};

#endif // YBPUSHBUTTON_H

#include "ybpushbutton.h"

YbPushButton::YbPushButton(QPixmap pixmap, QString text, QWidget *parent) :
    QPushButton(parent)
{
    setFixedHeight(25);
    setFixedWidth(90);
    setIcon(pixmap);
    setText(text);
    setStyleSheet("QPushButton{background-color: rgb(255, 255, 255); border: 2px groove gray; border-radius: 5px;} QPushButton:hover{background-color: rgb(224, 238, 238);} QPushButton:pressed{background-color: rgb(220, 220, 220);} QPushButton:disabled{color: rgb(190, 190, 190);}");
}

YbPushButton::YbPushButton(QString text, QWidget *parent) :
    QPushButton(parent)
{
    setFixedWidth(60);
    setFixedHeight(40);
    setText(text);
    setStyleSheet("QPushButton{background-color: rgb(30, 144, 255); border-radius: 5px;} QPushButton:hover{background-color: rgb(0, 0, 255);} QPushButton:pressed{background-color: rgb(65, 105, 225);} QPushButton:disabled{background-color: rgb(190, 190, 190);}");
}

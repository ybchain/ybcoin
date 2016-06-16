#include "ybtabbutton.h"

#include <QLabel>
#include <QHBoxLayout>

YbTabButton::YbTabButton(QString text, bool isChecked, QWidget *parent) :
    QPushButton(parent)
{
    setCheckable(true);
    setFixedHeight(35);
    setFixedWidth(100);
    setText(text);
    setStyleSheet("QPushButton{background-color: rgb(255, 255, 255); border: 1.5px groove gray; border-radius: 8px;} QPushButton:hover{background-color: rgb(255, 255, 255);} QPushButton:checked{background-color: rgb(30, 144, 255);} QPushButton:disabled{background-color: rgb(190, 190, 190);}");
    setChecked(isChecked);
}

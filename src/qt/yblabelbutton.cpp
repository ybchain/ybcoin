#include "yblabelbutton.h"

#include <QMouseEvent>

YbLabelButton::YbLabelButton(QPixmap pix, QWidget *parent) :
    QLabel(parent)
{
    setPixmap(pix);
    setFixedSize(pix.size());
}

void YbLabelButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->rect().contains(e->pos())){
        emit labelButtonClicked();
    }
}

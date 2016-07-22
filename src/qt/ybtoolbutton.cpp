#include "ybtoolbutton.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPainter>
#include <QBitmap>
#include <QMouseEvent>
#include <QFont>

YbToolButton::YbToolButton(QPixmap pixmap, QPixmap checkedPixmap, QString text,QWidget *parent, bool isCheckable) :
    m_pixmap(pixmap), m_checkedPixmap(checkedPixmap), QWidget(parent), m_isCheckable(isCheckable), m_isChecked(false)
{
    boldFont.setBold(true);
    m_picLabel = new QLabel;
    m_textLabel = new QLabel(text);
    setDefaultState();
    setAutoFillBackground(true);
    setMinimumHeight(40);
    setMinimumWidth(250);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addSpacing(20);
    mainLayout->addWidget(m_picLabel, 0, Qt::AlignVCenter);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(m_textLabel, 0, Qt::AlignVCenter);
    setLayout(mainLayout);
}

void YbToolButton::setChecked(bool isChecked)
{
    if(isEnabled()){
        if(m_isCheckable){
            m_isChecked = isChecked;
            if(m_isChecked){
                setBkPalette(224, 238, 238);
            }else{
                setBkPalette(255, 255, 255, 0);
            }
        }
    }
}

bool YbToolButton::isChecked()
{
    return m_isCheckable;
}

void YbToolButton::setDefaultState()
{
    setChecked(false);
    m_textLabel->setFont(normalFont);
    m_picLabel->clear();
    m_picLabel->setPixmap(m_pixmap);
    m_picLabel->setFixedSize(m_pixmap.size());
}

void YbToolButton::paintEvent(QPaintEvent *e)
{
    (void)e;
    QBitmap bitmap(this->size());
    bitmap.fill();
    QPainter painter(&bitmap);
    painter.setRenderHints(QPainter::Antialiasing,true);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QBrush(Qt::black));
    painter.drawRoundedRect(bitmap.rect(),5,5);
    setMask(bitmap);
}

void YbToolButton::enterEvent(QEvent *e)
{
    (void)e;
    if(m_isCheckable){
        if(isEnabled() && !m_isChecked){
            setBkPalette(224, 238, 238);
        }
    }else{
        if(isEnabled()){
        }
    }
}

void YbToolButton::mousePressEvent(QMouseEvent *e)
{
    (void)e;
    if(m_isCheckable){
        if(isEnabled() && !m_isChecked){
            setBkPalette(224, 238, 238);
            m_textLabel->setFont(boldFont);
            m_picLabel->clear();
            m_picLabel->setPixmap(m_checkedPixmap);
            m_picLabel->setFixedSize(m_checkedPixmap.size());
        }
    }else{
        if(isEnabled()){
        }
    }
}

void YbToolButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(this->rect().contains(e->pos())){
        if(m_isCheckable){
            if(isEnabled() && !m_isChecked){
                m_isChecked = true;
                setBkPalette(224, 238, 238);
            }
        }else{
            if(isEnabled()){
            }
        }
        emit toolClicked();
    }
}

void YbToolButton::leaveEvent(QEvent *e)
{
    if(m_isCheckable){
        if(isEnabled() && !m_isChecked){
            setDefaultState();
        }
    }else{
    }
}

void YbToolButton::setBkPalette(int r, int g, int b, int a)
{
    QPalette palette;
    palette.setBrush(QPalette::Window,QBrush(QColor(r, g, b, a)));
    setPalette(palette);
}

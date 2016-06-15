#ifndef YBLABEL_H
#define YBLABEL_H

#include <QLabel>

enum labelType{
    enable,
    forbidden,
    warning
};

class YbLabel : public QLabel
{
    Q_OBJECT
public:
    explicit YbLabel(QString text, labelType type = enable, QWidget *parent = 0);
    void setLabelType(labelType type);
};

#endif // YBLABEL_H

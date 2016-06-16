#ifndef YBHELPPAGE_H
#define YBHELPPAGE_H

#include <QWidget>

class YbTabWidget;
class YbInformation;

class YbHelpPage : public QWidget
{
    Q_OBJECT
public:
    explicit YbHelpPage(QWidget *parent = 0);

private:
    YbInformation *information;
    YbTabWidget *ybTabWidget;
};

#endif // YBHELPPAGE_H

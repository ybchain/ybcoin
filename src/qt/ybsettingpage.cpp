#include "ybsettingpage.h"

#include "yblabel.h"
#include "ybpushbutton.h"
#include "ybseparater.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

YbSettingPage::YbSettingPage(QWidget *parent) :
    QWidget(parent)
{
    createWidget();
}

void YbSettingPage::createWidget()
{
    qlabel1 = new QLabel(tr("加密钱包"));
    qlabel2 = new QLabel(tr("仅解锁锻造"));
    qlabel3 = new QLabel(tr("修改口令"));
    label1 = new YbLabel(this);
    label1->setText(tr("密码设置"));
    label1->setLabelType(enable);
    label2 = new YbLabel(this);
    label2->setText(tr("已禁用"));
    label2->setLabelType(forbidden);
    label3 = new YbLabel(this);
    label3->setText(tr("已禁用"));
    label3->setLabelType(forbidden);
    textLabel1 = new QLabel(tr("您的密码不会在我们的服务器上共享，这意味着如果您忘记了密码，我们将\n无法帮助您重置，请记录下您的恢复短语，这将帮助您在丢失密码的情况下\n恢复钱包的访问权限。"));
    textLabel2 = new QLabel(tr("您的密码不会在我们的服务器上共享，这意味着如果您忘记了密码，我们将\n无法帮助您重置，请记录下您的恢复短语，这将帮助您在丢失密码的情况下\n恢复钱包的访问权限。"));
    textLabel3 = new QLabel(tr("您的YBC钱包绝不会将密码传送到我们的服务器，这意味着我们无法知晓您的\n密码，并且如果您忘记了密码，我们无法为您重置。请创建一个能够牢记的\n口令，我们可在您忘记密码的情况下将其发送到您经过验证的电子邮件地址。"));
    warningLabel = new YbLabel(this);
    warningLabel->setText(tr("安全提示：确保您的信息正确并处于最新状态，以防止您的钱包遭受未经授权的访问，并在丢失钱包ID或密码的\n情况下帮助您恢复钱包的访问权限。"));
    warningLabel->setLabelType(warning);
    button1 = new YbPushButton(tr("更改"));
    button1->setCheckable(true);
    button2 = new YbPushButton(tr("更改"));
    button2->setCheckable(true);
    button3 = new YbPushButton(tr("更改"));

    connect(button1, SIGNAL(clicked(bool)), this, SIGNAL(button1Clicked(bool)));
    connect(button2, SIGNAL(clicked(bool)), this, SIGNAL(button2Clicked(bool)));
    connect(button3, SIGNAL(clicked()), this, SIGNAL(button3Clicked()));

    QHBoxLayout *hLayout1 = new QHBoxLayout;
    QHBoxLayout *hLayout1_h = new QHBoxLayout;
    QVBoxLayout *hLayout1_v = new QVBoxLayout;
    hLayout1_h->addWidget(qlabel1);
    hLayout1_h->addSpacing(30);
    hLayout1_h->addWidget(label1);
    hLayout1_h->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hLayout1_v->addLayout(hLayout1_h);
    hLayout1_v->addSpacing(10);
    hLayout1_v->addWidget(textLabel1);
    hLayout1->addLayout(hLayout1_v);
    hLayout1->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hLayout1->addWidget(button1, Qt::AlignVCenter);

    QHBoxLayout *hLayout2 = new QHBoxLayout;
    QHBoxLayout *hLayout2_h = new QHBoxLayout;
    QVBoxLayout *hLayout2_v = new QVBoxLayout;
    hLayout2_h->addWidget(qlabel2);
    hLayout2_h->addSpacing(30);
    hLayout2_h->addWidget(label2);
    hLayout2_h->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hLayout2_v->addLayout(hLayout2_h);
    hLayout2_v->addSpacing(10);
    hLayout2_v->addWidget(textLabel2);
    hLayout2->addLayout(hLayout2_v);
    hLayout2->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hLayout2->addWidget(button2, Qt::AlignVCenter);

    QHBoxLayout *hLayout3 = new QHBoxLayout;
    QHBoxLayout *hLayout3_h = new QHBoxLayout;
    QVBoxLayout *hLayout3_v = new QVBoxLayout;
    hLayout3_h->addWidget(qlabel3);
    hLayout3_h->addSpacing(30);
    hLayout3_h->addWidget(label3);
    hLayout3_h->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    hLayout3_v->addLayout(hLayout3_h);
    hLayout3_v->addSpacing(10);
    hLayout3_v->addWidget(textLabel3);
    hLayout3->addLayout(hLayout3_v);
    hLayout3->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum));
    hLayout3->addWidget(button3, Qt::AlignVCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addSpacing(10);
    mainLayout->addWidget(warningLabel);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(hLayout1);
    mainLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new YbSeparater);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(hLayout2);
    mainLayout->addSpacerItem(new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new YbSeparater);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(hLayout3);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(new YbSeparater);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QHBoxLayout *wholeLayout = new QHBoxLayout;
    wholeLayout->addSpacing(40);
    wholeLayout->addLayout(mainLayout);
    wholeLayout->addSpacing(40);

    setLayout(wholeLayout);
}

void YbSettingPage::setButton1Enabled(bool is)
{
    QString text;
    if(is){
        text = QString(tr("密码设置"));
        label1->setText(text);
        label1->setLabelType(enable);
    }else{
        text = QString(tr("已禁用"));
        label1->setText(text);
        label1->setLabelType(forbidden);
    }
    button1->setEnabled(is);
}

void YbSettingPage::setButton2Enabled(bool is)
{
    QString text;
    if(is){
        text = QString(tr("解锁锻造"));
        label2->setText(text);
        label2->setLabelType(enable);
    }else{
        text = QString(tr("已禁用"));
        label2->setText(text);
        label2->setLabelType(forbidden);
    }
    button2->setEnabled(is);
}

void YbSettingPage::setButton3Enabled(bool is)
{
    QString text;
    if(is){
        text = QString(tr("口令设置"));
        label3->setText(text);
        label3->setLabelType(enable);
    }else{
        text = QString(tr("已禁用"));
        label3->setText(text);
        label3->setLabelType(forbidden);
    }
    button3->setEnabled(is);
}

void YbSettingPage::setButton1Checked(bool is)
{
    button1->setChecked(is);
}

void YbSettingPage::setButton2Checked(bool is)
{
    button2->setChecked(is);
}


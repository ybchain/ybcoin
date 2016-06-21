#include "ybencryptiondialog.h"
#include "ybmessagedialogtitle.h"
#include "ybpushbutton.h"

#include <QLabel>
#include <QLineEdit>
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>

YbEncryptionDialog::YbEncryptionDialog(QWidget *parent) :
    QWidget(parent)
{
    this->setFixedHeight(250);
    this->setFixedWidth(500);
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(255, 255, 255));
    this->setPalette(pa);
    setStyleSheet("QLineEdit{border: 2px groove rgb(211, 211, 211)} QPlainTextEdit{border: 2px groove rgb(211, 211, 211)}");

    QFont boldFont;
    boldFont.setBold(true);

    QString titleText(tr("加密钱包"));
    title = new YbMessageDialogTitle(titleText, this);

    buttonBar = new EncryptionButtonBar(this);

    encryptionText = new QLabel(this);
    encryptionText->setText(tr("输入口令："));
    encryptionText->setFont(boldFont);

    newEncryptionText = new QLabel(this);
    newEncryptionText->setText(tr("新口令："));
    newEncryptionText->setFont(boldFont);

    reNewEncryptionText = new QLabel(this);
    reNewEncryptionText->setText(tr("重复新口令："));
    reNewEncryptionText->setFont(boldFont);

    encryption = new QLineEdit(this);
    newEncryption = new QLineEdit(this);
    reNewEncryption = new QLineEdit(this);

    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addSpacing(20);
    hlayout1->addWidget(encryptionText);
    hlayout1->addWidget(encryption);
    hlayout1->addSpacing(20);

    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addSpacing(20);
    hlayout2->addWidget(newEncryptionText);
    hlayout2->addWidget(newEncryption);
    hlayout2->addSpacing(20);

    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addSpacing(20);
    hlayout3->addWidget(reNewEncryptionText);
    hlayout3->addWidget(reNewEncryption);
    hlayout3->addSpacing(20);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setMargin(0);
    mainLayout->addWidget(title);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addLayout(hlayout1);
    mainLayout->addLayout(hlayout2);
    mainLayout->addLayout(hlayout3);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
    mainLayout->addWidget(buttonBar);

    setLayout(mainLayout);
}


EncryptionButtonBar::EncryptionButtonBar(QWidget *parent)
{
    setAutoFillBackground(true);
    QPalette pa = palette();
    pa.setColor(QPalette::Background,QColor(224, 238, 238));
    this->setPalette(pa);

    setFixedHeight(60);
    setFixedWidth(500);

    closeButton = new YbPushButton(tr("关闭"), 60, 30, false, this);
    okButton = new YbPushButton(tr("完成"), 60, 30, true, this);
    connect(closeButton, SIGNAL(clicked()), this, SIGNAL(close()));
    connect(okButton, SIGNAL(clicked()), this, SIGNAL(ok()));

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(closeButton);
    mainLayout->addSpacerItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    mainLayout->addWidget(okButton);

    setLayout(mainLayout);
}

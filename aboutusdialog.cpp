#include "aboutusdialog.h"
#include "ui_aboutusdialog.h"
#include "QPainter"
#include "QTimer"
#include <QDesktopServices>
#include <QUrl>

AboutUsDialog::AboutUsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutUsDialog)
{
    ui->setupUi(this);

    m_curIndex = 0;//当前角码
    m_showText = "海贝TV提供技术支持";//显示的文字
    m_charWidth = fontMetrics().width("海");//每个字符的宽度

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(500);
}

AboutUsDialog::~AboutUsDialog()
{
    delete ui;
}

void AboutUsDialog::paintEvent(QPaintEvent *)
{
    m_curIndex++;
    if (m_curIndex*m_charWidth > width())
        m_curIndex = 0;
    QPainter painter(this);
    painter.drawText(0, 30, m_showText.mid(m_curIndex));
    painter.drawText(width() - m_charWidth*m_curIndex, 30, m_showText.left(m_curIndex));
}


void AboutUsDialog::on_joinUsURl_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));

//    ui->joinUsURl->setText(tr("<a href =" "www.baidu.com" ">google</a>"));
//    ui->joinUsURl->setOpenExternalLinks(true);
}

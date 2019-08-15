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

    QString aboutVer = QString("版本：%1").arg(MainWindow::instance()->oldversion);
    ui->label_3->setText(aboutVer);

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

void AboutUsDialog::mousePressEvent(QMouseEvent *event)
{
    int curTextLeft = width() - m_charWidth*m_curIndex;
    int curTextRight = width() - m_charWidth*m_curIndex + m_showText.length()*m_charWidth;
    if (event->x() >= curTextLeft && event->x() <= curTextRight && event->y() >= 20 && event->y() <= 30){
        QMovie *movie = new QMovie(":images/easterEgg.gif");
        ui->label_3->setScaledContents(true);//拉伸
        ui->label_3->setGeometry(0, 0, 628, 316);//图片位置
        ui->label_3->setMovie(movie);
        ui->textBrowser->setVisible(false);
        movie->start();
    }
}

void AboutUsDialog::on_joinUsURl_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl(link));

//    ui->joinUsURl->setText(tr("<a href =" "www.baidu.com" ">google</a>"));
//    ui->joinUsURl->setOpenExternalLinks(true);
}

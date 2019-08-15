#include "updatedialog.h"
#include "ui_updatedialog.h"
#include <QNetworkRequest>
#include "QNetworkAccessManager"
#include "QNetworkReply"
#include "qprocess.h"
#include <QThread>

UpdateDialog *UpdateDialog::m_pSelf = NULL;

UpdateDialog::UpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdateDialog)
{
    m_pSelf = this;
    ui->setupUi(this);
    this->setFixedSize(554, 320);
    //让进度条循环显示
    ui->progressBar->setMaximum(0);
    ui->progressBar->setMinimum(0);

    ui->ContentLabel1->setVisible(false);
    ui->ContentBody->setVisible(false);
    ui->ContentLabel2->setVisible(false);

    ui->OKButton->setVisible(false);
    ui->CancelButton->setVisible(false);
    ui->scrollArea->setVisible(false);

    ui->scrollArea->setBackgroundRole(QPalette::Light);
    ui->ContentBody->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    m_ftp = new FtpManager;
    m_ftp->setProgressBar(ui->progressBar);

    appName = QCoreApplication::applicationName();
    curExePathAndName = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());
    //获取exe所在的目录
    WindowDirPath = QDir::toNativeSeparators(QCoreApplication::applicationDirPath());
    LinuxDirPath = QCoreApplication::applicationDirPath();
    //获取当前进程所在工作目录
    //qDebug() << "current currentPath: " << QDir::currentPath();

    upgradBatCommand = WindowDirPath + "\\upgrad.bat";

    UpgradFileDetect();

}

UpdateDialog::~UpdateDialog()
{
    m_pSelf = NULL;
    delete ui;
}

void UpdateDialog::on_OKButton_clicked()
{
    ui->label->setVisible(true);
    ui->label->setText("下载中...");
    ui->progressBar->setVisible(true);
    ui->progressBar->setValue(0);

    ui->ContentLabel1->setVisible(false);
    ui->ContentBody->setVisible(false);
    ui->ContentLabel2->setVisible(false);
    ui->OKButton->setVisible(false);
    ui->CancelButton->setVisible(false);
    ui->scrollArea->setVisible(false);

    curPage = DOWNLOAD_PAGE;

    m_ftp->get("/" + ftpExeName + ".exe", LinuxDirPath + "/" + downloadExeName + "_bak.exe");

}

void UpdateDialog::on_CancelButton_clicked()
{
    m_pSelf->close();
}

void UpdateDialog::UpgradFileDetect()
{
    // 设置 FTP 相关信息
//    m_ftp->setHostPort("169.254.105.212", 21);
//    m_ftp->setUserInfo("admin", "123456");

    curPage = INFO_PAGE;

    m_ftp->get("/upgradInfo.txt", LinuxDirPath + "/upgradInfo.txt");
    connect(m_ftp, SIGNAL(sigfinish(bool)), this, SLOT(finish(bool)));

}

void UpdateDialog::finish(bool result)
{
    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
        QMessageBox msg(QMessageBox::Warning, "upgradInfo", "下载失败");
        msg.exec();
        this->close();
        return;
    }

    if(curPage == INFO_PAGE){
        QFile f(LinuxDirPath + "/upgradInfo.txt");
        if(!f.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate))
        {
            qDebug() << "Open failed.";
            QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
            return;
        }

        QTextStream txtInput(&f);
        txtInput.setCodec("utf-8");
        QString lineStr;
        version = 0.0;
        QString upgradInfoStr;
        int forceUpgradFlag = 0;
        while(!txtInput.atEnd())
        {
            lineStr = txtInput.readLine();
            if(lineStr.startsWith("forceUpgrad", Qt::CaseInsensitive)){
                forceUpgradFlag = lineStr.mid(strlen("forceUpgrad:")).toInt();;
            }

            if(lineStr.startsWith("version", Qt::CaseSensitive)){
                version = lineStr.mid(8).toFloat();
            }

            if(lineStr.startsWith("ftpExeName", Qt::CaseInsensitive)){
                ftpExeName = lineStr.mid(strlen("ftpExeName:"));
            }

            if(lineStr.startsWith("downloadExeName", Qt::CaseInsensitive)){
                downloadExeName = lineStr.mid(strlen("downloadExeName:"));
            }

            if(lineStr.startsWith("info", Qt::CaseInsensitive)){
                upgradInfoStr = txtInput.readAll();
            }

//            qDebug() << lineStr;
//            qDebug() << txtInput.pos();
        }
        f.close();
        //删除文件
        QFile::remove(LinuxDirPath + "/upgradInfo.txt");

        //手动检测 0、1:检测到新版本并提示升级 2：检测到新版本不提示直接强制升级 3：忽略升级请求
        if((version <= MainWindow::instance()->oldversion) || forceUpgradFlag == 3)
        {
            QMessageBox msg(QMessageBox::Information, "upgradInfo", "已经是最新的版本了");
            msg.exec();
            this->close();
            return;
        }

        ui->label->setVisible(false);
        ui->progressBar->setVisible(false);

        ui->ContentLabel1->setVisible(true);
        ui->ContentLabel1->setText(QString("有新版本（V%1）可用。").arg(version));

        ui->ContentBody->setVisible(true);
        ui->ContentBody->setText(upgradInfoStr);
        ui->ContentLabel2->setVisible(true);

        ui->OKButton->setVisible(true);
        ui->CancelButton->setVisible(true);
        ui->scrollArea->setVisible(true);

        if(forceUpgradFlag == 2)
            on_OKButton_clicked();
    }
    else if(curPage == DOWNLOAD_PAGE)
    {
        ui->label->setText("下载完成，准备升级！");
        //更新步骤,制作辅助升级脚本->运行 ->关闭自己

        QFile file(upgradBatCommand);
        if(!file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))//有中文要用Text
        {
            qDebug() << "Open failed.";
            return;
        }

        //QString battmp = "start /d \"D:\\Desktop\\QT\\build-factory_mask-Desktop_Qt_5_10_1_MinGW_32bit-Debug\\debug\" 车间配置项掩码计算工具.exe";
        QString batStr = QString("TIMEOUT /T 2 \n\
del %3\\%1.exe\n\
rename %2_bak.exe %2.exe\n\
del %2_bak.exe\n\
start /d \"%3\" %2.exe\n@exit")
                .arg(appName)
                .arg(downloadExeName)
                .arg(WindowDirPath);

        QTextStream in(&file);
        in << batStr;
        file.close();

        QFile Assistantfile(WindowDirPath + "\\updateAssistant.bat");
        if(!Assistantfile.open(QIODevice::ReadWrite | QIODevice::Truncate))//Truncate清空内容再写
        {
            qDebug() << "Open failed.";
            return;
        }

        QString updateAssistantStr = QString("start /d \"%1\" upgrad.bat\n")
                .arg(WindowDirPath);

        QTextStream AssistantIn(&Assistantfile);
        AssistantIn << updateAssistantStr;
        Assistantfile.close();

//        QThread *workerThread = new QThread;
//        connect(workerThread, &QThread::started, this, &UpdateDialog::ThreadFuc);
//        workerThread->start();

        QProcess p(NULL);
        p.start(WindowDirPath + "\\updateAssistant.bat");
        p.waitForFinished();
        exit(1);
    }
}

void UpdateDialog::ThreadFuc(){
//    qDebug() << "ThreadFuc";
//    system("D:/Desktop/QT/build-factory_mask-Desktop_Qt_5_10_1_MinGW_32bit-Release/release/upgrad.bat");
}

void UpdateDialog::replyFinished(QNetworkReply *reply){
    //获取响应的信息，状态码为200表示正常

    QVariant status_code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    QByteArray bytes = reply->readAll();  //获取字节
    QString result(bytes);  //转化为字符串
    qDebug()<<result;

    //无错误返回
    if(reply->error() == QNetworkReply::NoError)
    {
//        download_Btn->setText("success");
//        QByteArray bytes = reply->readAll();  //获取字节
//        QString result(bytes);  //转化为字符串
        qDebug() << "  NoError   " ;
        qDebug()<<result;
    }
    else
    {
        qDebug() << "  failed   " ;
        //处理错误
        // download_Btn->setText("failed");
    }

    //reply->deleteLater();//要删除reply，但是不能在repyfinished里直接delete，要调用deletelater;

}

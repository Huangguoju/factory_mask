#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutusdialog.h"
#include <QMessageBox>
#include "qstring.h"
#include "updatedialog.h"

MainWindow *MainWindow::m_pSelf = NULL;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_pSelf = this;

    setFixedSize(892, 562);

    supportFactoryConfigMask = 0;
    ui->factoryConfigMaskEdit->clear();
    ui->curDateTimeLabel->clear();
    ui->movieLabel->clear();
	movie = NULL;

    UserInfo = new useUserInfo;
    qDebug() << UserInfo->ip() ; //有全局变量，不能删

    //当前软件版本
    oldversion = 1.2;

    //移除升级后的辅助升级bat脚本
    QFile::remove(QCoreApplication::applicationDirPath() + "/upgrad.bat");
    QFile::remove(QCoreApplication::applicationDirPath() + "/updateAssistant.bat");

    //首次启动检测是否有新版本
    m_detect = new FtpManager;
    m_detect->get("/upgradInfo.txt", QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
    connect(m_detect, SIGNAL(sigfinish(bool)), this, SLOT(finish(bool)));

    m_famousRemark = new FtpManager;
    m_famousRemark->get("/famousRemarkOnline.txt", QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
    connect(m_famousRemark, SIGNAL(sigfinish(bool)), this, SLOT(famousRemarkfinish(bool)));

    QTimer *Datetimer = new QTimer(this);
    connect(Datetimer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    Datetimer->start(1000);


    createActions();
    createMenus();
    createStatusBar();

    m_checkVec << ui->checkBox0 << ui->checkBox1 << ui->checkBox2 << ui->checkBox3 << ui->checkBox4 \
               << ui->checkBox5 << ui->checkBox6 << ui->checkBox7 << ui->checkBox8 << ui->checkBox9 \
               << ui->checkBox10 << ui->checkBox11 << ui->checkBox12 << ui->checkBox13 << ui->checkBox14 \
               << ui->checkBox15 << ui->checkBox16 << ui->checkBox17 << ui->checkBox18 << ui->checkBox19 \
               << ui->checkBox20 << ui->checkBox21 << ui->checkBox22 << ui->checkBox23 << ui->checkBox24 \
               << ui->checkBox25 << ui->checkBox26 << ui->checkBox27 << ui->checkBox28 << ui->checkBox29 \
               << ui->checkBox30 << ui->checkBox31 << ui->checkBox32;

    m_configNameVec << ui->label0 << ui->label1 << ui->label2 << ui->label3 << ui->label4 << ui->label5 \
                    << ui->label6 << ui->label7 << ui->label8 << ui->label9 << ui->label10 << ui->label11 \
                    << ui->label12 << ui->label13 << ui->label14 << ui->label15 << ui->label16 << ui->label17 \
                    << ui->label18 << ui->label19 << ui->label20 << ui->label21 << ui->label22 << ui->label23 \
                    << ui->label24 << ui->label25 << ui->label26 << ui->label27 << ui->label28 << ui->label29 \
                    << ui->label30 << ui->label31 << ui->label32;

    m_chooseLabelVec
        << ui->chooseLabel_1 << ui->chooseLabel_2 << ui->chooseLabel_3 << ui->chooseLabel_4 << ui->chooseLabel_5 \
        << ui->chooseLabel_6 << ui->chooseLabel_7 << ui->chooseLabel_8 << ui->chooseLabel_9 << ui->chooseLabel_10 \
        << ui->chooseLabel_11 << ui->chooseLabel_12 << ui->chooseLabel_13 << ui->chooseLabel_14 << ui->chooseLabel_15 \
        << ui->chooseLabel_16 << ui->chooseLabel_17 << ui->chooseLabel_18 << ui->chooseLabel_19 << ui->chooseLabel_20 \
        << ui->chooseLabel_21 << ui->chooseLabel_22 << ui->chooseLabel_23 << ui->chooseLabel_24 << ui->chooseLabel_25 \
        << ui->chooseLabel_26 << ui->chooseLabel_27 << ui->chooseLabel_28 << ui->chooseLabel_29 << ui->chooseLabel_30 \
        << ui->chooseLabel_31 << ui->chooseLabel_32 << ui->chooseLabel_33;

    for (int i = 0; i < m_checkVec.size(); i++){
        connect(m_checkVec[i], SIGNAL(clicked(bool)), this, SLOT(checkChanged()));
    }
    m_checkVec[0]->setChecked(true); //OrderDeviceID
    m_checkVec[0]->setEnabled(false);
    m_checkVec[1]->setChecked(true); //TranscationID
    m_checkVec[1]->setEnabled(false);

    for (int i = 0; i < m_chooseLabelVec.size(); i++){
        m_chooseLabelVec[i]->clear();
    }
}


MainWindow::~MainWindow()
{
    if (movie){
        movie->stop();
        disconnect(movie, SIGNAL(frameChanged(int)), this, SLOT(movieStatus(int)));
        movie->~QMovie();//要销毁qmovie才能删除图片
        ui->movieLabel->clear();
        ui->movieLabel->hide();
        QFile::remove(QCoreApplication::applicationDirPath() + "/startDemo.gif");
    }

    QFile::remove(QCoreApplication::applicationDirPath() + "/upgrad.bat");
    QFile::remove(QCoreApplication::applicationDirPath() + "/updateAssistant.bat");
    QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
    QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
    QFile::remove(QCoreApplication::applicationDirPath() + + "/" + UserInfo->ip() + "_" + UserInfo->mac() + ".txt");
    delete ui;
}


void MainWindow::timerUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->curDateTimeLabel->setText(str);
}

void MainWindow::movieStatus(int frameNumber)
{
    if (frameNumber == frameCount - 1){
        movie->stop();
        disconnect(movie, SIGNAL(frameChanged(int)), this, SLOT(movieStatus(int)));
        movie->~QMovie();//要销毁qmovie才能删除图片
        movie = NULL;
        ui->movieLabel->clear();
        ui->movieLabel->hide();
        QFile::remove(QCoreApplication::applicationDirPath() + "/startDemo.gif");
    }
}

void MainWindow::startDemofinish(bool result)
{
    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/startDemo.gif");
        return;
    }

    movie = new QMovie(QCoreApplication::applicationDirPath() + "/startDemo.gif");
    ui->movieLabel->setScaledContents(true);//拉伸
    ui->movieLabel->setGeometry(0, 0, 892, 491);//图片位置
    ui->movieLabel->setMovie(movie);
    frameCount = movie->frameCount();
    movie->start();
    connect(movie, SIGNAL(frameChanged(int)), this, SLOT(movieStatus(int)));
}

void MainWindow::uploadfinish(bool result)
{
    qDebug() << result ;
    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + + "/" + UserInfo->ip() + "_" + UserInfo->mac() + ".txt");
        return;
    }
    QFile::remove(QCoreApplication::applicationDirPath() + + "/" + UserInfo->ip() + "_" + UserInfo->mac() + ".txt");
}

void MainWindow::finish(bool result)
{
    /*
    forceUpgrad:0 //自动检测 0:检测到新版本不提示升级  1:检测到新版本并提示升级 2：检测到新版本不提示直接强制升级 3：忽略升级请求
    version:1.1
    showStartDemo:true
    connectUsUrl:http://www.baidu.com
    ftpExeName:33
    downloadExeName:车间配置项掩码计算工具
    info:
    增加一些功能，改善一些BUG。
    车间配置项掩码计算工具
    //无限升级：upgradInfo.txt里面的版本大于更新后的版本，且forceUpgrad=2，无限循环升级。
     */

    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
        QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
        return;
    }

    QFile f(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Open failed.";
        QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
        return;
    }

    QTextStream txtInput(&f);
    txtInput.setCodec("utf-8");
    QString lineStr;
    float newVersion = 0.0;
    int forceUpgradFlag = 0;
    m_connectUsUrl = "";
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        if(lineStr.startsWith("forceUpgrad", Qt::CaseInsensitive)){
            forceUpgradFlag = lineStr.mid(strlen("forceUpgrad:")).toInt();;
        }

        if(lineStr.startsWith("version", Qt::CaseSensitive)){
            newVersion = lineStr.mid(strlen("version:")).toFloat();
        }

        if(lineStr.startsWith("connectUsUrl", Qt::CaseSensitive)){
            m_connectUsUrl = lineStr.mid(strlen("connectUsUrl:"));
        }

        if(lineStr.startsWith("showStartDemo", Qt::CaseSensitive)){
            if (lineStr.mid(strlen("showStartDemo:")).toInt())
            {
                m_startDemo = new FtpManager;
                m_startDemo->get("/startDemo.gif", QCoreApplication::applicationDirPath() + "/startDemo.gif");
                connect(m_startDemo, SIGNAL(sigfinish(bool)), this, SLOT(startDemofinish(bool)));
            }
        }
    }
    f.close();
    //删除文件
    QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");

    //上传记录用户登录信息
    QString uploadFileName = QCoreApplication::applicationDirPath() + "/" + UserInfo->ip() + "_" + UserInfo->mac() + ".txt";
    QString fileName = "factory_useUserInfo/" + UserInfo->ip() + "_" + UserInfo->mac() + ".txt";

    QFile upload(uploadFileName);
    if(!upload.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Open failed.";
        QFile::remove(uploadFileName);
        return;
    }

    QTextStream txtUpload(&upload);
    txtUpload << UserInfo->localmachineName() + "\n";
    txtUpload << UserInfo->osVersion() + "\n";
    txtUpload << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss dddd") + "\n";
    upload.close();

    m_upload = new FtpManager;
    m_upload->put(uploadFileName, fileName);
    connect(m_upload, SIGNAL(sigfinish(bool)), this, SLOT(uploadfinish(bool)));

    //自动检测 0:检测到新版本不提示升级  1:检测到新版本并提示升级 2：检测到新版本不提示直接强制升级 3：忽略升级请求
    if(newVersion > oldversion && forceUpgradFlag != 3)
    {
        if(forceUpgradFlag == 1 || forceUpgradFlag == 2)
            update();
    }
}

void MainWindow::checkChanged()
{
    supportFactoryConfigMask = 0;
    for (int i = 0; i < m_chooseLabelVec.size(); i++){
        m_chooseLabelVec[i]->clear();
    }
    int chooseNode = 0;
    m_checkVec[0]->setChecked(true); //OrderDeviceID
    m_checkVec[0]->setEnabled(false);
    m_checkVec[1]->setChecked(true); //TranscationID
    m_checkVec[1]->setEnabled(false);

    for (int i = 0; i < m_checkVec.size(); i++){
        if (m_checkVec[i]->isChecked()){
            m_chooseLabelVec[chooseNode++]->setText(m_configNameVec[i]->text());
            SET_BIT_LL(&supportFactoryConfigMask, i);
        }
        else
            CLR_BIT_LL(&supportFactoryConfigMask, i);
    }
    char ConfigMask[16];
    sprintf(ConfigMask, "%lld", supportFactoryConfigMask);
    ui->factoryConfigMaskEdit->setText(QString(ConfigMask));
}



void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("文件");
    fileMenu->addSeparator(); //分隔符
    fileMenu->addAction(exitAct);
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu("帮助");
    //helpMenu->addAction(helpAct);
    helpMenu->addAction(developAct);
    helpMenu->addAction(updateAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutUsAct);
}

void MainWindow::createActions()
{
    exitAct = new QAction(QIcon(":images/exit.ico"), "退出", this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip("Exit the application");
    connect(exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    //helpAct = new QAction(QIcon(":images/help.ico"), "帮助", this);
    //helpAct->setShortcuts(QKeySequence::HelpContents); //添加快捷键
    //connect(helpAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    aboutUsAct = new QAction(QIcon(":images/about.ico"), "关于", this);
    aboutUsAct->setShortcuts(QKeySequence::WhatsThis); //添加快捷键
    connect(aboutUsAct, SIGNAL(triggered()), this, SLOT(about()));

    developAct = new QAction("develop", this);
    connect(developAct, SIGNAL(triggered()), this, SLOT(develop()));

    updateAct = new QAction("检查更新...", this);
    connect(updateAct, SIGNAL(triggered()), this, SLOT(update()));
}

void MainWindow::develop()
{
    view = new QQuickView;
    view->create();
    if (rand() % 3){
        view->setSource(QUrl("qrc:/frame.qml"));
        view->setTitle("粒子漩涡");
    }
    else
    {
        view->setSource(QUrl("qrc:/samegame.qml"));
        view->setTitle("隐藏模式");
    }
    view->setIcon(QIcon(":images/exit.ico"));

    view->setResizeMode(QQuickView::SizeRootObjectToView);
    view->setMaximumSize(QSize(1820, 1080));
    view->setMinimumSize(QSize(300, 150));
    view->setX(this->x() - 100);
    view->setY(this->y() - 100);
    view->show();
}

void MainWindow::about()
{
    AboutUsDialog *aboutUsdia = new AboutUsDialog(this);
    aboutUsdia->exec();
}

void MainWindow::update()
{
    UpdateDialog *updateDia = new UpdateDialog(this);
    //无标题
    //updateDia->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);
    updateDia->exec();
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage("已就绪");

    famousRemark();
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateFamous()));
    timer->start(100000);
}

void MainWindow::updateFamous()
{
    statusBar()->showMessage(famousRemarkVector.at(rand() % famousRemarkVector.size()));
}


void MainWindow::famousRemarkfinish(bool result)
{
    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
        return;
    }

    QFile f(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Open failed.";
        QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
        return;
    }

    QTextStream txtInput(&f);
    txtInput.setCodec("utf-8");
    QString lineStr;
    famousRemarkVector.clear();
    timer->stop();
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        if(lineStr.startsWith("interval", Qt::CaseSensitive)){
            //1秒：1000
            timer->setInterval(lineStr.mid(strlen("interval:")).toInt());
        }else{
            famousRemarkVector << lineStr;
        }
    }
    f.close();
    //删除文件
    QFile::remove(QCoreApplication::applicationDirPath() + "/famousRemarkOnline.txt");
    timer->start();
}

void MainWindow::famousRemark()
{
    srand(time(NULL));
    famousRemarkVector  << "";
}

void MainWindow::on_allButton_clicked()
{
    for (int i = 0; i < m_checkVec.size(); i++){
        m_checkVec[i]->setChecked(true);
    }
    checkChanged();
}

void MainWindow::on_clearButton_clicked()
{
    for (int i = 0; i < m_checkVec.size(); i++){
        m_checkVec[i]->setChecked(false);
    }
    checkChanged();
}

void MainWindow::on_factoryConfigMaskEdit_textEdited(const QString &arg1)
{
    supportFactoryConfigMask = arg1.toULongLong();
    for (int i = 0; i < m_chooseLabelVec.size(); i++){
        m_chooseLabelVec[i]->clear();
    }
    int chooseNode = 0;

    m_checkVec[0]->setChecked(true); //OrderDeviceID
    m_checkVec[0]->setEnabled(false);
    m_checkVec[1]->setChecked(true); //TranscationID
    m_checkVec[1]->setEnabled(false);

    for (int i = 0; i < m_checkVec.size(); i++){
        if (CHK_BIT_LL(&supportFactoryConfigMask, i)){
            m_checkVec[i]->setChecked(true);
            m_chooseLabelVec[chooseNode++]->setText(m_configNameVec[i]->text());
        }
        else
            m_checkVec[i]->setChecked(false);
    }
}



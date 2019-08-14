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

    setFixedSize(892, 491);

    createActions();
    createMenus();
    createStatusBar();

    m_checkVec << ui->checkBox0 << ui->checkBox1 << ui->checkBox2 << ui->checkBox3 << ui->checkBox4 \
               << ui->checkBox5 << ui->checkBox6 << ui->checkBox7 << ui->checkBox8 << ui->checkBox9 \
               << ui->checkBox10 << ui->checkBox11 << ui->checkBox12 << ui->checkBox13 << ui->checkBox14 \
               << ui->checkBox15 << ui->checkBox16 << ui->checkBox17 << ui->checkBox18 << ui->checkBox19 \
               << ui->checkBox20 << ui->checkBox21 << ui->checkBox22 << ui->checkBox23 << ui->checkBox24 \
               << ui->checkBox25 << ui->checkBox26 << ui->checkBox27 << ui->checkBox28 << ui->checkBox29;

    m_configNameVec << ui->label0 << ui->label1 << ui->label2 << ui->label3 << ui->label4 << ui->label5 \
                    << ui->label6 << ui->label7 << ui->label8 << ui->label9 << ui->label10 << ui->label11 \
                    << ui->label12 << ui->label13 << ui->label14 << ui->label15 << ui->label16 << ui->label17 \
                    << ui->label18 << ui->label19 << ui->label20 << ui->label21 << ui->label22 << ui->label23 \
                    << ui->label24 << ui->label25 << ui->label26 << ui->label27 << ui->label28 << ui->label29;

    m_chooseLabelVec
        << ui->chooseLabel_1 << ui->chooseLabel_2 << ui->chooseLabel_3 << ui->chooseLabel_4 << ui->chooseLabel_5 \
        << ui->chooseLabel_6 << ui->chooseLabel_7 << ui->chooseLabel_8 << ui->chooseLabel_9 << ui->chooseLabel_10 \
        << ui->chooseLabel_11 << ui->chooseLabel_12 << ui->chooseLabel_13 << ui->chooseLabel_14 << ui->chooseLabel_15 \
        << ui->chooseLabel_16 << ui->chooseLabel_17 << ui->chooseLabel_18 << ui->chooseLabel_19 << ui->chooseLabel_20 \
        << ui->chooseLabel_21 << ui->chooseLabel_22 << ui->chooseLabel_23 << ui->chooseLabel_24 << ui->chooseLabel_25 \
        << ui->chooseLabel_26 << ui->chooseLabel_27 << ui->chooseLabel_28 << ui->chooseLabel_29 << ui->chooseLabel_30;

    for (int i = 0; i < m_checkVec.size(); i++){
        connect(m_checkVec[i], SIGNAL(clicked(bool)), this, SLOT(checkChanged()));
    }

    for (int i = 0; i < m_chooseLabelVec.size(); i++){
        m_chooseLabelVec[i]->clear();
    }

    supportFactoryConfigMask = 0;
    ui->factoryConfigMaskEdit->clear();

    oldversion = 1.1;

    m_detect = new FtpManager;
//    m_detect->setHostPort("169.254.105.212", 21);
//    m_detect->setUserInfo("admin", "123456");

    QFile::remove(QCoreApplication::applicationDirPath() + "/upgrad.bat");
    QFile::remove(QCoreApplication::applicationDirPath() + "/updateAssistant.bat");

    m_detect->get("/upgradInfo.txt", QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
    connect(m_detect, SIGNAL(sigfinish(bool)), this, SLOT(finish(bool)));

}

void MainWindow::finish(bool result)
{
    /*
    forceUpgrad:0 //自动检测 0:检测到新版本不提示升级  1:检测到新版本并提示升级 2：检测到新版本不提示直接强制升级 3：忽略升级请求
    version:1.1
    ftpExeName:33
    downloadExeName:车间配置项掩码计算工具
    info:
    增加一些功能，改善一些BUG。
    车间配置项掩码计算工具

     */


    if(!result){
        QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");
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
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        if(lineStr.startsWith("forceUpgrad", Qt::CaseInsensitive)){
            forceUpgradFlag = lineStr.mid(strlen("forceUpgrad:")).toInt();;
        }

        if(lineStr.startsWith("version", Qt::CaseSensitive)){
            newVersion = lineStr.mid(strlen("version:")).toFloat();
        }
    }
    f.close();
    //删除文件
    QFile::remove(QCoreApplication::applicationDirPath() + "/upgradInfo.txt");

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


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu("文件");
    fileMenu->addSeparator(); //分隔符
    fileMenu->addAction(exitAct);
    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu("帮助");
    helpMenu->addAction(helpAct);
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

    helpAct = new QAction(QIcon(":images/help.ico"), "帮助", this);
    helpAct->setShortcuts(QKeySequence::HelpContents); //添加快捷键
    connect(helpAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

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
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateFamous()));
    timer->start(100000);

}

void MainWindow::updateFamous()
{
    statusBar()->showMessage(famousRemarkVector.at(rand() % famousRemarkVector.size()));
}

void MainWindow::famousRemark()
{
    srand(time(NULL));
    famousRemarkVector  << "泰戈尔曾经说过:要是童年的日子能重新回来，那我一定不再浪费光阴，我要把每分每秒都用来读书。"
        << "三年二班的周杰伦,请马上到教导处来"
        << "真理惟一可靠的标准就是永远自相符合。"
        << "时间是一切财富中最宝贵的财富。"
        << "世界上一成不变的东西，只有“任何事物都是在不断变化的”这条真理。"
        << "生活有度，人生添寿。 —— 书摘"
        << "任何事物都无法抗拒吞食一切的时间。"
        << "青春是没有经验和任性的。"
        << "思想以自己的言语喂养它自己，而成长起来。"
        << "泰戈尔曾经说过：沉默是一种美德，但在喜欢的人面前沉默那便是懦弱，我已经懦弱了三年，"
        "今天我要勇敢。黄晶晶同学，你愿意和我携手告别高中时代吗！ - 青春派"
        << "过放荡不羁的生活，容易得像顺水推舟，但是要结识良朋益友，却难如登天。"
        << "这世界要是没有爱情，它在我们心中还会有什么意义！这就如一盏没有亮光的走马灯。"
        << "人生并不像火车要通过每个站似的经过每一个生活阶段。人生总是直向前行走，从不留下什么。"
        << "有所作为是生活的最高境界。 —— 恩格斯"
        << "我们活着不能与草木同腐，不能醉生梦死，枉度人生，要有所做为。"
        << "浪费别人的时间等于是谋财害命，浪费自己的时间等于是慢性自杀。 —— 列宁"
        << "现实是此岸，理想是彼岸，中间隔着湍急的河流，行动则是架在河上的桥梁。 —— 克雷洛夫"
        << "读书谓已多，抚事知不足。 —— 王安石"
        << "青年长于创造而短于思考，长于猛干而短于讨论，长于革新而短于持重。 —— 培根"
        << "理想对我来说，具有一种非凡的魅力。我的理想总是充满着生活和泥土气息。我从来都不去空想那些不可能实现的事情。 —— 奥斯特洛夫斯基"
        << "青春啊，难道你始终囚禁在狭小圈子里？你得撕破老年的蛊惑人心的网。";
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

    for (int i = 0; i < m_checkVec.size(); i++){
        if (CHK_BIT_LL(&supportFactoryConfigMask, i)){
            m_checkVec[i]->setChecked(true);
            m_chooseLabelVec[chooseNode++]->setText(m_configNameVec[i]->text());
        }
        else
            m_checkVec[i]->setChecked(false);
    }
}

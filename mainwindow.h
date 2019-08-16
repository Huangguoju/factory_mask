#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QDragEnterEvent>
#include <QDebug>
#include <QGraphicsView>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "QVector"

#include "QTimer"
#include "time.h"

#include <QtQuick/QQuickView>
#include <QNetworkAccessManager>
#include "ftpmanager.h"
#include <QMovie>
#include <QNetworkRequest>
#include "QNetworkReply"
#include "qprocess.h"
#include <QThread>

/* 计算数组下标 */
#define ARRAY_INDEX(index, BitNum) ((index) / (BitNum))
/* 计算BIT位 */
#define BIT_INDEX(index, BitNum) ((index) % (BitNum))


#define CLR_BIT_NUM(addr, index, BitNum, type) \
        *(addr + ARRAY_INDEX(index, BitNum)) &= \
            (~(((type)0x01) << BIT_INDEX(index, BitNum)))

#define SET_BIT_NUM(addr, index, BitNum, type) \
        *(addr + ARRAY_INDEX(index, BitNum)) |= \
            (((type)0x01) << BIT_INDEX(index, BitNum))

#define CHK_BIT_NUM(addr, index, BitNum, type) \
        (*(addr + ARRAY_INDEX(index, BitNum)) >> BIT_INDEX(index, BitNum)) & ((type)0x01)


#define CLR_BIT_BYTE(addr, index) (CLR_BIT_NUM(addr, index, 8, unsigned char))
#define SET_BIT_BYTE(addr, index) (SET_BIT_NUM(addr, index, 8, unsigned char))
#define CHK_BIT_BYTE(addr, index) (CHK_BIT_NUM(addr, index, 8, unsigned char))

#define CLR_BIT_SHORT(addr, index) (CLR_BIT_NUM(addr, index, 16, unsigned short))
#define SET_BIT_SHORT(addr, index) (SET_BIT_NUM(addr, index, 16, unsigned short))
#define CHK_BIT_SHORT(addr, index) (CHK_BIT_NUM(addr, index, 16, unsigned short))

#define CLR_BIT_LL(addr, index) (CLR_BIT_NUM(addr, index, 64, unsigned long long))
#define SET_BIT_LL(addr, index) (SET_BIT_NUM(addr, index, 64, unsigned long long))
#define CHK_BIT_LL(addr, index) (CHK_BIT_NUM(addr, index, 64, unsigned long long))


#define CLR_BIT_LONG(addr, index) (CLR_BIT_NUM(addr, index, 32, unsigned long))
#define SET_BIT_LONG(addr, index) (SET_BIT_NUM(addr, index, 32, unsigned long))
#define CHK_BIT_LONG(addr, index) (CHK_BIT_NUM(addr, index, 32, unsigned long))

#define CLR_BIT_UNINT(addr, index) (CLR_BIT_NUM(addr, index, 32, unsigned int))
#define SET_BIT_UNINT(addr, index) (SET_BIT_NUM(addr, index, 32, unsigned int))
#define CHK_BIT_UNINT(addr, index) (CHK_BIT_NUM(addr, index, 32, unsigned int))

#define CLR_BIT_SLL(addr, index) (CLR_BIT_NUM(addr, index, 64, signed long long))
#define SET_BIT_SLL(addr, index) (SET_BIT_NUM(addr, index, 64, signed long long))
#define CHK_BIT_SLL(addr, index) (CHK_BIT_NUM(addr, index, 64, signed long long))


#define CLR_BIT(addr, index) (CLR_BIT_BYTE((unsigned char *)addr, index))
#define SET_BIT(addr, index) (SET_BIT_BYTE((unsigned char *)addr, index))
#define CHK_BIT(addr, index) (CHK_BIT_BYTE((unsigned char *)addr, index))

namespace Ui {
class MainWindow;
}

#define UNUSED(x) (void)x;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    static MainWindow *instance() {return m_pSelf;}
    ~MainWindow();

private slots:
    void createActions();
    void createMenus();
    void createStatusBar();
    void famousRemark();
    void updateFamous();
    void checkChanged();

    void about();
    void develop();
    void update();
    void timerUpdate();
    void movieStatus(int frameNumber);

    void finish(bool);
    void famousRemarkfinish(bool);
    void startDemofinish(bool);

    void on_allButton_clicked();
    void on_clearButton_clicked();
    void on_factoryConfigMaskEdit_textEdited(const QString &arg1);
public:
    float oldversion;
    QString m_connectUsUrl;
private:
    Ui::MainWindow *ui;

    QVector<QString> famousRemarkVector;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *exitAct;
    //QAction *helpAct;
    QAction *aboutUsAct;
    QAction *developAct;
    QAction *updateAct;

    QVector<QCheckBox *> m_checkVec;
    QVector<QLabel *> m_configNameVec;
    QVector<QLabel *> m_chooseLabelVec;
    unsigned long long supportFactoryConfigMask;

    FtpManager *m_detect;
    FtpManager *m_famousRemark;
    FtpManager *m_startDemo;
    QTimer *timer;
    QMovie *movie;
    int frameCount;

    QQuickView *view;
    static MainWindow *m_pSelf;
};

#endif // MAINWINDOW_H

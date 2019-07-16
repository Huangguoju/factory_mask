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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void createActions();
    void createMenus();
    void createStatusBar();
    void famousRemark();
    void updateFamous();
    void checkChanged();

    void about();
    void on_allButton_clicked();

    void on_clearButton_clicked();

private:
    Ui::MainWindow *ui;

    QVector<QString> famousRemarkVector;

    QMenu *fileMenu;
    QMenu *helpMenu;

    QAction *exitAct;
    QAction *helpAct;
    QAction *aboutUsAct;

    QVector<QCheckBox *> m_checkVec;
    QVector<QLabel *> m_configNameVec;
    QVector<QLabel *> m_chooseLabelVec;
    unsigned long long supportFactoryConfigMask;

};

#endif // MAINWINDOW_H
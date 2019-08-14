#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "ftpmanager.h"

enum pageEnum{
    INFO_PAGE = 0,
    DOWNLOAD_PAGE
};

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit UpdateDialog(QWidget *parent = 0);
    static UpdateDialog *instance() {return m_pSelf;}
    ~UpdateDialog();

private slots:
    void replyFinished(QNetworkReply *reply);
    void finish(bool);
    void on_OKButton_clicked();

    void on_CancelButton_clicked();
    void ThreadFuc();

private:

    void UpgradFileDetect();

    QNetworkAccessManager *accessManager;
    QNetworkRequest *request;

    FtpManager *m_ftp;
    float version;
    QString ftpExeName; //不能是中文
    QString downloadExeName;
    pageEnum curPage;
    QString appName;
    QString curExePathAndName;
    QString WindowDirPath;
    QString LinuxDirPath;
    QString upgradBatCommand;

    Ui::UpdateDialog *ui;
    static UpdateDialog *m_pSelf;
};

#endif // UPDATEDIALOG_H

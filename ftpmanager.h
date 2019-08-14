#ifndef FTPMANAGER_H
#define FTPMANAGER_H

#include <QMainWindow>
#include <QObject>
#include <QUrl>
#include <QFile>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "qprogressbar.h"

class FtpManager : public QObject
{
    Q_OBJECT
public:
    explicit FtpManager(QObject *parent = nullptr);

    // 设置地址和端口
    void setHostPort(const QString &host, int port = 21);
    // 设置登录 FTP 服务器的用户名和密码
    void setUserInfo(const QString &userName, const QString &password);
    // 上传文件
    void put(const QString &fileName, const QString &path);
    // 下载文件
    void get(const QString &path, const QString &fileName);

    void setProgressBar(QProgressBar *progressBar) {m_pDownloadBar = progressBar;}
signals:
    void sigfinish(bool);
private slots:
    void ftperror(QNetworkReply::NetworkError);
    // 上传进度
    void ftpuploadProgress(qint64 bytesSent, qint64 bytesTotal);
    // 下载进度
    void ftpdownloadProgress(qint64 bytesReceived, qint64 bytesTotal);

    // 下载过程中写文件
    void finished();
private:
    QUrl m_pUrl;
    QFile m_file;
    QNetworkAccessManager m_manager;
    QProgressBar *m_pDownloadBar;
};

#endif // FTPMANAGER_H

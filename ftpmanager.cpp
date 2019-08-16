#include "ftpmanager.h"
#include <QFileInfo>

FtpManager::FtpManager(QObject *parent) : QObject(parent)
{
    // 设置协议
    m_pUrl.setScheme("ftp");
    m_pDownloadBar = NULL;
    setHostPort("172.18.6.148", 21);
    setUserInfo("admin", "123456");
}

// 设置地址和端口
void FtpManager::setHostPort(const QString &host, int port)
{
    m_pUrl.setHost(host);
    m_pUrl.setPort(port);
}

// 设置登录 FTP 服务器的用户名和密码
void FtpManager::setUserInfo(const QString &userName, const QString &password)
{
    m_pUrl.setUserName(userName);
    m_pUrl.setPassword(password);
}

// 上传文件
void FtpManager::put(const QString &fileName, const QString &path)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();

    m_pUrl.setPath(path);
    QNetworkReply *pReply = m_manager.put(QNetworkRequest(m_pUrl), data);

    connect(pReply, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(ftpuploadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(ftperror(QNetworkReply::NetworkError)));
}

// 下载文件
//path 不能是exe结尾的，会导致获取失败，可以去掉exe结尾传输。fileName可以是exe
void FtpManager::get(const QString &path, const QString &fileName)
{
    QFileInfo info;
    info.setFile(fileName);

    m_file.setFileName(fileName);
    m_file.open(QIODevice::WriteOnly);

    m_pUrl.setPath(path);

    QNetworkRequest request(m_pUrl);
    pReply = m_manager.get(request);

    pTimeout = new QReplyTimeout(pReply, 1000);
    // 网络超时进一步处理,如果没有发射finished()信号就超时，即使正在下载。
//    connect(pTimeout, &QReplyTimeout::timeout, [=]() {
//        qDebug() << "Connect Network Timeout";
//    });

    connect(pReply, SIGNAL(finished()), this, SLOT(finished()));
    connect(pReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(ftpdownloadProgress(qint64, qint64)));
    connect(pReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(ftperror(QNetworkReply::NetworkError)));
}

// 错误处理
void FtpManager::ftperror(QNetworkReply::NetworkError error)
{
    switch (error) {
    case QNetworkReply::HostNotFoundError :
        qDebug() << QString::fromLocal8Bit("主机没有找到");
        break;
    default:
        qDebug() << QString::fromLocal8Bit("其他错误处理");
        break;
    }
}

// 更新上传进度
void FtpManager::ftpuploadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    qDebug() << bytesSent << bytesTotal ;
    pTimeout->stop();
//    m_pUploadBar->setMaximum(bytesTotal);
//    m_pUploadBar->setValue(bytesSent);
}

// 更新下载进度
void FtpManager::ftpdownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    qDebug() << bytesReceived << bytesTotal ;
    pTimeout->stop();
    if(m_pDownloadBar){
        m_pDownloadBar->setMaximum(bytesTotal);
        m_pDownloadBar->setValue(bytesReceived);
    }
}

// 下载过程中写文件
void FtpManager::finished()
{
    QNetworkReply *pReply = qobject_cast<QNetworkReply *>(sender());
    bool result = true;
    switch (pReply->error()) {
    case QNetworkReply::NoError : {
        m_file.write(pReply->readAll());
        m_file.flush();
    }
        break;
    default:
        qDebug() << " download err: please Check Network Status  " ;
        result = false;
        break;
    }

    m_file.close();
    pReply->deleteLater();

    emit sigfinish(result);
}

#ifndef USEUSERINFO_H
#define USEUSERINFO_H

#include <QNetworkInterface>
#include <QHostAddress>
#include <QSysInfo>
#include "QSettings"
#include <QDesktopWidget>
#include <QFileInfoList>
#include <QDir>
#include <QLibrary>
#include <QTimer>
#include <QHostInfo>
#include <QTcpSocket>

class useUserInfo
{
public:
    useUserInfo();
    //获取计算机名称
    const QString localmachineName();
    //本地IP地址
    const QString ip();
    //公网IP
    const QString publicIp();
    //获取计算机MAC地址
    const QString mac();
    //获取计算机操作系统
    const QString osVersion();
    //判断计算机是否可以联网
    bool ipLive();
    //获取Windows操作系统版本
    QString getWindowsVersion();
    //获取编译这个程序的平台位数
    int getSystemBit();

private:
    QString ipconfigCMDStr;
};

#endif // USEUSERINFO_H

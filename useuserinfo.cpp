#include "useuserinfo.h"
#include "mainwindow.h"
#include "qsysinfo.h"

useUserInfo::useUserInfo()
{

}

const QString useUserInfo::localmachineName()
{
    QString machineName = QHostInfo::localHostName();
    return machineName;
}

const QString useUserInfo::ip()
{
    #if 0
    QString localIPAddress = "";
    QList<QHostAddress> listAddress = QNetworkInterface::allAddresses();
    for(int j = 0; j < listAddress.size(); j++){
        qDebug() << listAddress.at(j).toString() ;
      if(!listAddress.at(j).isNull()
          && listAddress.at(j).protocol() == QAbstractSocket::IPv4Protocol
          && listAddress.at(j) != QHostAddress::LocalHost){
              localIPAddress = listAddress.at(j).toString();
              return localIPAddress;
      }
    }
    return localIPAddress;

    QString netInfo = "";
    foreach(QNetworkInterface interfacetmp, QNetworkInterface::allInterfaces())
    {
        netInfo += "-------------------------\nDevice:" + interfacetmp.name() + "\n" +
                "Hardware Address:" + interfacetmp.hardwareAddress() + "\n";
        foreach(QNetworkAddressEntry addressEntry, interfacetmp.addressEntries())
        {
            if(addressEntry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                netInfo += "IPv4 Address:" + addressEntry.ip().toString()+ "\n";
            }
            else if(addressEntry.ip().protocol() == QAbstractSocket::IPv6Protocol)
            {
                netInfo += "IPv6 Address:" + addressEntry.ip().toString()+ "\n";
            }
            else
            {}
            netInfo += "Broadcast:" + addressEntry.broadcast().toString() + "\n" +
                "Mask:" + addressEntry.netmask().toString() + "\n===\n";
        }
    }

    return netInfo;
    #endif

    char pszIP[16] = {0};
    char pPath[256] = {0};
    QString netMaskStr = QCoreApplication::applicationDirPath() + "/NetMask.txt";
    strcat(pPath, netMaskStr.toUtf8().constData());
    char pszCmd[256] = {0};
    strcpy(pszCmd, "ipconfig /all > ");
    strcpy(pszCmd + strlen(pszCmd), pPath);

    //执行DOS命令,并把网络信息输出到文件夹下
    //system(pszCmd); //弹框
    //不弹框
    QFile ipconfigCMDfile(QDir::toNativeSeparators(QCoreApplication::applicationDirPath()) + "\\ipconfigCMD.bat");
    if(!ipconfigCMDfile.open(QIODevice::ReadWrite | QIODevice::Truncate))//Truncate清空内容再写
    {
        qDebug() << "Open failed.";
        return NULL;
    }

    QTextStream openUrlIn(&ipconfigCMDfile);
    openUrlIn << pszCmd;
    ipconfigCMDfile.close();

    QProcess p(NULL);
    p.start(QCoreApplication::applicationDirPath() + "/ipconfigCMD.bat");
    p.waitForFinished();
    QFile::remove(QCoreApplication::applicationDirPath() + "/ipconfigCMD.bat");

    QFile f(QCoreApplication::applicationDirPath() + "/NetMask.txt");
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::Truncate))
    {
        qDebug() << "Open failed.";
        QFile::remove(QCoreApplication::applicationDirPath() + "/NetMask.txt");
        return NULL;
    }

    QTextStream txtInput(&f);
    while(!txtInput.atEnd())
    {
        ipconfigCMDStr = txtInput.readAll();
    }
    f.close();
    QFile::remove(QCoreApplication::applicationDirPath() + "/NetMask.txt");

    char pBuf[1024 * 10] = {0};
    strcpy(pBuf, ipconfigCMDStr.toUtf8().constData());
    char *pLAN = strstr(pBuf, "本地连接:");
    char *pIPV4 = strstr(pLAN, "IPv4 地址");
    pIPV4 = strstr(pIPV4, ":");
    pIPV4 += 2; //翻过“：”符号，获得真正的IP
    char *pEnd = strstr(pIPV4, "(");

    strncpy(pszIP, pIPV4, pEnd - pIPV4);

    //qDebug() << pszIP;
    QString curIP = pszIP;

    return curIP;

}

const QString useUserInfo::publicIp()
{
    QString strIp;
    QString webCode = "";//getHtml("http://whois.pconline.com.cn/");

    if(!webCode.isEmpty())
    {
        QString web = webCode.replace(" ", "");
        web = web.replace("\r", "");
        web = web.replace("\n", "");
        QStringList list = web.split("<br/>");
        QString tar = list[3];
        QStringList ip = tar.split("=");
        strIp = ip[1];
    }
    else
        strIp="无法获取公网ip";

    return strIp;
}

const QString useUserInfo::mac()
{
    QString strMac;

    char pBuf[1024 * 10] = {0};
    strcpy(pBuf, ipconfigCMDStr.toUtf8().constData());
    char *pLAN = strstr(pBuf, "本地连接:");
    char *pMac = strstr(pLAN, "物理地址");
    pMac = strstr(pMac, ":");
    pMac += 2; //翻过“：”符号，获得真正的Mac

    char pszMac[256] = {0};
    strncpy(pszMac, pMac, 17);

    strMac = pszMac;

    return strMac;
}

const QString useUserInfo::osVersion()
{
    QString m_osDescirbe;
//    typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
//    LPFN_ISWOW64PROCESS fnIsWow64Process;
//    BOOL bIsWow64 = FALSE;
//    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress( GetModuleHandle(TEXT("kernel32")),"IsWow64Process");
//    if (NULL != fnIsWow64Process)
//    {
//        fnIsWow64Process(GetCurrentProcess(),&bIsWow64);
//    }
//    QString sysBit = "unknown";
//    if(bIsWow64)
//        sysBit = "64位";
//    else
//        sysBit = "32位";

    m_osDescirbe = QSysInfo::prettyProductName() + " " + "sysBit";
    return m_osDescirbe;
}

bool useUserInfo::ipLive()
{
    //能接通百度IP说明可以通外网
    QTcpSocket tcpClient;
    tcpClient.abort();
    tcpClient.connectToHost("202.108.22.5", 80);
    //100毫秒没有连接上则判断不在线
    return tcpClient.waitForConnected(100);
}

QString useUserInfo::getWindowsVersion()
{
    QString winVersion = "";
    QSysInfo::WinVersion wv = QSysInfo::windowsVersion();
    switch (wv) {
    case QSysInfo::WV_XP:
        winVersion = "Windows XP";
        break;
    case QSysInfo::WV_2003:
        winVersion = "Windows Server 2003";
        break;
    case QSysInfo::WV_VISTA:
        winVersion = "Windows Vista";
        break;
    case QSysInfo::WV_WINDOWS7:
        winVersion = "Windows 7";
        break;
    case QSysInfo::WV_WINDOWS8:
        winVersion = "Windows 8";
        break;
    case QSysInfo::WV_WINDOWS8_1:
        winVersion = "Windows 8.1";
        break;
    case QSysInfo::WV_WINDOWS10:
        winVersion = "Windows 10";
        break;
    default:
        break;
    }
    return winVersion;

}

int useUserInfo::getSystemBit()
{
    return QSysInfo::WordSize;
}

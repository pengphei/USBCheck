#include "DiskCheck.h"
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QFile>
#include <QCryptographicHash>
#include <QByteArray>
#include <QTextStream>
#include <string>
#include <QDebug>

#define DIR_FLAG "D"
#define FILE_FLAG "F"
#define DOT_FLAG ","
#define CDOT_FLAG ','

DiskCheck* DiskCheck::pInstance = NULL;

DiskCheck::DiskCheck(QObject *parent) : QObject(parent)
{
    connect(this, SIGNAL(startCheck(QString,QString)), this, SLOT(DBCheck(QString,QString)));
    connect(this, SIGNAL(startGenerate(QString,QString)), this, SLOT(DBGenerate(QString,QString)));

    this->moveToThread(&this->task);
    this->task.start();
}

int32_t DiskCheck::dirWrite(QTextStream &stream, const QString &path)
{
    stream << QString(DIR_FLAG) << QString(DOT_FLAG) << path << "\r\n";
    return 0;
}

bool DiskCheck::dirCheck(const QString &path)
{
    return QFileInfo::exists(path);
}

int32_t DiskCheck::fileWrite(QTextStream &stream, const QString &path, const QByteArray &md5)
{
    stream << QString(FILE_FLAG) << QString(DOT_FLAG) << path << QString(DOT_FLAG) << md5 << "\r\n";
    return 0;
}

bool DiskCheck::fileCheck(const QString &path, const QByteArray &md5)
{
   QByteArray ba = MD5Check(path);
   return (ba == md5);
}

QByteArray DiskCheck::MD5Check(const QString &fPath)
{
    QFile df(fPath);
    df.open(QIODevice::ReadOnly);
    QByteArray ba = QCryptographicHash::hash(df.readAll(), QCryptographicHash::Md5);
    df.close();
    return ba.toHex();
}

int32_t DiskCheck::DBGenerateSub(QTextStream &stream, const QString &subPath)
{
    QDir subDir = QDir(subPath);
    QFileInfoList fInfoList = subDir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries, QDir::DirsFirst);

    foreach (QFileInfo fInfo, fInfoList)
    {
        if(fInfo.isDir())
        {
            dirWrite(stream, fInfo.absoluteFilePath());
            DBGenerateSub(stream, fInfo.absoluteFilePath());
            notifyGenerate(DISK_GENE_INFO, fInfo.absoluteFilePath());
        }
        else if(fInfo.isFile())
        {
            QByteArray md5 = MD5Check(fInfo.absoluteFilePath());
            fileWrite(stream, fInfo.absoluteFilePath(), md5);
            notifyGenerate(DISK_GENE_INFO, fInfo.absoluteFilePath());
        }
    }

    notifyCheck(DISK_GENE_FINI, tr("All File Generate MD5 Finished"), 100);

    return 0;
}

int32_t DiskCheck::DBGenerate(const QString &disk, const QString &dbPath)
{
    QFile dbFile(dbPath);
    dbFile.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&dbFile);
    DBGenerateSub(stream, disk);
    dbFile.flush();
    dbFile.close();
    return 0;
}

int32_t DiskCheck::DBCheck(const QString &disk, const QString &dbPath)
{
    QFile fmd5(dbPath);
    fmd5.open(QIODevice::ReadOnly|QIODevice::Text);
    fmd5.seek(-1);
    qint64 endPos = fmd5.pos();
    qint64 curPos = 0;
    int32_t percent = 0;
    fmd5.seek(0);

    while(false == fmd5.atEnd())
    {
        QList<QByteArray> sitems = fmd5.readLine().split(CDOT_FLAG);
        curPos = fmd5.pos();
        percent = curPos*100/endPos;
        if(sitems.at(0) == DIR_FLAG)
        {
            if(dirCheck(sitems.at(1)))
            {
                notifyCheck(DISK_CHECK_INFO, sitems.at(1), percent);
            }
            else
            {
                notifyCheck(DISK_CHECK_ERR0R, sitems.at(1), percent);
            }
        }
        else if(sitems.at(0) == FILE_FLAG)
        {
            if(fileCheck(sitems.at(1), sitems.at(2)))
            {
                notifyCheck(DISK_CHECK_INFO, sitems.at(1), percent);;
            }
            else
            {
                notifyCheck(DISK_CHECK_ERR0R, sitems.at(1), percent);
            }
        }
    }

    notifyCheck(DISK_CHECK_FINI, tr("All File Check Finished"), 100);

    return 0;
}

DiskCheck *DiskCheck::getInstance()
{
    if(NULL == pInstance)
    {
        pInstance = new DiskCheck;
    }
    return pInstance;
}

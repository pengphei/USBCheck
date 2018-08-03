#ifndef DISKCHECK_H
#define DISKCHECK_H

#include <QObject>
#include <QThread>

class QByteArray;
class QTextStream;
class QDir;

typedef enum
{
    DISK_CHECK_INFO = 0,
    DISK_CHECK_ERR0R = 1,
    DISK_CHECK_FINI = 2,
}eDiskCheckFlagT;

typedef enum
{
    DISK_ERROR_NOT_EXISTED = 0,
    DISK_ERROR_PARTITION_ERROR = 1,
    DISK_ERROR_DIR_NOT_EXISTED = 2,
    DISK_ERROR_FILE_NOT_EXISTED = 3,
    DISK_ERROR_FILE_CHK_FAILED = 4,
    DISK_ERROR_BLOCK_CHK_FAILED = 5,
    DISK_ERROR_NUM,
}eDiskErrorCheckFlagT;

typedef enum
{
    DISK_GENE_INFO = 0,
    DISK_GENE_ERR0R = 1,
    DISK_GENE_FINI = 2,
}eDiskGeneFlagT;

class DiskCheck : public QObject
{
    Q_OBJECT
public:
    explicit DiskCheck(QObject *parent = nullptr);

    int32_t udiskScan();

    int32_t dirWrite(QTextStream& stream, const QString& path);
    bool dirCheck(const QString& path);

    int32_t fileWrite(QTextStream& stream, const QString& path, const QByteArray& md5);
    bool fileCheck(const QString& path, const QByteArray& md5);

    QByteArray MD5Check(const QString& fPath);

    int32_t DBGenerateSub(QTextStream &stream, const QString &subPath);

public:
    static DiskCheck* getInstance();

signals:
    void notifyCheck(int flag, const QString& tag, int percent);
    void notifyGenerate(int flag, const QString& tag);
    void startGenerate(const QString& disk, const QString& dbPath);
    void startCheck(const QString& disk, const QString& dbPath);

private slots:
    int32_t DBGenerate(const QString& disk, const QString& dbPath);
    int32_t DBCheck(const QString& disk, const QString& dbPath);

private:
    static DiskCheck* pInstance;
     QThread task;
};

#endif // DISKCHECK_H

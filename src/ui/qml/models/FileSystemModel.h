#pragma once

#include <QFileSystemModel>
#include <QQuickTextDocument>
#include <QStandardPaths>

class FileSystemModel : public QFileSystemModel
{
    Q_OBJECT

    Q_PROPERTY(QModelIndex rootIndex READ rootIndex WRITE setRootIndex NOTIFY rootIndexChanged)
    Q_PROPERTY(QString appPath READ getAppPath CONSTANT)
    Q_PROPERTY(QString desktopPath READ getDesktopPath CONSTANT)
    Q_PROPERTY(QString homePath READ getHomePath CONSTANT)
    Q_PROPERTY(QString downloadsPath READ getDownloadsPath CONSTANT)

public:
    explicit FileSystemModel(QObject *parent = nullptr);

    // Functions invokable from QML
    Q_INVOKABLE void readFile(const QString &filePath);

    Q_INVOKABLE void setTreeRootDirectory(const QString &path = getDefaultRootDir());
    Q_INVOKABLE void setParentDirectoryAsRoot();
    Q_INVOKABLE QString getCurrentRootPath();
    Q_INVOKABLE void setToFilesystemRoot();


    // Overridden functions
    int columnCount(const QModelIndex &parent) const override;

    // Member functions from here
    QModelIndex rootIndex() const;
    void setRootIndex(const QModelIndex index);

    static QString getDefaultRootDir();

    //Constants
    QString getAppPath() {return QDir::currentPath();};
    QString getDesktopPath() {return QDir(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)).path();};
    QString getHomePath() {return QDir(QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).path();};
    QString getDownloadsPath() {return QDir(QStandardPaths::writableLocation(QStandardPaths::DownloadLocation)).path();};

Q_SIGNALS:
    void rootIndexChanged();

private:
    QModelIndex m_rootIndex;
};


#include "FileSystemModel.h"

#include <QMimeDatabase>
#include <QTextDocument>
#include <QTextObject>
#include <iostream>

FileSystemModel::FileSystemModel(QObject *parent) : QFileSystemModel(parent)
{
    setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);

    setToFilesystemRoot();
}

void FileSystemModel::setParentDirectoryAsRoot() {
    QDir currentDir = rootDirectory();
    if(currentDir.cdUp()) {
        setTreeRootDirectory(currentDir.path());
    }
}

QString FileSystemModel::getCurrentRootPath() {
    return rootDirectory().path();
}

void FileSystemModel::readFile(const QString &filePath)
{
    std::cout << "READ FILE" << std::endl;
}

int FileSystemModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 1;
}

QModelIndex FileSystemModel::rootIndex() const
{
    return m_rootIndex;
}

void FileSystemModel::setRootIndex(const QModelIndex index)
{
    if (index == m_rootIndex)
        return;
    m_rootIndex = index;
    emit rootIndexChanged();
}

void FileSystemModel::setTreeRootDirectory(const QString &path)
{
    QDir dir(path);
    if (dir.makeAbsolute())
        setRootPath(dir.path());
    else
        setRootPath(getDefaultRootDir());

    setRootIndex(QFileSystemModel::index(dir.path(), 0));
}

void FileSystemModel::setToFilesystemRoot()
{
    setRootPath(nullptr);
    setRootIndex(QFileSystemModel::index(nullptr, 0));
}

QString FileSystemModel::getDefaultRootDir()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

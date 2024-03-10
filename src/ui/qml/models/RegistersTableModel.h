#pragma once

#include <qqml.h>
#include <QAbstractTableModel>

class RegistersTableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_ELEMENT

public:
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    enum numberSystem {HEX, DEC, BIN};
    void switchNumberSystem(numberSystem sys);

    Q_INVOKABLE void switchToHex();
    Q_INVOKABLE void switchToDec();
    Q_INVOKABLE void switchToBin();

private:
    numberSystem _currentNumberSystem = HEX;
};

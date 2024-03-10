#include "RegistersTableModel.h"

//###################################################################
//# Override table model members
//###################################################################

int RegistersTableModel::rowCount(const QModelIndex &) const
{
    return 32;
}

int RegistersTableModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant RegistersTableModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return QString("%1, %2").arg(index.column()).arg(index.row());
    default:
        break;
    }

    return QVariant();
}

QVariant RegistersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Horizontal)
        return QVariant();

    switch(section) {
    case 0:
        return "Register";
    case 1:
        return "ABI name";
    case 2:
        return "Value";
    }

    return QVariant();
}

QHash<int, QByteArray> RegistersTableModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//###################################################################
//# Other public members
//###################################################################

void RegistersTableModel::switchToHex()
{
    switchNumberSystem(HEX);
}

void RegistersTableModel::switchToDec()
{
    switchNumberSystem(DEC);
}

void RegistersTableModel::switchToBin()
{
    switchNumberSystem(BIN);
}

void RegistersTableModel::switchNumberSystem(numberSystem sys)
{
    if(sys != _currentNumberSystem) {
        _currentNumberSystem = sys;
        //TODO: signal number system change
    }
}

//###################################################################
//# Other private members
//###################################################################

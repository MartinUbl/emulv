#include "RegistersTableModel.h"
#include <bitset>

//###################################################################
//# Override table model members
//###################################################################

int RegistersTableModel::rowCount(const QModelIndex &) const
{
    return _registersDefault.size();
}

int RegistersTableModel::columnCount(const QModelIndex &) const
{
    return VIEW_COLUMNS;
}

QVariant RegistersTableModel::data(const QModelIndex &index, int role) const
{

    switch (role) {
    case Qt::DisplayRole:
        if(index.column() == 0) {
            if(_registers.size() != 0) {
                return QString::fromStdString(std::get<0>(_registers.at(index.row())));
            } else {
                return QString::fromStdString(std::get<0>(_registersDefault.at(index.row())));
            }
        } else if (index.column() == 1) {
            if(_registers.size() != 0) {
                return QString::fromStdString(_abiNames.at(std::get<0>(_registers.at(index.row()))));
            } else {
                return QString::fromStdString(_abiNames.at(std::get<0>(_registersDefault.at(index.row()))));
            }
        } else if (index.column() == 2) {
            switch(_currentNumberSystem) {
            case HEX:
                if(_registers.size() != 0) {
                    return QString::fromStdString(_intToHex(std::get<1>(_registers.at(index.row())),8));
                } else {
                    return QString::fromStdString(_intToHex(std::get<1>(_registersDefault.at(index.row())),8));
                }
            case DEC:
                if(_registers.size() != 0) {
                    return std::get<1>(_registers.at(index.row()));
                } else {
                    return std::get<1>(_registersDefault.at(index.row()));
                }
            case BIN:
                if(_registers.size() != 0) {
                    return QString::fromStdString(_intToBin<uint32_t,8*4>(std::get<1>(_registers.at(index.row()))));
                } else {
                    return QString::fromStdString(_intToBin<uint32_t,8*4>(std::get<1>(_registersDefault.at(index.row()))));
                }
            }
        }
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

void RegistersTableModel::refreshTable()
{
    beginResetModel();
    endResetModel();
}

void RegistersTableModel::switchNumberSystem(numberSystem sys)
{
    if(sys != _currentNumberSystem) {
        _currentNumberSystem = sys;
        refreshTable();
    }
}

//###################################################################
//# Slots
//###################################################################

void RegistersTableModel::updateRegisters(std::vector<std::tuple<std::string, uint32_t> > registers)
{
    _registers = registers;
    refreshTable();
}

template<typename I>
std::string RegistersTableModel::_intToHex(I w, size_t len)
{
    static const char* digits = "0123456789ABCDEF";
    std::string rc(len,'0');
    for (size_t i=0, j=(len-1)*4 ; i<len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    //Remove leading zeroes
    rc.erase(0, rc.find_first_not_of('0'));
    if(rc.length() == 0)
        return "0";
    return rc;
}

template<typename I, int LEN>
std::string RegistersTableModel::_intToBin(I w)
{
    return std::bitset<LEN>(w).to_string();
}


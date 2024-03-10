#include "MemoryTableModel.h"
#include <limits>
#include <bitset>
#include <iostream>

//###################################################################
//# Override table model members
//###################################################################

int MemoryTableModel::rowCount(const QModelIndex &) const
{
    return std::ceil(_memory.size()/(double)_memoryCellColumns);
}

int MemoryTableModel::columnCount(const QModelIndex &) const
{
    return _totalColumns;
}

QVariant MemoryTableModel::data(const QModelIndex &index, int role) const
{

    if(role == Qt::DisplayRole) {
        if(index.column() >= _memoryCellColumns) {
            // String columns

            // Separator
            if(index.column() == _memoryCellColumns) {
                return " ";
            }
            // Get memory cell coresponding to this position
            int memoryCell = (index.column() - 1 - _memoryCellColumns) + index.row() * _memoryCellColumns;
            if(memoryCell >= _memory.size()) {
                return QString(" ");
            }

            char content = _memory[memoryCell];
            if(isprint(content)) {
                return QString(content);
            } else {
                return QString('.');
            }
        } else {
            // Memory columns
            int memoryCell = index.column() + index.row()*_memoryCellColumns;
            if(memoryCell >= _memory.size()) {
                return QString(" ");
            } else {
                return QString::fromStdString(intToMemoryByte(_memory[index.column() + index.row() * _memoryCellColumns]));
            }
        }
    }

    return QVariant();
}

QVariant MemoryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    // Memory addresses
    if (orientation == Qt::Vertical) {
        int address = getMemoryStartAddress() + (section * _memoryCellColumns);

        // Display as 8 hex characters when possible
        if(address > std::numeric_limits<uint32_t>::max())
            return QString::fromStdString(_intToHexPrefixed(address));
        else
            return QString::fromStdString(_intToHexPrefixed(address, 8));

    }

    // Offsets
    if (orientation == Qt::Horizontal) {
        if(section >= _memoryCellColumns)
            return " ";
        else
            return QString::fromStdString(_intToHex(section, 2));
    }

    return QVariant();
}

QHash<int, QByteArray> MemoryTableModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//###################################################################
//# Other public members
//###################################################################

void MemoryTableModel::switchToHex()
{
    switchNumberSystem(HEX);
}

void MemoryTableModel::switchToDec()
{
    switchNumberSystem(DEC);
}

void MemoryTableModel::switchToBin()
{
    switchNumberSystem(BIN);
}

void MemoryTableModel::switchNumberSystem(numberSystem sys)
{
    if(sys != _currentNumberSystem) {
        _currentNumberSystem = sys;
        refreshModel();
    }
}

template<typename I>
std::string MemoryTableModel::intToMemoryByte(I w) const
{
    if(_currentNumberSystem == HEX) {
        return _intToHex(w,2);
    } else if(_currentNumberSystem == DEC) {
        // Length must be 3 (255 is max number)
        std::string num = std::to_string(w);
        if(num.length() >= 3) return num;
        std::string leadingZero(3 - num.length(), '0');

        return leadingZero + num;
    } else if(_currentNumberSystem == BIN) {
        return _intToBin<I,8>(w);
    }
    return "";
}

void MemoryTableModel::loadMemory(std::vector<uint8_t> &&memory, uint64_t startAddress)
{
    _memory = std::move(memory);
    _startAddress = startAddress;
}

uint64_t MemoryTableModel::getMemoryStartAddress() const
{
    return _startAddress;
}

uint64_t MemoryTableModel::getMemoryEndAddress() const
{
    return _startAddress + _memory.size();
}

void MemoryTableModel::setMemoryCellColumns(int columnNumber)
{
    _memoryCellColumns = columnNumber;
    _stringColumns = _memoryCellColumns + 1;
    _totalColumns = _memoryCellColumns + _stringColumns;
}

int MemoryTableModel::getRowFromAddress(uint64_t address)
{
    if(address < getMemoryStartAddress())
        return 0;
    if(address > getMemoryEndAddress())
        return 0;

    return (address - getMemoryStartAddress())/_memoryCellColumns;

}

void MemoryTableModel::refreshModel()
{
    beginResetModel();
    endResetModel();
}

//###################################################################
//# Other private members
//###################################################################
template<typename I>
std::string MemoryTableModel::_intToHexPrefixed(I w, size_t len)
{
    static const char* digits = "0123456789ABCDEF";
    std::string rc(len,'0');
    for (size_t i=0, j=(len-1)*4 ; i<len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return "0x" + rc;
}

template<typename I>
std::string MemoryTableModel::_intToHex(I w, size_t len)
{
    static const char* digits = "0123456789ABCDEF";
    std::string rc(len,'0');
    for (size_t i=0, j=(len-1)*4 ; i<len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}

template<typename I, int LEN>
std::string MemoryTableModel::_intToBin(I w)
{
    return std::bitset<LEN>(w).to_string();
}




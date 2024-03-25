#include "MemoryTableModel.h"
#include <limits>
#include <bitset>
#include <iostream>

//###################################################################
//# Override table model members
//###################################################################

int MemoryTableModel::rowCount(const QModelIndex &) const {
    // TODO?
    return std::ceil(_memoryPageModel->getMemorySizeBytes() / (double) _memoryCellColumns);
}

int MemoryTableModel::columnCount(const QModelIndex &) const {
    return _totalColumns;
}

QVariant MemoryTableModel::data(const QModelIndex &index, int role) const {

    if (role == Qt::DisplayRole) {
        if (index.column() >= _memoryCellColumns) {
            // String columns

            // Separator column
            if (index.column() == _memoryCellColumns) {
                return " ";
            }

            // Get memory cell coresponding to this position
            int memoryCell = (index.column() - 1 - _memoryCellColumns) + index.row() * _memoryCellColumns;
            if (memoryCell >= _memoryPageModel->getMemorySizeBytes()) {
                return QString(" ");
            }

            char content = _memoryPageModel->getByteAtIndex(memoryCell);
            if (isprint(content)) {
                return QString(content);
            } else {
                return QString('.');
            }
        } else {
            // Memory columns

            int memoryCell = index.column() + index.row() * _memoryCellColumns;
            if (memoryCell >= _memoryPageModel->getMemorySizeBytes()) {
                return QString(" ");
            } else {
                return QString::fromStdString(intToMemoryByte(_memoryPageModel->getByteAtIndex(memoryCell)));
            }
        }
    }

    return QVariant();
}

QVariant MemoryTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    // Memory addresses
    if (orientation == Qt::Vertical) {
        uint64_t address = getMemoryStartAddress() + _memoryPageModel->getAddressAtIndex(section * _memoryCellColumns);

        // Display as 8 hex characters when possible
        if (address > std::numeric_limits<uint32_t>::max())
            return QString::fromStdString(_intToHexPrefixed(address));
        else
            return QString::fromStdString(_intToHexPrefixed(address, 8));

    }

    // Offsets
    if (orientation == Qt::Horizontal) {
        if (section >= _memoryCellColumns)
            return " ";
        else
            return QString::fromStdString(_intToHex(section, 2));
    }

    return QVariant();
}

QHash<int, QByteArray> MemoryTableModel::roleNames() const {
    return {{Qt::DisplayRole, "display"}};
}

//###################################################################
//# Other public members
//###################################################################

void MemoryTableModel::switchToHex() {
    switchNumberSystem(HEX);
}

void MemoryTableModel::switchToDec() {
    switchNumberSystem(DEC);
}

void MemoryTableModel::switchToBin() {
    switchNumberSystem(BIN);
}

void MemoryTableModel::switchNumberSystem(numberSystem sys) {
    if (sys != _currentNumberSystem) {
        _currentNumberSystem = sys;
        refreshView();
    }
}

template<typename I>
std::string MemoryTableModel::intToMemoryByte(I w) const {
    if (_currentNumberSystem == HEX) {
        return _intToHex(w, 2);
    } else if (_currentNumberSystem == DEC) {
        // Length must be 3 (255 is max number)
        std::string num = std::to_string(w);
        if (num.length() >= 3) return num;
        std::string leadingZero(3 - num.length(), '0');

        return leadingZero + num;
    } else if (_currentNumberSystem == BIN) {
        return _intToBin<I, 8>(w);
    }
    return "";
}

uint64_t MemoryTableModel::getMemoryStartAddress() const {
    return MemoryPageModel::getStartAddress();
}

uint64_t MemoryTableModel::getMemoryEndAddress() const {
    return _memoryPageModel->getEndAddress();
}

void MemoryTableModel::setMemoryCellColumns(int columnNumber) {
    _memoryCellColumns = columnNumber;
    _stringColumns = _memoryCellColumns + 1;
    _totalColumns = _memoryCellColumns + _stringColumns;
}

int MemoryTableModel::getRowFromAddress(uint64_t address) {
    if (address < getMemoryStartAddress())
        return 0;

    return (_memoryPageModel->getOffsetFromStart(address) - getMemoryStartAddress()) / _memoryCellColumns;
}

void MemoryTableModel::refreshView() {
    beginResetModel();
    endResetModel();
}

void MemoryTableModel::memoryRefreshed() {
    _memoryPageModel->refresh();
    refreshView();
}

void
MemoryTableModel::memoryPointerChanged(const emulator::PagesMap *pages) {
    _memoryPageModel->setMemoryPages(pages);
    refreshView();
}


//###################################################################
//# Other private members
//###################################################################
template<typename I>
std::string MemoryTableModel::_intToHexPrefixed(I w, size_t len) {
    static const char *digits = "0123456789ABCDEF";
    std::string rc(len, '0');
    for (size_t i = 0, j = (len - 1) * 4; i < len; ++i, j -= 4)
        rc[i] = digits[(w >> j) & 0x0f];
    return "0x" + rc;
}

template<typename I>
std::string MemoryTableModel::_intToHex(I w, size_t len) {
    static const char *digits = "0123456789ABCDEF";
    std::string rc(len, '0');
    for (size_t i = 0, j = (len - 1) * 4; i < len; ++i, j -= 4)
        rc[i] = digits[(w >> j) & 0x0f];
    return rc;
}

template<typename I, int LEN>
std::string MemoryTableModel::_intToBin(I w) {
    return std::bitset<LEN>(w).to_string();
}

//###################################################################
//###################################################################
//# MEMORY PAGE MODEL
//###################################################################
//###################################################################
MemoryPageModel::MemoryPageModel(const emulator::PagesMap *memoryPages) {
    setMemoryPages(memoryPages);
}

void MemoryPageModel::setMemoryPages(const emulator::PagesMap *memoryPages) {
    _memoryPages = memoryPages;
    refresh();
}

void MemoryPageModel::refresh() {
    _pageAddresses.clear();

    if (!_memoryPages)
        return;

    // Put all keys of _memoryPages into _pageAddresses
    for (auto const &kv: *_memoryPages) {
        _pageAddresses.insert(kv.first);
    }
}

uint64_t MemoryPageModel::getMemorySizeBytes() const {
    if (!_memoryPages)
        return 0;

    return _memoryPages->size() * RISCV_PAGE_SIZE;
}

uint64_t MemoryPageModel::getStartAddress() {
    return _startAddress;
}

uint64_t MemoryPageModel::getEndAddress() const {
    if (_pageAddresses.empty())
        return _startAddress;

    return _startAddress + *_pageAddresses.rbegin();
}

uint8_t MemoryPageModel::getByteAtIndex(uint64_t index) {
    if (!_memoryPages)
        return 254;

    emulator::MachineAddress possiblePageAddress = std::floor(index / (double) RISCV_PAGE_SIZE);
    uint64_t offsetFromPageStart = index % RISCV_PAGE_SIZE;

    if (_memoryPages->contains(possiblePageAddress)) {
        // this page is present, simply return byte
        return _memoryPages->at(possiblePageAddress).page().buffer8[offsetFromPageStart];
    } else {
        // there is no page at this address -> get the nearest page bigger than this address

        // Perform automatic refresh if sizes don't match (won't work if memory pages modified but size stays the same)
        if (_memoryPages->size() != _pageAddresses.size()) {
            refresh();
        }

        emulator::MachineAddress nearestPage = _getNearestPageTo(possiblePageAddress);
        // Refresh needs to be performed if this address is not in _memoryPages
        if (!_memoryPages->contains(nearestPage)) {
            refresh();
            nearestPage = _getNearestPageTo(possiblePageAddress);
        }

        // Page still doesn't exist even after refresh -> return 0
        if (!_memoryPages->contains(nearestPage)) {
            // !!!TODO!!!
            return 254;
        }

        // Return the page byte
        return _memoryPages->at(nearestPage).page().buffer8[offsetFromPageStart];
    }
}

uint64_t MemoryPageModel::getAddressAtIndex(uint64_t index) {
    if (!_memoryPages)
        return 254;

    emulator::MachineAddress possiblePageAddress = std::floor(index / (double) RISCV_PAGE_SIZE);
    uint64_t offsetFromPageStart = index % RISCV_PAGE_SIZE;

    if (_memoryPages->contains(possiblePageAddress)) {
        // this page is present, address is the same as index
        return (possiblePageAddress * RISCV_PAGE_SIZE) + offsetFromPageStart;
    } else {
        // there is no page at this address -> get the nearest page bigger than this address

        // Perform automatic refresh if sizes don't match (won't work if memory pages modified but size stays the same)
        if (_memoryPages->size() != _pageAddresses.size()) {
            refresh();
        }

        emulator::MachineAddress nearestPage = _getNearestPageTo(possiblePageAddress);
        // Refresh needs to be performed if this address is not in _memoryPages
        if (!_memoryPages->contains(nearestPage)) {
            refresh();
            nearestPage = _getNearestPageTo(possiblePageAddress);
        }

        // Page still doesn't exist even after refresh -> return 0
        if (!_memoryPages->contains(nearestPage)) {
            // !!!TODO!!!
            return 254;
        }

        // Return the address of this page + offset
        return (nearestPage * RISCV_PAGE_SIZE) + offsetFromPageStart;
    }
}

emulator::MachineAddress MemoryPageModel::_getNearestPageTo(emulator::MachineAddress addr) const {
    const std::set<emulator::MachineAddress>::const_iterator &upper = _pageAddresses.upper_bound(addr);
    // This will return "addr" if no upper bound is found
    if((upper == _pageAddresses.end()) && (*_pageAddresses.end() <= addr)) {
        return addr;
    } else {
        return *upper;
    }
}

uint64_t MemoryPageModel::getOffsetFromStart(uint64_t address) {
    emulator::MachineAddress pageAddress = std::floor(address / (double) RISCV_PAGE_SIZE);
    uint64_t offsetFromPageStart = address % RISCV_PAGE_SIZE;

    if (!_pageAddresses.contains(pageAddress)) {
        return 0;
    } else {
        return (std::distance(_pageAddresses.begin(), _pageAddresses.find(pageAddress)) * RISCV_PAGE_SIZE) + offsetFromPageStart;
    }
}


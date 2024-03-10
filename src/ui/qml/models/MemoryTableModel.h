#pragma once

#include <qqml.h>
#include <QAbstractTableModel>

class MemoryTableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
    QML_ELEMENT

public:
    MemoryTableModel() {
        for(int i = 0; i < 2998; i++) {
            _memory.push_back(i%255);
        }
        _startAddress = 1;
    }

    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    enum numberSystem {HEX, DEC, BIN};
    void switchNumberSystem(numberSystem sys);
    template<typename I>
    std::string intToMemoryByte(I w) const;

    Q_INVOKABLE void switchToHex();
    Q_INVOKABLE void switchToDec();
    Q_INVOKABLE void switchToBin();

    void loadMemory(std::vector<uint8_t>&& memory, uint64_t startAddress);
    uint64_t getMemoryStartAddress() const;
    uint64_t getMemoryEndAddress() const;

    Q_INVOKABLE void setMemoryCellColumns(int columnNumber);
    Q_INVOKABLE int getRowFromAddress(uint64_t address);
    Q_INVOKABLE void refreshModel();

private:
    std::vector<uint8_t> _memory;
    uint64_t _startAddress = 0;
    int _memoryCellColumns = 8;
    int _stringColumns = _memoryCellColumns + 1;
    int _totalColumns = _memoryCellColumns + _stringColumns;

    numberSystem _currentNumberSystem = HEX;

    template<typename I>
    static std::string _intToHexPrefixed(I w, size_t len = sizeof(I)<<1);

    template<typename I>
    static std::string _intToHex(I w, size_t len = sizeof(I)<<1);

    template<typename I, int LEN>
    static std::string _intToBin(I w);
};

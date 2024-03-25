#pragma once

#include <qqml.h>
#include <QAbstractTableModel>
#include "EmulatorUnit.h"

/**
 * Helper class wrapping around std::unordered_map (emulator::PagesMap) containing memory pages
 */
class MemoryPageModel {
public:
    explicit MemoryPageModel() = default;
    explicit MemoryPageModel(const emulator::PagesMap *memoryPages);

    void setMemoryPages(const emulator::PagesMap *memoryPages);
    /**
     * Refreshes content of _pageAddresses, call when the content of _memoryPages changes
     */
    void refresh();
    uint64_t getMemorySizeBytes() const;

    static uint64_t getStartAddress();
    uint64_t getEndAddress() const;

    uint8_t getByteAtIndex(uint64_t index);
    /**
     * Address of the byte might be different than index (when the page space is not continuous)
     */
    uint64_t getAddressAtIndex(uint64_t index);

    uint64_t  getOffsetFromStart(uint64_t address);

    const emulator::PagesMap *getMemoryPages() const;

private:
    static constexpr uint64_t _startAddress = 0;

    const emulator::PagesMap* _memoryPages = nullptr;
    // Keeps sorted keys of _memoryPages (updated via refresh())
    std::set<emulator::MachineAddress> _pageAddresses{};

};

/**
 * The model used by GUI table
 */
class MemoryTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    MemoryTableModel() {}

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

    uint64_t getMemoryStartAddress() const;
    uint64_t getMemoryEndAddress() const;

    Q_INVOKABLE void setMemoryCellColumns(int columnNumber);
    Q_INVOKABLE int getRowFromAddress(uint64_t address);
    Q_INVOKABLE void refreshView();

    const std::unique_ptr<MemoryPageModel> &getMemoryPageModel() const;

public Q_SLOTS:
    void memoryRefreshed();
    void memoryPointerChanged(const emulator::PagesMap* pages);

private:
    std::unique_ptr<MemoryPageModel> _memoryPageModel = std::make_unique<MemoryPageModel>();

    int _rowCount = 0;
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

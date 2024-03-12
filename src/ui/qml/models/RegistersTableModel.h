#pragma once

#include <qqml.h>
#include <QAbstractTableModel>

class RegistersTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    static constexpr int VIEW_COLUMNS = 3;

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

    void refreshTable();

public Q_SLOTS:
    void updateRegisters(std::vector<std::tuple<std::string, uint32_t>> registers);

private:
    numberSystem _currentNumberSystem = HEX;
    template<typename I>
    static std::string _intToHex(I w, size_t len = sizeof(I)<<1);

    template<typename I, int LEN>
    static std::string _intToBin(I w);

    std::vector<std::tuple<std::string, uint32_t> > _registers = {};

    // Default values which will be shown in table when nothing is loaded
    const std::vector<std::tuple<std::string, uint32_t> > _registersDefault = {
        {"x0",0},
        {"x1",0},
        {"x2",0},
        {"x3",0},
        {"x4",0},
        {"x5",0},
        {"x6",0},
        {"x7",0},
        {"x8",0},
        {"x9",0},
        {"x10",0},
        {"x11",0},
        {"x12",0},
        {"x13",0},
        {"x14",0},
        {"x15",0},
        {"x16",0},
        {"x17",0},
        {"x18",0},
        {"x19",0},
        {"x20",0},
        {"x21",0},
        {"x22",0},
        {"x23",0},
        {"x24",0},
        {"x25",0},
        {"x26",0},
        {"x27",0},
        {"x28",0},
        {"x29",0},
        {"x30",0},
        {"x31",0},
        {"pc",0}
    };

    // Translates register name to ABI name
    const std::unordered_map<std::string, std::string>  _abiNames{
        {"x0","zero"},
        {"x1","ra"},
        {"x2","sp"},
        {"x3","gp"},
        {"x4","tp"},
        {"x5","t0"},
        {"x6","t1"},
        {"x7","t2"},
        {"x8","s0/fp"},
        {"x9","s1"},
        {"x10","a0"},
        {"x11","a1"},
        {"x12","a2"},
        {"x13","a3"},
        {"x14","a4"},
        {"x15","a5"},
        {"x16","a6"},
        {"x17","a7"},
        {"x18","s2"},
        {"x19","s3"},
        {"x20","s4"},
        {"x21","s5"},
        {"x22","s6"},
        {"x23","s7"},
        {"x24","s8"},
        {"x25","s9"},
        {"x26","s10"},
        {"x27","s11"},
        {"x28","t3"},
        {"x29","t4"},
        {"x30","t5"},
        {"x31","t6"},
        {"pc",""}
    };

};

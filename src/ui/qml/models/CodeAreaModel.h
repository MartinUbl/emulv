#pragma once

#include <qqml.h>
#include <QAbstractTableModel>
#include <unordered_set>

class CodeAreaModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    int rowCount(const QModelIndex & = QModelIndex()) const override;

    int columnCount(const QModelIndex & = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    std::unordered_set<int> breakpointIds{};
    Q_INVOKABLE void removeAllBreakpoints();
    Q_INVOKABLE void toggleBreakpoint(int row);

public Q_SLOTS:
    void updateDisassembly(std::tuple<QList<uint64_t>, QList<QString>>);

Q_SIGNALS:
    void removeBreakpoint(uint64_t address);
    void addBreakpoint(uint64_t address);

private:
    QList<uint64_t> _lineNumbers;
    QList<QString> _textLines;

    template<typename I>
    static std::string _intToHex(I w, size_t len);
};

#include "CodeAreaModel.h"
#include <filesystem>
#include <fstream>
#include <iostream>

//###################################################################
//# Override table model members
//###################################################################

int CodeAreaModel::rowCount(const QModelIndex &) const
{
    return _textLines.size();
}

int CodeAreaModel::columnCount(const QModelIndex &) const
{
    return 2;
}

QVariant CodeAreaModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        if(index.column() == 0) {
            return QString::fromStdString(_intToHex(_lineNumbers[index.row()], 8));
        } else {
            return _textLines[index.row()];
        }
    default:
        break;
    }

    return QVariant();
}

QVariant CodeAreaModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation != Qt::Vertical)
        return QVariant();

    return breakpointIds.count(section) == 1;
}

QHash<int, QByteArray> CodeAreaModel::roleNames() const
{
    return { {Qt::DisplayRole, "display"} };
}

//###################################################################
//# Public members
//###################################################################
void CodeAreaModel::removeAllBreakpoints()
{
    for (const auto& row: breakpointIds) { emit
        emit removeBreakpoint(_lineNumbers[row]);
    }

    breakpointIds.clear();
    emit headerDataChanged(Qt::Vertical, 0, _lineNumbers.size());
}

void CodeAreaModel::toggleBreakpoint(int row)
{
    if(breakpointIds.count(row) == 1) {
        emit removeBreakpoint(_lineNumbers[row]);
        breakpointIds.erase(row);
        emit headerDataChanged(Qt::Vertical, 0, _lineNumbers.size());
    } else {
        breakpointIds.insert(row);
        emit headerDataChanged(Qt::Vertical, 0, _lineNumbers.size());
        //TODO add bp
        emit addBreakpoint(_lineNumbers[row]);

    }
}

void CodeAreaModel::updateDisassembly(std::tuple<QList<uint64_t>, QList<QString> > disassembly)
{
    removeAllBreakpoints();

    beginResetModel();
    _lineNumbers = std::get<0>(disassembly);
    _textLines = std::get<1>(disassembly);
    setHighlightedLine(-1);
    endResetModel();
}

void CodeAreaModel::stepTo(uint64_t pc)
{
    auto it = std::find(_lineNumbers.begin(), _lineNumbers.end(), pc);
    if (it == _lineNumbers.end())
    {
        // pc not in vector
        setHighlightedLine(-1);
    } else
    {
        auto index = std::distance(_lineNumbers.begin(), it);
        setHighlightedLine(index);
    }

    // Works without beginResetModel(), endResetModel()
}

void CodeAreaModel::saveAsTxt(QString path)
{
    if(_textLines.empty()) {
        return;
    }

    // Remove file:/// if it's present;
    const std::string prefix = "file:///";
    std::string pathStr = path.toStdString();
    pathStr = (pathStr.find(prefix) == 0) ? pathStr.substr(prefix.size()) : pathStr;

    std::filesystem::path fsPath{pathStr};
    if(!std::filesystem::exists(fsPath.parent_path())) {
        return;
    }

    std::ofstream ofs(fsPath);
    if(!ofs.is_open()) {
        return;
    }

    for (size_t i = 0; i < _textLines.size(); i++) {
        if(i < _lineNumbers.size()) {
            ofs << _intToHex(_lineNumbers[i], 8) << "\t";
        }
        ofs << _textLines[i].toStdString() << std::endl;
    }

    ofs.close();
}

size_t CodeAreaModel::highlightedLine() const
{
    return _highlightedLine;
}

void CodeAreaModel::setHighlightedLine(size_t newHighlightedLine)
{
    if (_highlightedLine == newHighlightedLine)
        return;
    _highlightedLine = newHighlightedLine;
    emit highlightedLineChanged();
}

//###################################################################
//# Private members
//###################################################################
template<typename I>
std::string CodeAreaModel::_intToHex(I w, size_t len)
{
    static const char* digits = "0123456789ABCDEF";
    std::string rc(len,'0');
    for (size_t i=0, j=(len-1)*4 ; i<len; ++i,j-=4)
        rc[i] = digits[(w>>j) & 0x0f];
    return rc;
}

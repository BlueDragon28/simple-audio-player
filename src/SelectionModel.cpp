#include "SelectionModel.h"
#include <algorithm>

SelectionModel::SelectionModel(QObject* parent) :
    QAbstractListModel(parent)
{}

SelectionModel::~SelectionModel()
{}

int SelectionModel::rowCount(const QModelIndex& parent) const
{
    return m_listData.count();
}

QVariant SelectionModel::data(const QModelIndex& index, int role) const
{
    // Check if the index is valid.
    if (index.isValid() &&
        index.column() == 0 &&
        index.row() >= 0 && index.row() < rowCount())
    {
        // If the view ask if the item is selected.
        if (role == IS_SELECTED)
        {
            return m_listData.at(index.row()).isSelected;
        }
    }

    return QVariant();
}

QHash<int, QByteArray> SelectionModel::roleNames() const
{
    /*
    Return the roles available for this model has string to 
    allow call them directly from JavaScript.
    */
    QHash<int, QByteArray> roles;
    roles[IS_SELECTED] = "isSelected";
    return roles;
}

int SelectionModel::lowestSelectedIndex() const
{
    // Get index position to an invalid value (one value ahead of max index of m_listData).
    int lowestIndex = m_listData.size();
        
    // Getting the lowest index inside m_selectionOrder list.
    foreach (int index, m_selectionOrder)
    {
        if (index < lowestIndex)
        {
            lowestIndex = index;
        }
    }

    // If the lowest index, return it, otherwise, return -1 (error).
    if (lowestIndex != m_listData.size())
        return lowestIndex;
    else
        return -1;
}

void SelectionModel::clear()
{
    m_selectionOrder.clear();
    m_listData.clear();
}

void SelectionModel::clearSelection()
{
    // Clear all the selection.
    m_selectionOrder.clear();
    for (ListItem& item : m_listData)
    {
        item.isSelected = false;
    }

    // Notify the view of the modification.
    emit dataChanged(index(0), index(m_listData.size()-1));
}

void SelectionModel::setSelections(const QList<int>& indices) {
    for (int index : indices) {
        if (index < 0 || index > rowCount()-1) continue;

        m_listData[index].isSelected = true;
    }

    m_selectionOrder = indices;
}

void SelectionModel::selectAtIndex(int index)
{
    // Check if the index is valid.
    if (index >= 0 && index < m_listData.size())
    {
        // Clear all the selection.
        clearSelection();

        // Select the item at index (index).
        m_listData[index].isSelected = true;

        // Put the index into the selection order list.
        m_selectionOrder.append(index);

        // Notify the view.
        emit dataChanged(this->index(index), this->index(index), {IS_SELECTED});
    }
}

void SelectionModel::shiftSelectItem(int index)
{
    // Check if the index is valid.
    if (index >= 0 && index < m_listData.size())
    {
        // Getting the lowest selected index.
        int lowestIndex = lowestSelectedIndex();

        // If index lower than lowestIndex. Select from index to lowestIndex.
        if (index < lowestIndex)
        {
            clearSelection();
            for (int i = index; i <= lowestIndex; i++)
            {
                m_listData[i].isSelected = true;
                m_selectionOrder.append(i);
            }
            emit dataChanged(this->index(index), this->index(lowestIndex), {IS_SELECTED});
        }

        // If index greater than lowestIndex. Select from lowestIndex to index.
        else if (index > lowestIndex && lowestIndex != -1)
        {
            clearSelection();
            for (int i = lowestIndex; i <= index; i++)
            {
                m_listData[i].isSelected = true;
                m_selectionOrder.append(i);
            }
            emit dataChanged(this->index(lowestIndex), this->index(index), {IS_SELECTED});
        }
        // Otherwise, this item is already selected or lowestIndex is invalid. Don't do anything.
    }
}

void SelectionModel::ctrlSelectItem(int index)
{
    // Check if the index is valid.
    if (index >= 0 && index < m_listData.size())
    {
        // Swap isSelected of the item.
        m_listData[index].isSelected = !m_listData[index].isSelected;

        // Remove (if any) the index of the item from m_selectionOrder list.
        int listSortingIndex = m_selectionOrder.indexOf(index);
        if (listSortingIndex != -1)
        {
            m_selectionOrder.remove(listSortingIndex);
        }

        // If the item is selected, add the index of the item at the end of m_selectionOrder list.
        if (m_listData.at(index).isSelected)
        {
            m_selectionOrder.append(index);
        }

        // Notify the view of the change.
        emit dataChanged(this->index(index), this->index(index), {IS_SELECTED});
    }
}

const QList<int>& SelectionModel::selectedIndices() const
{
    return m_selectionOrder;
}

QVariantList SelectionModel::itemList() const
{
    return _itemList(false);
}

QVariant SelectionModel::item(int index) const
{
    // Check if index is valid.
    if (index >= 0 && index < m_listData.size())
    {
        return m_listData.at(index).item;
    }

    return QVariant();
}

QVariantList SelectionModel::selectedItemList() const
{
    return _itemList(true);
}

inline QVariantList SelectionModel::_itemList(bool onlySelected) const
{
    QVariantList variantList;

    // If onlySelected is false, resize variantList to the size of m_listData.
    if (!onlySelected)
    {
        variantList.resize(m_listData.size());
        
        // If onlySelected is false, retrieve all the items inside m_listData.
        for (int i = 0; i < m_listData.size(); i++)
        {
            variantList[i] = m_listData.at(i).item;
        }
    }

    // Otherwise, retrieve only the selected items in the right order.
    else
    {
        foreach (int index, m_selectionOrder)
        {
            // Check if the index is valid.
            if (index >= 0 && index < m_listData.size())
            {
                // Append the item into the variantList.
                variantList.append(m_listData.at(index).item);
            }
        }
    }

    return variantList;
}

void SelectionModel::setItemList(const QVariantList& variantList)
{
    // Clear everyting in the list.
    clear();
    
    // Resize m_listData to the size of variantList.
    m_listData.resize(variantList.size());

    // Copy data from variantList into m_listData.
    for (int i = 0; i < m_listData.size(); i++)
    {
        m_listData[i] = {false, variantList.at(i)};
    }
}

void SelectionModel::addItemList(const QVariantList& variantList)
{
    for (const QVariant variant : variantList)
    {
        m_listData.append({false, variant});
    }
}

void SelectionModel::removeSelectedItems()
{
    QList<int> selectedIndicesCopy(m_selectionOrder);

    std::sort(selectedIndicesCopy.begin(), selectedIndicesCopy.end(),
        [](int nb1, int nb2) -> bool {
            return nb1 > nb2;
        });

    for(int index : selectedIndicesCopy)
    {
        beginRemoveRows(QModelIndex(), index, index);
        m_listData.remove(index);
        endRemoveRows();
    }

    clearSelection();
}

bool SelectionModel::moveItemDown(int index) {
    // Check if index is out of range or at the last place.
    // If it is at the last index, it will go beyond scope.
    if (index < 0 || index >= rowCount()-1) {
        return false;
    }

    const auto temporaryItem = m_listData.at(index);
    m_listData[index] = m_listData.at(index+1);
    m_listData[index+1] = temporaryItem;

    return true;
}

bool SelectionModel::moveItemUp(int index) {
    // Check if index is out of range or at the first place.
    // If it is at the first place, it will go beyond scope.
    if (index <= 0 || index > rowCount()-1) {
        return false;
    }

    const auto temporaryItem = m_listData.at(index);
    m_listData[index] = m_listData.at(index-1);
    m_listData[index-1] = temporaryItem;
    return true;
}

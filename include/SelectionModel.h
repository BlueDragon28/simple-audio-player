#ifndef SIMPLEAUDIOPLAYER_SELECTIONMODEL_H_
#define SIMPLEAUDIOPLAYER_SELECTIONMODEL_H_

#include <QAbstractListModel>
#include <QVariant>
#include <QList>

/*
Handle selection.
Base class for all list base model class.
*/
class SelectionModel : public QAbstractListModel
{
    Q_OBJECT

public:
    SelectionModel(QObject* parent = nullptr);
    virtual ~SelectionModel();

    enum SelectionRole
    {
        IS_SELECTED = Qt::UserRole + 1, // If an element is selected.
        MAX_ROLE, // The children role will start from this enum value.
    };

    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QHash<int, QByteArray> roleNames() const override;

    /*
    Clear everything (the data and the selection list).
    */
    Q_INVOKABLE void clear();

    /*
    Clear the selection.
    */
    Q_INVOKABLE void clearSelection();

    /*
    Select an item and clear all other selection.
    */
    Q_INVOKABLE void selectAtIndex(int index);

    /*
    Called when the user selected an item with shift.
    */
    Q_INVOKABLE void shiftSelectItem(int index);

    /*
    Called when the user selected an item with ctrl.
    */
    Q_INVOKABLE void ctrlSelectItem(int index);

    /*
    Return the list of selected indices.
    */
    const QList<int>& selectedIndices() const;

    /*
    Return the items inside the list.
    */
    QVariantList itemList() const;

    /*
    Get an item at a specific index.
    */
    QVariant item(int index) const;

    /*
    Return the selected items inside the list.
    */
    QVariantList selectedItemList() const;

    /*
    Set the items list.
    */
    void setItemList(const QVariantList& variantList);

    /*
    Add items to the list.
    */
    void addItemList(const QVariantList& variantList);

protected:
    /*
    Return the lowest index inside m_selectionOrder list.
    */
    int lowestSelectedIndex() const;

private:
    /*
    Retrieve the items inside the list.
    onlySelected: if true, return only the selected items, if not, return the entire list.
    */
    inline QVariantList _itemList(bool onlySelected) const;

    /*
    Struct to hold the item selection property and the item itself.
    */
    struct ListItem
    {
        bool isSelected; // If the element is selected.
        QVariant item; // Item. A Variant to allow subclass to use whatever element type they want.
    };

    // Holding the data of the model.
    QList<ListItem> m_listData;

    // Order of the selection (holding the indices).
    QList<int> m_selectionOrder;
};

#endif // SIMPLEAUDIOPLAYER_SELECTIONMODEL_H_

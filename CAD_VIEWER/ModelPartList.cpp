/**     @file ModelPartList.h
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model part list that will be used to create the trewview.
  *
  *     P Evans 2022
  */

#include "ModelPartList.h"
#include "ModelPart.h"
#include <QAbstractItemModel>  // For QAbstractItemModel and related functionality
#include <QModelIndex>          // For QModelIndex handling
#include <QVariant>             // For QVariant, which is used in model data
#include <QList>                // For QList, if you're using it to store children in ModelPart
#include <QDebug>

/*!
 * \brief ModelPartList::ModelPartList
 * Constructor
 */
ModelPartList::ModelPartList( const QString& data, QObject* parent ) : QAbstractItemModel(parent) {
    /* Have option to specify number of visible properties for each item in tree - the root item
     * acts as the column headers
     */
    rootItem = new ModelPart( { tr("Part"), tr("Visible?"),tr("R"),tr("G"),tr("B"),tr("XCLIPMIN"),tr("XCLIPMAX"),tr("YCLIPMIN"),tr("YCLIPMAX"),tr("ZCLIPMIN"),tr("ZCLIPMAX"),tr("SIZE") });
}


/*!
 * \brief ModelPartList::~ModelPartList
 * Destructor
 */

ModelPartList::~ModelPartList() {
    delete rootItem;
}

/*!
 * \brief ModelPartList::columnCount
 * Counts the number of columns of children for a parent
 * \param parent the index
 * \return number of columns
 */
int ModelPartList::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED(parent);

    return rootItem->columnCount();
}

/*!
 * \brief ModelPartList::data
 * Returns the requested data from the selected index
 *
 * \param index The index of the item
 * \param role is what the data is used for
 * \return the column requested by the index
 */
QVariant ModelPartList::data( const QModelIndex& index, int role ) const {
    /* If the item index isnt valid, return a new, empty QVariant (QVariant is generic datatype
     * that could be any valid QT class) */
    if( !index.isValid() )
        return QVariant();

    /* Role represents what this data will be used for, we only need deal with the case
     * when QT is asking for data to create and display the treeview. Return a new,
     * empty QVariant if any other request comes through. */
    if (role != Qt::DisplayRole)
        return QVariant();

    /* Get a a pointer to the item referred to by the QModelIndex */
    ModelPart* item = static_cast<ModelPart*>( index.internalPointer() );

    /* Each item in the tree has a number of columns ("Part" and "Visible" in this 
     * initial example) return the column requested by the QModelIndex */
    return item->data( index.column() );
}

/*!
 * \brief ModelPartList::flags#
 * Returns the flags of the selected item
 * \param index the index of the item
 * \return the flags of the index
 */
Qt::ItemFlags ModelPartList::flags( const QModelIndex& index ) const {
    if( !index.isValid() )
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags( index );
}

/*!
 * \brief ModelPartList::headerData
 * returns header data if the conditions for the role and orientation are met
 * \param section the column
 * \param orientation the orientation of the header
 * \param role the requested data
 * \return the header data
 */
QVariant ModelPartList::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( orientation == Qt::Horizontal && role == Qt::DisplayRole )
        return rootItem->data( section );

    return QVariant();
}

/*!
 * \brief ModelPartList::index
 * Finds the desired item from the parent index
 * \param row The row number
 * \param column The column number
 * \param parent The parent index
 * \return the QModelIndex for the desired childItem
 */
QModelIndex ModelPartList::index(int row, int column, const QModelIndex& parent) const {
    ModelPart* parentItem;
    
    if( !parent.isValid() || !hasIndex(row, column, parent) )
        parentItem = rootItem;              // default to selecting root 
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    ModelPart* childItem = parentItem->child(row);
    if( childItem )
        return createIndex(row, column, childItem);
    
    
    return QModelIndex();
}

/*!
 * \brief ModelPartList::parent
 * The parent item's index is returned
 * \param index The index of the item
 * \return The QModelIndex of the parent
 */
QModelIndex ModelPartList::parent( const QModelIndex& index ) const {
    if (!index.isValid())
        return QModelIndex();

    ModelPart* childItem = static_cast<ModelPart*>(index.internalPointer());
    ModelPart* parentItem = childItem->parentItem();

    if( parentItem == rootItem )
        return QModelIndex();

    return createIndex( parentItem->row(), 0, parentItem );
}

/*!
 * \brief ModelPartList::rowCount
 * Counts the number of children the parent has
 * \param parent The parent index
 * \return The amount of children the parent item has
 */
int ModelPartList::rowCount( const QModelIndex& parent ) const {
    ModelPart* parentItem;
    if( parent.column() > 0 )
        return 0;

    if( !parent.isValid() )
        parentItem = rootItem;
    else
        parentItem = static_cast<ModelPart*>(parent.internalPointer());

    return parentItem->childCount();
}

/*!
 * \brief ModelPartList::getRootItem
 *  Returns the rootItem
 * \return the rootItem
 */
ModelPart* ModelPartList::getRootItem() {
    return rootItem; 
}


/*!
 * \brief ModelPartList::appendChild
 * A child item with data is being added to a  parent index
 * \param parent the parent index which the child item is being added to a parent index
 * \param data the information in the child item being added
 * \return
 */
QModelIndex ModelPartList::appendChild(QModelIndex& parent, const QList<QVariant>& data) {      
    ModelPart* parentPart;

    if (parent.isValid())
        parentPart = static_cast<ModelPart*>(parent.internalPointer());
    else {
        parentPart = rootItem;
        parent = createIndex(0, 0, rootItem );
    }

    beginInsertRows( parent, rowCount(parent), rowCount(parent) ); 

    ModelPart* childPart = new ModelPart( data, parentPart );

    parentPart->appendChild(childPart);

    QModelIndex child = createIndex(0, 0, childPart);

    endInsertRows();

    emit layoutChanged();

    return child;
}




/**     @file ModelPart.cpp
  *
  *     EEEE2076 - Software Engineering & VR Project
  *
  *     Template for model parts that will be added as treeview items
  *
  *     P Evans 2022
  */

#include "ModelPart.h"

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>



/*!
 * \brief ModelPart::ModelPart
 * Constructor
 */
ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {
}

/*!
 * \brief ModelPart::~ModelPart
 * Destructor
 */
ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}

/*!
 * \brief ModelPart::appendChild
 * Adds a child item to the parent item
 * \param item the child part
 */
void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}

/*!
 * \brief ModelPart::child
 * Returns a pointer to the child item
 * \param row the row number of the child item
 * \return A pointer to the child item if it is valid or nothing when invalid
 */
ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

/*!
 * \brief ModelPart::childCount
 * Counts the number of child items and returns the value
 * \return returns the number of child items
 */
int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}

/*!
 * \brief ModelPart::columnCount
 * Counts the number of columns
 * \return returns the number of columns
 */
int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

/*!
 * \brief ModelPart::data
 * Returns data from the given column
 * \param column the column's index
 * \return the data in the column
 */
QVariant ModelPart::data(int column) const {
    /* Return the data associated with a column of this item 
     *  Note on the QVariant type - it is a generic placeholder type
     *  that can take on the type of most Qt classes. It allows each 
     *  column or property to store data of an arbitrary type.
     */
    if (column < 0 || column >= m_itemData.size())
        return QVariant();
    return m_itemData.at(column);
}

/*!
 * \brief ModelPart::set
 *  The column is changed to another value
 * \param column the column's index
 * \param value a value
 */
void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}

/*!
 * \brief ModelPart::setName
 * Changes the name of the column
 * \param name a new name
 */
void ModelPart::setName(QString name){
    m_itemData.replace(0, name);
}

/*!
 * \brief ModelPart::parentItem
 * points to the parent item
 * \return the parent item
 */
ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}

/*!
 * \brief ModelPart::row
 * The row index of the child item is returned relative to it's parent's row index
 * \return  the row index of the specific item in respect to the parent's index
 */
int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

/*!
 * \brief ModelPart::setColour
 * Sets the rgb values
 * \param R the red component of the rgb values
 * \param G the green component of the rgb values
 * \param B the blue component of the rgb values
 */
void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    // Replace data in column 2,3,4 with the RGB vals
    m_itemData.replace(2, R);
    m_itemData.replace(3, G);
    m_itemData.replace(4, B);
}
/*!
 * \brief ModelPart::getColourR
 * Calls the red value of the rgb values of the part
 * \return the red value of the part
 */
unsigned char ModelPart::getColourR() {
    // Returns the R data val from column 2 
    return m_itemData.at(2).toInt();
}

/*!
 * \brief ModelPart::getColourG
 * Calls the green value of the rgb values of the part
 * \return the green value of the part
 */
unsigned char ModelPart::getColourG() {
    // Returns the B data val from column 3
    return m_itemData.at(3).toInt();
}

/*!
 * \brief ModelPart::getColourB
 * Calls the blue value of the rgb values of the part
 * \return the blue value of the part
 */
unsigned char ModelPart::getColourB() {
    // Returns the G data val from column 4 
    return m_itemData.at(4).toInt();
}
/*!
 * \brief ModelPart::setVisible
 * Sets the visibility for the part
 * \param isVisible A variable to show if it is visible or not
 */
void ModelPart::setVisible(bool isVisible) {
    // Replace data in column 1 with the vis boolean
    m_itemData.replace(1, isVisible);
}

/*!
 * \brief ModelPart::visible
 * Shows the visibility of the part
 * \return returns a true or false if it is visibile
 */
bool ModelPart::visible() {
    // Returns the visibilty from coloumn 1
    return m_itemData.at(1).toBool();
}

/*!
 * \brief ModelPart::loadSTL
 * Loads an STL file, sets up the name, the mapper an prepares it to be rendered
 * \param fileName
 */
void ModelPart::loadSTL( QString fileName ) {
    
    // 1. Use the vtkSTLReader class to load the STL file 

    file = vtkNew<vtkSTLReader>();
    file->SetFileName(fileName.toLocal8Bit());

    /* 2. Initialise the part's vtkMapper */
    mapper = vtkNew<vtkPolyDataMapper>();
    mapper->SetInputConnection(file->GetOutputPort());


    /* 3. Initialise the part's vtkActor and link to the mapper */
    actor = vtkNew<vtkActor>();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1, 0, 0.35);

    
}
/*!
 * \brief ModelPart::getActor
 * It returns the vtk actor of the part
 * \return the actor
 */
vtkSmartPointer<vtkActor> ModelPart::getActor() {
    // Returns the actor of the part
    return actor;
}
//vtkActor* ModelPart::getNewActor() {
    /* This is a placeholder function that you will need to modify if you want to use it
     * 
     * The default mapper/actor combination can only be used to render the part in 
     * the GUI, it CANNOT also be used to render the part in VR. This means you need
     * to create a second mapper/actor combination for use in VR - that is the role
     * of this function. */
     
     
     /* 1. Create new mapper */
     
     /* 2. Create new actor and link to mapper */
     
     /* 3. Link the vtkProperties of the original actor to the new actor. This means 
      *    if you change properties of the original part (colour, position, etc), the
      *    changes will be reflected in the GUI AND VR rendering.
      *    
      *    See the vtkActor documentation, particularly the GetProperty() and SetProperty()
      *    functions.
      */
    

    /* The new vtkActor pointer must be returned here */
//    return nullptr;
    
//}


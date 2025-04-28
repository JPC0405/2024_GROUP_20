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





ModelPart::ModelPart(const QList<QVariant>& data, ModelPart* parent )
    : m_itemData(data), m_parentItem(parent) {
}


ModelPart::~ModelPart() {
    qDeleteAll(m_childItems);
}


void ModelPart::appendChild( ModelPart* item ) {
    /* Add another model part as a child of this part
     * (it will appear as a sub-branch in the treeview)
     */
    item->m_parentItem = this;
    m_childItems.append(item);
}


ModelPart* ModelPart::child( int row ) {
    /* Return pointer to child item in row below this item.
     */
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int ModelPart::childCount() const {
    /* Count number of child items
     */
    return m_childItems.count();
}


int ModelPart::columnCount() const {
    /* Count number of columns (properties) that this item has.
     */
    return m_itemData.count();
}

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


void ModelPart::set(int column, const QVariant &value) {
    /* Set the data associated with a column of this item 
     */
    if (column < 0 || column >= m_itemData.size())
        return;

    m_itemData.replace(column, value);
}


void ModelPart::setName(QString name){
    m_itemData.replace(0, name);
}

ModelPart* ModelPart::parentItem() {
    return m_parentItem;
}


int ModelPart::row() const {
    /* Return the row index of this item, relative to it's parent.
     */
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<ModelPart*>(this));
    return 0;
}

void ModelPart::setColour(const unsigned char R, const unsigned char G, const unsigned char B) {
    // Replace data in column 2,3,4 with the RGB vals
    m_itemData.replace(2, R);
    m_itemData.replace(3, G);
    m_itemData.replace(4, B);
}

unsigned char ModelPart::getColourR() {
    // Returns the R data val from column 2 
    return m_itemData.at(2).toInt();
}

unsigned char ModelPart::getColourG() {
    // Returns the B data val from column 3
    return m_itemData.at(3).toInt();
}


unsigned char ModelPart::getColourB() {
    // Returns the G data val from column 4 
    return m_itemData.at(4).toInt();
}


void ModelPart::setVisible(bool isVisible) {
    // Replace data in column 1 with the vis boolean
    m_itemData.replace(1, isVisible);
}

bool ModelPart::visible() {
    // Returns the visibilty from coloumn 1
    return m_itemData.at(1).toBool();
}

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


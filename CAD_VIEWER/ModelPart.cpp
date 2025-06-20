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
#include <vtkProperty.h>
#include <vtkProperty.h>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QDebug>








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

void ModelPart::setClip(float minX, float maxX,float minY, float maxY,float minZ, float maxZ){
    m_itemData.replace(5,minX);
    m_itemData.replace(6,maxX);
    m_itemData.replace(7,minY);
    m_itemData.replace(8,maxY);
    m_itemData.replace(9,minZ);
    m_itemData.replace(10,maxZ);
}
float ModelPart::getMinX(){
    return m_itemData.at(5).toFloat();
}
float ModelPart::getMaxX(){
    return m_itemData.at(6).toFloat();
}
float ModelPart::getMinY(){
    return m_itemData.at(7).toFloat();
}
float ModelPart::getMaxY(){
    return m_itemData.at(8).toFloat();
}
float ModelPart::getMinZ(){
    return m_itemData.at(9).toFloat();
}
float ModelPart::getMaxZ(){
    return m_itemData.at(10).toFloat();
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


float ModelPart::getSize(){
    qDebug()<<"6 got size: "<<m_itemData.at(11).toFloat();
    return m_itemData.at(11).toFloat();
}

void ModelPart::setMapper(vtkSmartPointer<vtkDataSetMapper> inputMapper) {
    if (mapper)
    {
        mapper = inputMapper;
    }
    mapper->Update();
}

vtkSmartPointer<vtkMapper> ModelPart::getMapper() {
    // Returns the G data val from column 4 
    return mapper;
}


void ModelPart::setSize(float size){
    m_itemData.replace(11,size);
    qDebug()<<"7 set size: "<<size;
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
    file->Update();

    vtkSmartPointer<vtkPolyData> inputPolyData = file->GetOutput();
    double bounds[6];
    inputPolyData->GetBounds(bounds);
    qDebug() << "Model Bounds:"
             << "X: [" << bounds[0] << "," << bounds[1] << "]"
             << "Y: [" << bounds[2] << "," << bounds[3] << "]"
             << "Z: [" << bounds[4] << "," << bounds[5] << "]";
    qDebug() << "Input Points:" << inputPolyData->GetNumberOfPoints();




    /* 2. Initialise the part's vtkMapper */
    mapper = vtkNew<vtkDataSetMapper>();

    //mapper->SetInputConnection(file->GetOutputPort());
    mapper->SetInputConnection(file->GetOutputPort( ));
    mapper = applyClip();


    /* 3. Initialise the part's vtkActor and link to the mapper */
    actor = vtkNew<vtkActor>();
    actor->SetMapper(mapper);
    actor->GetProperty()->SetColor(1., 0., 0.35);

}

vtkSmartPointer<vtkDataSetMapper> ModelPart::applyClip(){//new function for clipping
    vtkSmartPointer<vtkPlane> planeLeft = vtkSmartPointer<vtkPlane>::New ( ) ;//creates plane to hide parts of the model at coordinates x<getMinX()
    
    if (file)
    {
        vtkSmartPointer<vtkPolyData> inputPolyData = file->GetOutput();//gets the model

        double bounds[6];//creates array
        inputPolyData->GetBounds(bounds);//stores the bounds in the array - [lowest x coord, highest x coord, lowest y coord, highest y coord, lowest z coord, highest z coord]

        qDebug() << "Min X=" << getMinX();
        //SetOrigin(X,Y,Z)
        float lowerX = bounds[0] + (getMinX() / 100.0) * (bounds[1] - bounds[0]);//uses the result from getMinX() as the proportion of the model to be cut off - e.g. if getMinX() returns 20, the first 20% of the model will be clipped
        planeLeft->SetOrigin(lowerX, 0.0, 0.0);//sets the origin of the plane (the first X coordinate to be shown in the display)
        planeLeft->SetNormal(1.0, 0.0, 0.0);//sets the direction of the plane to the positive X direction, so x coordinates higher than the given are showed

        vtkSmartPointer<vtkClipPolyData> clipFilterL = vtkSmartPointer<vtkClipPolyData >::New();
        clipFilterL->SetInputConnection(file->GetOutputPort());
        clipFilterL->SetClipFunction(planeLeft.Get());//these lines are for creating the actual model that has been clipped

        // Set up the second clipping plane - code is the same as above but for different clips
        vtkSmartPointer<vtkPlane> planeRight = vtkSmartPointer<vtkPlane>::New();
        qDebug() << "Max X=" << getMaxX();
        float upperX = bounds[0] + (getMaxX() / 100.0) * (bounds[1] - bounds[0]);
        planeRight->SetOrigin(upperX, 0.0, 0.0);
        planeRight->SetNormal(-1.0, 0.0, 0.0); // Normal points along -x (keeps left side)

        vtkSmartPointer<vtkClipPolyData> clipFilterR = vtkSmartPointer<vtkClipPolyData>::New();
        clipFilterR->SetInputConnection(clipFilterL->GetOutputPort());
        clipFilterR->SetClipFunction(planeRight.Get());

        // Set up the third clipping plane
        vtkSmartPointer<vtkPlane> planeLowerY = vtkSmartPointer<vtkPlane>::New();
        qDebug() << "Min Y=" << getMinY();
        float lowerY = bounds[2] + (getMinY() / 100.0) * (bounds[3] - bounds[2]);
        planeLowerY->SetOrigin(0, lowerY, 0.0);
        planeLowerY->SetNormal(0., 1.0, 0.0); // Normal points along y

        vtkSmartPointer<vtkClipPolyData> clipFilterLowY = vtkSmartPointer<vtkClipPolyData>::New();
        clipFilterLowY->SetInputConnection(clipFilterR->GetOutputPort());
        clipFilterLowY->SetClipFunction(planeLowerY.Get());

        // Set up the fourth clipping plane
        vtkSmartPointer<vtkPlane> planeUpperY = vtkSmartPointer<vtkPlane>::New();
        qDebug() << "Max Y=" << getMaxY();
        float upperY = bounds[2] + (getMaxY() / 100.0) * (bounds[3] - bounds[2]);
        planeUpperY->SetOrigin(0, upperY, 0.0);
        planeUpperY->SetNormal(0, -1.0, 0.0); // Normal points along -y

        vtkSmartPointer<vtkClipPolyData> clipFilterUpY = vtkSmartPointer<vtkClipPolyData>::New();
        clipFilterUpY->SetInputConnection(clipFilterLowY->GetOutputPort());
        clipFilterUpY->SetClipFunction(planeUpperY.Get());

        // Set up the fifth clipping plane
        vtkSmartPointer<vtkPlane> planeLowerZ = vtkSmartPointer<vtkPlane>::New();
        qDebug() << "Min Z=" << getMinZ();
        float lowerZ = bounds[4] + (getMinZ() / 100.0) * (bounds[5] - bounds[4]);
        planeLowerZ->SetOrigin(0., 0., lowerZ);
        planeLowerZ->SetNormal(0., 0., 1); // Normal points along z

        vtkSmartPointer<vtkClipPolyData> clipFilterLowZ = vtkSmartPointer<vtkClipPolyData>::New();
        clipFilterLowZ->SetInputConnection(clipFilterUpY->GetOutputPort());
        clipFilterLowZ->SetClipFunction(planeLowerZ.Get());

        // Set up the sixth clipping plane
        vtkSmartPointer<vtkPlane> planeUpperZ = vtkSmartPointer<vtkPlane>::New();
        qDebug() << "Max Z=" << getMaxZ();
        float upperZ = bounds[4] + (getMaxZ() / 100.0) * (bounds[5] - bounds[4]);
        planeUpperZ->SetOrigin(0, 0., upperZ);
        planeUpperZ->SetNormal(0, 0., -1); // Normal points along -z

        vtkSmartPointer<vtkClipPolyData> clipFilterUpZ = vtkSmartPointer<vtkClipPolyData>::New();
        clipFilterUpZ->SetInputConnection(clipFilterLowZ->GetOutputPort());
        clipFilterUpZ->SetClipFunction(planeUpperZ.Get());

        vtkSmartPointer<vtkShrinkFilter>shrinkFilter = vtkSmartPointer<vtkShrinkFilter>::New();
        shrinkFilter->SetInputConnection(clipFilterUpZ->GetOutputPort());
        shrinkFilter->SetShrinkFactor(getSize() / 100);
        qDebug() << "Size: " << getSize() << "%";
        // shrinkFilter->Update();

        vtkSmartPointer<vtkDataSetMapper> newMapper = vtkSmartPointer<vtkDataSetMapper>::New();
        //mapper->SetInputConnection(file->GetOutputPort());
        newMapper->SetInputConnection(shrinkFilter->GetOutputPort());

        return newMapper;
    }
}

/*!
 * \brief ModelPart::getActor
 * It returns the vtk actor of the part
 * \return the actor
 */
vtkSmartPointer<vtkActor> ModelPart::getActor() {

    if (!actor) {
        qDebug() << "Error: Actor is null in getActor.";
        actor = vtkNew<vtkActor>();
    }
    if (!mapper) {
        qDebug() << "Warning: Mapper is null in getActor.";
        mapper = vtkNew<vtkDataSetMapper>();
        if (file && file->GetOutput()) {
            mapper->SetInputConnection(file->GetOutputPort());
        } else {
            vtkSmartPointer<vtkPolyData> emptyData = vtkSmartPointer<vtkPolyData>::New();//need to use empty data to avoid pipeline errors
            mapper->SetInputDataObject(emptyData);
            qDebug() << "Error: No valid file output for mapper.";
        }
    }
    actor->SetMapper(mapper);
    return actor;
}

//vtkActor* ModelPart::getNewActor() {




vtkSmartPointer<vtkActor> ModelPart::getNewActor() {

    /* This is a placeholder function that you will need to modify if you want to use it
     *
     * The default mapper/actor combination can only be used to render the part in
     * the GUI, it CANNOT also be used to render the part in VR. This means you need
     * to create a second mapper/actor combination for use in VR - that is the role
     * of this function. */

     
     /* 1. Create new mapper */

    vtkNew<vtkPolyDataMapper> newMapper;
    newMapper->SetInputConnection(file->GetOutputPort());

     
     /* 2. Create new actor and link to mapper */

    vtkActor* newActor = vtkActor::New();
    newActor->SetMapper(newMapper);
     
     /* 3. Link the vtkProperties of the original actor to the new actor. This means
      *    if you change properties of the original part (colour, position, etc), the
      *    changes will be reflected in the GUI AND VR rendering.
      *
      *    See the vtkActor documentation, particularly the GetProperty() and SetProperty()
      *    functions.
      */
    

    newActor->SetProperty(this->getActor()->GetProperty());


    /* The new vtkActor pointer must be returned here */
    return newActor;

    //test
    
    // Test
}


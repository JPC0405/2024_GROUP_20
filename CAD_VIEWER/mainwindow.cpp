/*! @file mainwindow.cpp
 *
 *  Contains the functions to display STL file, able to change variables and to see the changes on the ui
 *  It also includes functions to allow it to be displayed on vr
 *
 *  Jay Chauhan, Charles Egan and Jacob Moore 2025
 */

#include "mainwindow.h"
#include "VRRenderThread.h"
#include "./ui_mainwindow.h"
#include "optiondialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDialog>
 #include <QTreeWidgetItemIterator>
#include <vtkrenderWindow.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkProperty.h>


/*!
 * \brief MainWindow::MainWindow
 * It constructs the main window
 * It sets up the ui with the widget and vtk rendererand adds actions to interact with the ui
 * Connects slot and signals to the ui and creates a treeview with a part list
 *
 * \param parent represents the parent widget of the main window
 */




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Setup ui with widgets and VTK renderer
    ui->setupUi(this);
    ui->treeView->addAction(ui->actionItems_Options);
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    
    // Render Initial cylinder in render window
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor(1., 0., 0.35);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);
    
    // Modify camera to focus render
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    // Connecting Slots and signals of UI elements
    connect( ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton );
    connect( ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClick );
    connect(this, &MainWindow::statusUpdateMessage,ui->statusbar, &QStatusBar::showMessage);

    // Instatiate a tree view with a part list 
    this->partList = new ModelPartList("PartsList");
    ui->treeView->setModel(this->partList);
    ModelPart *rootItem = this->partList->getRootItem();

    // Instantiates the root item "Model" into the part list and tree view
    QString name = QString("Model").arg(1);
    QString visible("true");
    qint64 R(255);
    qint64 G(0);
    qint64 B(90);

    ModelPart* childItem = new ModelPart({ name,visible,R,G,B, 0., 100., 0., 100., 0., 100., 100 });
    rootItem->appendChild(childItem);

    /* Test to check if tree view works
    for (int i =0; i<3; i++){
        QString name = QString("TopLevel %1").arg(1);
        QString visible("true");
        qint64 R(0);
        qint64 G(0);
        qint64 B(0);

        ModelPart *childItem = new ModelPart({name,visible,R,G,B});

        rootItem->appendChild(childItem);

        for (int j=0;j<5;j++){
            QString name = QString("Item %1,%2").arg(i).arg(j);
            QString visible("true");
            qint64 R(0);
            qint64 G(0);
            qint64 B(0);



            ModelPart *childChildItem = new ModelPart({name , visible,R,G,B});

            childItem->appendChild(childChildItem);
        }
    }
    */

    
}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}


/*!
 * \brief MainWindow::handleButton
 *  Emits a message that the button has been pressed to the status bar
 */
void MainWindow::handleButton(){
    //QMessageBox msgBox;
    //msgBox.setText("Add button was clicked");
    //msgBox.exec();
    emit statusUpdateMessage( QString("Add button was clicked"), 0);
}

/*!
 * \brief MainWindow::handleTreeClick
 * Emits a message saying which part of the treeview was clicked
 */

void MainWindow::handleTreeClick(){
    //Select the part clicked in the tree view
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();

    // Update the status bar with the name of the model part
    emit statusUpdateMessage(QString("The selected item is: ")+text,0);
}


/*!
 * \brief MainWindow::on_actionOpen_File_triggered
 * When the open file action is triggered, a message is emitted to the staus bar and dialog box is opened to choose a STL or txt file
 * The STL file is given standard parameters and then is rendered
 */
void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage(QString("Open File action triggered"),0);

    // Open a dialog box to select STL or text files
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Open File"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));
=======
void MainWindow::handleButton(){
    QMessageBox msgBox;


    //msgBox.exec();

    if (VR_ON==0)
    {
        VR_ON=1;
        VRthread = new VRRenderThread();
        QModelIndex index = ui->treeView->currentIndex();
        AddVRActors(index,VRthread);
        VRthread->start();
        emit statusUpdateMessage(QString("VR Renderer Started"),0);
    }
    else
    {
        emit statusUpdateMessage(QString("VR Renderer already running"),0);
    }
}



void MainWindow::on_pushButton_3_clicked()
{
   if(VR_ON==1)
   {
        VRthread->issueCommand(0, 0);
        VR_ON = 0;
        emit statusUpdateMessage(QString("VR Renderer closed"), 0);
    }

    else
    {
        emit statusUpdateMessage(QString("No VR Renderer running"),0);
    }
}


/*!
 * \brief MainWindow::on_pushButton_2_clicked
 * A dialog is opened and the name, visibility, RGB values are determined
 * When the buuton is pressed, the dialog is accepted and the values are stored and applied
 */

void MainWindow::on_pushButton_2_clicked()
{
    // Open dialog window
    OptionDialog dialog(this);

    // Access currently selected model part
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

   // Get data from selected part
    qDebug()<<"getting data from selected part";
    QString name = selectedPart->data(0).toString();
    bool vis = selectedPart->data(1).toBool();
    qint64 R = selectedPart->getColourR();
    qint64 G = selectedPart->getColourG();
    qint64 B = selectedPart->getColourB();
    float xmin = selectedPart->getMinX();
    float xmax = selectedPart->getMaxX();
    float ymin = selectedPart->getMinY();
    float ymax = selectedPart->getMaxY();
    float zmin = selectedPart->getMinZ();
    float zmax = selectedPart->getMaxZ();
    float size = selectedPart->getSize();
    qDebug()<<"got data from selected part";

    // Set accessed data in dialog box
    qDebug()<<"setting data in dialog box";
    dialog.setVisibility(vis);
    dialog.set_name(name);
    dialog.set_R(R);
    dialog.set_G(G);
    dialog.set_B(B);
    dialog.set_Clip(xmin, xmax, ymin, ymax, zmin, zmax);
    dialog.setSize(size);
    qDebug()<<"3 Set size: "<<size;
    qDebug()<<"set data in dialog box";

    // if the accept button is pressed
    if (dialog.exec() == QDialog::Accepted){
        emit statusUpdateMessage(QString("Dialog accepted"), 0);


        // use get functions in dialog to get users choice
        qDebug()<<"getting user choice";
        bool n_vis = dialog.getVisibility();
        QString n_name = dialog.get_name();
        double n_R = dialog.get_R();
        double n_G = dialog.get_G();
        double n_B = dialog.get_B();
        float minX = dialog.get_MinX();
        float maxX = dialog.get_MaxX();
        float minY = dialog.get_MinY();
        float maxY = dialog.get_MaxY();
        float minZ = dialog.get_MinZ();
        float maxZ = dialog.get_MaxZ();
        float sizeF = dialog.getSize();
        qDebug()<<"4 got size: "<<sizeF;
        qDebug()<<"got user choice";

        // update the selected item
        qDebug()<<"updating selected item";
        selectedPart->setVisible(n_vis);
        selectedPart->setName(n_name);
        selectedPart->setColour(n_R,n_G,n_B);
        selectedPart->setClip(minX,maxX,minY,maxY,minZ,maxZ);
        selectedPart->setSize(sizeF);
        selectedPart->setMapper(selectedPart->applyClip());

        qDebug()<<"5 set size: "<<sizeF;
        qDebug()<<"updated selected item";


        // if an actor for the model part exists
        if (selectedPart->getActor()) {
            // Set the colour and visibility
            selectedPart->getActor()->GetProperty()->SetColor(n_R / 255, n_G / 255, n_B / 255);
            selectedPart->getActor()->SetVisibility(n_vis);
        }


        //update child items
        updateChildren(selectedPart, vis, n_R, n_G, n_B);

        
    }

    // if cancel button is clicked
    else{
        emit statusUpdateMessage(QString("Dialog rejected"),0);
    }
}

void MainWindow::on_actionItems_Options_triggered()
{


/*!
 * \brief MainWindow::on_actionItems_Options_triggered
 * A message is emitted to the status bar for which action is selected
 */
void MainWindow::on_actionItems_Options_triggered()

    // Open dialog window
    OptionDialog dialog(this);

    // Access currently selected model part
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Get data from selected part
    QString name = selectedPart->data(0).toString();
    bool vis = selectedPart->data(1).toBool();
    qint64 R = selectedPart->getColourR();
    qint64 G = selectedPart->getColourG();
    qint64 B = selectedPart->getColourB();

    // Set accessed data in dialog box
    dialog.setVisibility(vis);
    dialog.set_name(name);
    dialog.set_R(R);
    dialog.set_G(G);
    dialog.set_B(B);

    // if the accept button is pressed
    if (dialog.exec() == QDialog::Accepted){
        emit statusUpdateMessage(QString("Dialog accepted"), 0);


        // use get functions in dialog to get users choice
        bool n_vis = dialog.getVisibility();
        QString n_name = dialog.get_name();
        double n_R = dialog.get_R();
        double n_G = dialog.get_G();
        double n_B = dialog.get_B();

        // update the selected item
        selectedPart->setVisible(n_vis);
        selectedPart->setName(n_name);
        selectedPart->setColour(n_R,n_G,n_B);

        // if an actor for the model part exists
        if (selectedPart->getActor()) {
            // Set the colour and visibility
            selectedPart->getActor()->GetProperty()->SetColor(n_R / 255, n_G / 255, n_B / 255);
            selectedPart->getActor()->SetVisibility(n_vis);
        }


        //update child items
        updateChildren(selectedPart, vis, n_R, n_G, n_B);

    }

    // if cancel button is clicked
    else{
        emit statusUpdateMessage(QString("Dialog rejected"),0);
    }
}

void MainWindow::on_actionOpen_File_triggered()
{
    emit statusUpdateMessage(QString("Open File action triggered"),0);

    // Open a dialog box to select STL or text files
    QStringList fileNames = QFileDialog::getOpenFileNames(
        this,
        tr("Open Files"),
        "C:\\",
        tr("STL Files(*.stl);;Text Files(*.txt)"));

    //emit statusUpdateMessage(QString(fileName),0);

    // for all items selected in the file directory
    for (int i=0;i<fileNames.size();i++)
    {
        // Create a new model part item with default perameters and append it to the tree
        QString visible("true");
        qint64 R(255);
        qint64 G(0);
        qint64 B(90);

        ModelPart* childItem = new ModelPart({ fileNames[i].section('/', -1),visible,R,G,B, 0.,100.,0.,100.,0.,100.,100});
        QModelIndex index = ui->treeView->currentIndex();
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        selectedPart->appendChild(childItem);

        // Load the selected STL file and update status bar
        childItem->loadSTL(fileNames[i]);
        emit statusUpdateMessage(QString("Loaded STL File"+QString(fileNames[i])), 0);

        // Add the loaded STL file to the renderer
        renderer->AddActor(childItem->getActor());

        // Update the render to show new model
        updateRender();
    }
}


/*!
 * \brief MainWindow::UpdateRenderFromTree
 * Updates the renderer when a valid index is passed by adding the actor for the selected part
 * \param index the index from the ModelPart
 */




void MainWindow::UpdateRenderFromTree(const QModelIndex& index) {

    //if a a valid index is passed
    if (index.isValid()) {
        // Add the actor for the selected part to the renderer
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        renderer->AddActor(selectedPart->getActor());
        ;
    }

    // if no children exist for the passed item
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
    {
        return;
    }

    else {
        // Count number of rows in the tree
        int rows = partList->rowCount(index);

        // for each item in the tree recursively run this function
        for (int i = 0; i < rows; i++)
        {
            UpdateRenderFromTree(partList->index(i, 0, index));
        }
    }
}


/*!
 * \brief MainWindow::updateChildren
 * Updates the children of the selected parent being updated
 * This contains the visibility value and the rgb values of the parent
 * \param parent the parent of the model part being updated
 * \param vis the visibility value
 * \param r the amount of red
 * \param g the amount of green
 * \param b the amount of blue
 */


void MainWindow::updateChildren(ModelPart* parent, bool vis, double r, double g, double b)
{
    // for the number of children of the passed item
    for (int i = 0; i < parent->childCount(); i++)
    {
        // access the child to change colour and visiblilty
        ModelPart* childPart = parent->child(i);
        childPart->setVisible(vis);
        childPart->setColour(r,g,b);


        // if the model part has an actor set colour and visibility
        if (childPart->getActor())
        {
            childPart->getActor()->GetProperty()->SetColor(r / 255, g / 255, b / 255);
            childPart->getActor()->SetVisibility(vis);



        }

        // Recursivly run this function for any children of this model part
        updateChildren(childPart, vis, r, g, b);
    }
}


/*!
 * \brief MainWindow::updateRender
 * Refreshes the renderer so all actors are set to default
 */
  
void MainWindow::updateRender() {
    // Remove all actors from render window
    renderer->RemoveAllViewProps();

    //add all actors to render window and render
    UpdateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->Render();

    // Reset the camera
    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    // VRthread->issueCommand(4,0);
    // QModelIndex index = ui->treeView->currentIndex();
    // AddVRActors(index,VRthread);
    // VRthread->issueCommand(5,0);
}


void MainWindow::AddVRActors(const QModelIndex& index,VRRenderThread* thread) {

    //if a a valid index is passed
    if (index.isValid()) {
        // Add the actor for the selected part to the vr render thread
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        if(selectedPart->getActor())
        {
            thread->addActorOffline(selectedPart->getNewActor());
        }

    }
conflicts 
    // if no children exist for the passed item
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren))
    {
        return;
    }

    else {
        // Count number of rows in the tree
        int rows = partList->rowCount(index);

        // for each item in the tree recursively run this function
        for (int i = 0; i < rows; i++)
        {
            AddVRActors(partList->index(i, 0, index),thread);
        }
    }
}







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
#include <vtkLight.h>


//main window thing:
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

    vtkSmartPointer<vtkLight> light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    light->SetPosition(5, 5, 15);
    light->SetPositional(true);
    light->SetConeAngle(10);
    light->SetFocalPoint(0, 0, 0);
    light->SetDiffuseColor(1, 1, 1);
    light->SetAmbientColor(1, 1, 1);
    light->SetSpecularColor(1, 1, 1);
    light->SetIntensity(0.5);

    renderer->AddLight( light );

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

    ModelPart* childItem = new ModelPart({ name,visible,R,G,B });
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

    VRthread = NULL;

}

// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleTreeClick(){
    //Select the part clicked in the tree view
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();

    // Update the status bar with the name of the model part
    emit statusUpdateMessage(QString("The selected item is: ")+text,0);
}

void MainWindow::handleButton(){
    QMessageBox msgBox;

    //msgBox.exec();

    if (VR_ON==0)
    {
        VR_ON=1;
        VRthread = new VRRenderThread();
        QModelIndex index = ui->treeView->currentIndex();
        AddVRActors(index);
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

void MainWindow::on_pushButton_2_clicked()
{
    // Open dialog window
    OptionDialog dialog(this);

    // Access currently selected model part
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());

    if (!selectedPart) return;

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

void MainWindow::on_actionItems_Options_triggered()
{

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
        QFileInfo fileInfo(fileNames[i]);
        QString fileExtension = fileInfo.suffix().toLower();
        if (fileExtension == "png")//checks if the loaded file is a png (skybox)
        {
            // Load the (single cross-layout) PNG
            vtkNew<vtkPNGReader> reader;
            reader->SetFileName(fileNames[i].toStdString().c_str());
            reader->Update();

            // Verify dimensions - makes sure the image is the right resolution
            int* extent = reader->GetOutput()->GetExtent();
            int width = extent[1] - extent[0] + 1;
            int height = extent[3] - extent[2] + 1;
            if (width != 1024 || height != 768)
            {
                emit statusUpdateMessage(QString("Error: PNG must be 1024x768 for 4x3 cross layout"), 0);
                continue;
            }

            // Face size is 256x256 (1024/4 = 256, 768/3 = 256)
            int faceSize = 256;

            // Define the six faces' extents in the cross layout - "refolds" the faces on the skybox
            int faceExtents[6][4] = {
                {512, 768, 256, 512},  // Right (Positive X)
                {0, 256, 256, 512},    // Left (Negative X)
                {256, 512, 0, 256},    // Top (Positive Y)
                {256, 512, 512, 768},  // Bottom (Negative Y)
                {256, 512, 256, 512},  // Front (Positive Z)
                {768, 1024, 256, 512}  // Back (Negative Z)
            };

            // Create cubemap texture
            vtkNew<vtkTexture> texture;
            texture->CubeMapOn();
            for (int j = 0; j < 6; j++)
            {
                vtkNew<vtkImageClip> clip;
                clip->SetInputConnection(reader->GetOutputPort());
                clip->SetOutputWholeExtent(faceExtents[j][0], faceExtents[j][1] - 1, faceExtents[j][2], faceExtents[j][3] - 1, 0, 0);
                clip->ClipDataOn();
                clip->Update();

                texture->SetInputConnection(j, clip->GetOutputPort());
            }
            texture->InterpolateOn();
            texture->MipmapOn();

            // Create and configure skybox
            skyboxActor = vtkSmartPointer<vtkSkybox>::New(); // Store in member variable
            skyboxActor->SetTexture(texture);
            skyboxActor->SetProjection(vtkSkybox::Cube);

            // Add skybox to renderer
            renderer->AddActor(skyboxActor);
            renderer->SetBackground(0.0, 0.0, 0.0);
            renderer->GetActiveCamera()->SetClippingRange(1.0, 10000.0);
            renderer->ResetCameraClippingRange();

            emit statusUpdateMessage(QString("Skybox added using cross-layout PNG: ") + fileNames[i], 0);
        }
        else{
        // Create a new model part item with default perameters and append it to the tree
        QString visible("true");
        qint64 R(255);
        qint64 G(0);
        qint64 B(90);

        ModelPart* childItem = new ModelPart({ fileNames[i].section('/', -1),visible,R,G,B });
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

    
    if (VR_ON == 1)
    {
        if (VRthread) {
            //VRthread->issueCommand(4, 0);
            QModelIndex index = ui->treeView->currentIndex();
            AddVRActors(index);
            //VRthread->issueCommand(5, 0);
        }
    }
    

}


void MainWindow::AddVRActors(const QModelIndex& index) {

    //if a a valid index is passed
    if (index.isValid()) {
        // Add the actor for the selected part to the vr render thread
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

        if(selectedPart->getActor())
        {
            VRthread->addActorOffline(selectedPart->getNewActor());
        }

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
            AddVRActors(partList->index(i, 0, index));
        }
    }
}







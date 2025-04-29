#include "mainwindow.h"
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

/* Vtk headers */
#include <vtkActor.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRCamera.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkNamedColors.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkSTLReader.h>
#include <vtkDataSetmapper.h>
#include <vtkCallbackCommand.h>


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

}


// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton(){
    QMessageBox msgBox;
    emit statusUpdateMessage(QString("VR Renderer Started"),0);
    //msgBox.exec();
    VRRenderThread *VRThread = new VRRenderThread();
    VRThread->start();

    QModelIndex index = ui->treeView->currentIndex();
    AddVRActors(index,VRThread);


}

void MainWindow::handleTreeClick(){
    //Select the part clicked in the tree view
    QModelIndex index = ui->treeView->currentIndex();
    ModelPart *selectedPart = static_cast<ModelPart*>(index.internalPointer());
    QString text = selectedPart->data(0).toString();

    // Update the status bar with the name of the model part
    emit statusUpdateMessage(QString("The selected item is: ")+text,0);
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


void MainWindow::on_pushButton_2_clicked()
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
void MainWindow::on_actionItems_Options_triggered()
{
    //emit statusUpdateMessage(QString("Test action selected"),0);

}

void MainWindow::UpdateRenderFromTree(const QModelIndex& index) {

    //if a a valid index is passed
    if (index.isValid()) {
        // Add the actor for the selected part to the renderer
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        renderer->AddActor(selectedPart->getActor());
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

}


void MainWindow::AddVRActors(const QModelIndex& index,VRRenderThread* thread) {

    //if a a valid index is passed
    if (index.isValid()) {
        // Add the actor for the selected part to the renderer
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        thread->addActorOffline(selectedPart->getActor());

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
            AddVRActors(partList->index(i, 0, index),thread);
        }
    }
}


/* The class constructor is called by MainWindow and runs in the primary program thread, this thread
 * will go on to handle the GUI (mouse clicks, etc). The OpenVRRenderWindowInteractor cannot be start()ed
 * in the constructor, as it will take control of the main thread to handle the VR interaction (headset
 * rotation etc. This means that a second thread is needed to handle the VR.
 */
VRRenderThread::VRRenderThread( QObject* parent ) {
    /* Initialise actor list */
    actors = vtkActorCollection::New();

    /* Initialise command variables */
    rotateX = 0.;
    rotateY = 0.;
    rotateZ = 0.;
}


/* Standard destructor - this is important here as the class will be destroyed when the user
 * stops the VR thread, and recreated when the user starts it again. If class variables are
 * not deallocated properly then there will be a memory leak, where the program's total memory
 * usage will increase for each start/stop thread cycle.
 */
VRRenderThread::~VRRenderThread() {

}


void VRRenderThread::addActorOffline( vtkActor* actor ) {

    /* Check to see if render thread is running */
    if (!this->isRunning()) {
        double* ac = actor->GetOrigin();

        /* I have found that these initial transforms will position the FS
         * car model in a sensible position but you can experiment
         */
        actor->RotateX(-90);
        actor->AddPosition(-ac[0]+0, -ac[1]-100, -ac[2]-200);

        actors->AddItem(actor);
    }
}



void VRRenderThread::issueCommand( int cmd, double value ) {

    /* Update class variables according to command */
    switch (cmd) {
    /* These are just a few basic examples */
    case END_RENDER:
        this->endRender = true;
        break;

    case ROTATE_X:
        this->rotateX = value;
        break;

    case ROTATE_Y:
        this->rotateY = value;
        break;

    case ROTATE_Z:
        this->rotateZ = value;
        break;
    }
}

/* This function runs in a separate thread. This means that the program
 * can fork into two separate execution paths. This thread is triggered by
 * calling VRRenderThread::start()
 */
void VRRenderThread::run() {
    /* You might want to edit the 3D model once VR has started, however VTK is not "thread safe".
     * This means if you try to edit the VR model from the GUI thread while the VR thread is
     * running, the program could become corrupted and crash. The solution is to get the VR thread
     * to edit the model. Any decision to change the VR model will come fromthe user via the GUI thread,
     * so there needs to be a mechanism to pass data from the GUi thread to the VR thread.
     */

    vtkNew<vtkNamedColors> colors;

    // Set the background color.
    std::array<unsigned char, 4> bkg{ {26, 51, 102, 255} };
    colors->SetColor("BkgColor", bkg.data());

    // The renderer generates the image
    // which is then displayed on the render window.
    // It can be thought of as a scene to which the actor is added
    renderer = vtkOpenVRRenderer::New();

    renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());

    /* Loop through list of actors provided and add to scene */
    vtkActor* a;
    actors->InitTraversal();
    while( (a = (vtkActor*)actors->GetNextActor() ) ) {
        renderer->AddActor(a);
    }

    /* The render window is the actual GUI window
     * that appears on the computer screen
     */
    window = vtkOpenVRRenderWindow::New();

    window->Initialize();
    window->AddRenderer(renderer);

    /* Create Open VR Camera */
    camera = vtkOpenVRCamera::New();
    renderer->SetActiveCamera(camera);

    /* The render window interactor captures mouse events
     * and will perform appropriate camera or actor manipulation
     * depending on the nature of the events.
     */
    interactor = vtkOpenVRRenderWindowInteractor::New();
    interactor->SetRenderWindow(window);
    interactor->Initialize();
    window->Render();


    /* Now start the VR - we will implement the command loop manually
     * so it can be interrupted to make modifications to the actors
     * (i.e. to implement animation)
     */
    endRender = false;
    t_last = std::chrono::steady_clock::now();

    while( !interactor->GetDone() && !this->endRender ) {
        interactor->DoOneEvent( window, renderer );

        /* Check to see if enough time has elapsed since last update
         * This looks overcomplicated (and it is, C++ loves to make things unecessarily complicated!) but
         * is really just checking if more than 20ms have elaspsed since the last animation step. The
         * complications comes from the fact that numbers representing time on computers don't usually have
         * standard second/millisecond units. Because everything is a class in C++, the converion from
         * computer units to seconds/milliseconds ends up looking like what you see below.
         *
         * My choice of 20ms is arbitrary, if this value is too small the animation calculations could begin to
         * interfere with the interator processes and make the simulation unresponsive. If it is too large
         * the animations will be jerky. Play with the value to see what works best.
         */
        if (std::chrono::duration_cast <std::chrono::milliseconds> (std::chrono::steady_clock::now() - t_last).count() > 20) {

            /* Do things that might need doing ... */
            vtkActorCollection* actorList = renderer->GetActors();
            vtkActor* a;

            /* X Rotation */
            actorList->InitTraversal();
            while ((a = (vtkActor*)actorList->GetNextActor())) {
                a->RotateX(rotateX);
            }

            /* Y Rotation */
            actorList->InitTraversal();
            while ((a = (vtkActor*)actorList->GetNextActor())) {
                a->RotateY(rotateY);
            }

            /* Z Rotation */
            actorList->InitTraversal();
            while ((a = (vtkActor*)actorList->GetNextActor())) {
                a->RotateZ(rotateZ);
            }

            /* Remember time now */
            t_last = std::chrono::steady_clock::now();
        }
    }
}


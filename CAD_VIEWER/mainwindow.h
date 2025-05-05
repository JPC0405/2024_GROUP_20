/*! @file mainwindow.h
 *
 *  The header file's purpose is to make the functions used to create the applications for the ui.
 *
 *  Jay Chauhan, Charles Egan and Jacob Moore 2025
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Modelpart.h"
#include "ModelpartList.h"
#include "VRRenderThread.h"
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>

/* Project headers */

/* Qt headers */
#include <QThread>
#include <QMutex>
#include <QWaitCondition>

/* Vtk headers */
#include <vtkActor.h>
#include <vtkOpenVRRenderWindow.h>
#include <vtkOpenVRRenderWindowInteractor.h>
#include <vtkOpenVRRenderer.h>
#include <vtkOpenVRCamera.h>
#include <vtkActorCollection.h>
#include <vtkCommand.h>
#include <vtkSkybox.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
/*! \class MainWindow
 *  \brief The MainWindow class intialises the main window and its components for the application and for it to work with VTK
 *  In the MainWindow class, it connects the components from the ModelPartList and the vtk renderer to the MainWindow for the user to be able to interact with the ui.
 *  The class allows for rendering and holding of values, the user to perform file actions and button interactions on the ui.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /*!
    * Constructor
    * \param parent widget is set to null
    */
    MainWindow(QWidget *parent = nullptr);
    /*! Destructor
    */
    ~MainWindow();
    void updateRender();
    void UpdateRenderFromTree(const QModelIndex& index);

    void AddVRActors( const QModelIndex& index);


    /*!
     * \brief updateChildren
     * Updates the values of the vis, r, g and b
     * \param parent the item in the ModelPart which contains the values of the children
     * \param vis the visibilty value of the item
     * \param r the red value of the colour of the item
     * \param g the green value of the colour of the item
     * \param b the blue value of the colour of the item
     */
    void updateChildren(ModelPart* parent, bool vis, double r, double g, double b, float xmin, float xmax, float ymin, float ymax, float zmin, float zmax, float size);

private:

    Ui::MainWindow *ui; /*!< Pointer to the ui>*/
    ModelPartList* partList; /*!< Pointer to the ModelPartList file>*/
    vtkSmartPointer<vtkRenderer> renderer; /*!< Pointer to the vtk renderer>*/
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;/*!< Pointer to the vtk generic open GL renderer window >*/
    bool VR_ON = 0;
    vtkSmartPointer<vtkSkybox> skyboxActor;
    VRRenderThread* VRthread;

    vtkSmartPointer<vtkLight> light;

public slots:
    void handleButton();
    void handleTreeClick();


signals:
    void statusUpdateMessage(const QString & message, int timeout);

private slots:
    void on_actionOpen_File_triggered();
    void on_pushButton_2_clicked();
    void on_actionItems_Options_triggered();



    void on_pushButton_3_clicked();

};



#endif // MAINWINDOW_H



#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Modelpart.h"
#include "ModelpartList.h"
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

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

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE
class VRRenderThread : public QThread {
    Q_OBJECT

public:
    /** List of command names */
    enum {
        END_RENDER,
        ROTATE_X,
        ROTATE_Y,
        ROTATE_Z
    } Command;


    /**  Constructor
      */
    VRRenderThread(QObject* parent = nullptr);

    /**  Denstructor
      */
    ~VRRenderThread();

    /** This allows actors to be added to the VR renderer BEFORE the VR
      * interactor has been started
     */
    void addActorOffline(vtkActor* actor);


    /** This allows commands to be issued to the VR thread in a thread safe way.
      * Function will set variables within the class to indicate the type of
      * action / animation / etc to perform. The rendering thread will then impelement this.
      */
    void issueCommand( int cmd, double value );



protected:
    /** This is a re-implementation of a QThread function
      */
    void run() override;

private:
    /* Standard VTK VR Classes */
    vtkSmartPointer<vtkOpenVRRenderWindow>              window;
    vtkSmartPointer<vtkOpenVRRenderWindowInteractor>    interactor;
    vtkSmartPointer<vtkOpenVRRenderer>                  renderer;
    vtkSmartPointer<vtkOpenVRCamera>                    camera;

    /* Use to synchronise passing of data to VR thread */
    QMutex                                              mutex;
    QWaitCondition                                      condition;

    /** List of actors that will need to be added to the VR scene */
    vtkSmartPointer<vtkActorCollection>                 actors;

    /** A timer to help implement animations and visual effects */
    std::chrono::time_point<std::chrono::steady_clock>  t_last;

    /** This will be set to false by the constructor, if it is set to true
      * by the GUI then the rendering will end
      */
    bool                                                endRender;

    /* Some variables to indicate animation actions to apply.
     *
     */
    double rotateX;         /*< Degrees to rotate around X axis (per time-step) */
    double rotateY;         /*< Degrees to rotate around Y axis (per time-step) */
    double rotateZ;         /*< Degrees to rotate around Z axis (per time-step) */
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void updateRender();
    void UpdateRenderFromTree(const QModelIndex& index);
    void AddVRActors( const QModelIndex& index,VRRenderThread* thread);
    void updateChildren(ModelPart* parent, bool vis, double r, double g, double b);

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;

    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;

public slots:
    void handleButton();
    void handleTreeClick();


signals:
    void statusUpdateMessage(const QString & message, int timeout);

private slots:
    void on_actionOpen_File_triggered();
    void on_pushButton_2_clicked();
    void on_actionItems_Options_triggered();
};



#endif // MAINWINDOW_H



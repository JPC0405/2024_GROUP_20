#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Modelpart.h"
#include "ModelpartList.h"
#include "VRRenderThread.h"
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
    bool VR_ON = 0;


    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    VRRenderThread* VRthread;

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



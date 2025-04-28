#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Modelpart.h"
#include "ModelpartList.h"
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>

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

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
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkLight.h>

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
    /*!
     * \brief The function updates the vtk to change it to the current values
    */
    void updateRender();
    /*!
     * \brief UpdateRenderFromTree
     * Updates the index in the tree
     * \param index the item number in the tree
     */
    void UpdateRenderFromTree(const QModelIndex& index);
    /*!
     * \brief updateChildren
     * Updates the values of the vis, r, g and b
     * \param parent the item in the ModelPart which contains the values of the children
     * \param vis the visibilty value of the item
     * \param r the red value of the colour of the item
     * \param g the green value of the colour of the item
     * \param b the blue value of the colour of the item
     */
    void updateChildren(ModelPart* parent, bool vis, double r, double g, double b);

private:

    Ui::MainWindow *ui; /*!< Pointer to the ui>*/
    ModelPartList* partList; /*!< Pointer to the ModelPartList file>*/

    vtkSmartPointer<vtkRenderer> renderer; /*!< Pointer to the vtk renderer>*/
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;/*!< Pointer to the vtk generic open GL renderer window >*/

    vtkSmartPointer<vtkLight> light;

public slots:
    /*!
     * \brief handleButton
     * The function for when the button is pressed
     */
    void handleButton();
    /*!
     * \brief handleTreeClick
     * The function for when the tree is clicked and emits a message for which item is selected
     */
    void handleTreeClick();


signals:
    /*!
     * \brief statusUpdateMessage
     * The signal emits an updated message to the status bar
     * \param message the message emitted
     * \param timeout how long the message stays there
     */
    void statusUpdateMessage(const QString & message, int timeout);

private slots:
    /*!
     * \brief on_actionOpen_File_triggered
     * Opens a file and releases the name of the file to the status bar.
     * In addition, opens the STL file to the renderer with predetermined rgb and vis values
     */
    void on_actionOpen_File_triggered();
    /*!
     * \brief on_pushButton_2_clicked
     * Opens a dialog to set the rgb values and vis value and applies them to the rendered STL file
     */
    void on_pushButton_2_clicked();
    /*!
     * \brief on_actionItems_Options_triggered
     * Emits a message to the status bar that the action has been clicked
     */
    void on_actionItems_Options_triggered();



};
#endif // MAINWINDOW_H

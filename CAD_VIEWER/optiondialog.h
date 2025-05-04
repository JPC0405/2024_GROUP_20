/*! @file optiondialog.h
 *
 *  Opens a dialog widget to set values for the rgb values and visibility value and returns those values
 *
 *  Jay Chauhan, Charles Egan and Jacob Moore 2025
 */

#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}
/*!
 * \brief The OptionDialog class
 * The class is used for a dialog to set rgb values and the visibilty value
 *
 */
class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    /*! Constructor
     * \param parent widget is set to null
     */
    explicit OptionDialog(QWidget *parent = nullptr);
    /*! Destructor
     */
    ~OptionDialog();

    /*!
     * \brief setVisibility function is used to set the value of visibility
     * \param v is the variable to represent the value of visibility
     */
    void setVisibility( bool v );
    /*!
     * \brief getVisibility calls the current value of the visibility
     * \return returns thr value of v
     */
    bool getVisibility();

    /*!
     * \brief set_name sets the name of the file
     * \param name is the variable to represent the name of file
     */
    void set_name(QString name);

    /*!
     * \brief get_name calls the current name of the file
     * \return returns the variable name
     */
    QString get_name();

    /*!
     * \brief set_R Sets the amount of red in the colour
     * \param R represents the amount of red
     */
    void set_R(unsigned char R);
    /*!
     * \brief get_R calls the current value of the red
     * \return returns the variable R
     */
    unsigned char get_R();

    /*!
     * \brief set_G Sets the amount of green in the colour
     * \param G represents the amount of green
     */
    void set_G(unsigned char G);
    /*!
     * \brief get_G calls the amount of green in the colour
     * \return returns the variable G
     */
    unsigned char get_G();

    /*!
     * \brief set_B sets the amount of blue in the colour
     * \param B represents the amount of blue
     */
    void set_B(unsigned char B);
    /*!
     * \brief get_B calls the amount of blue in the colour
     * \return returns the variable B
     */
    unsigned char get_B();

    void set_Clip(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    void setSize(float size);
    float get_MinX();
    float get_MaxX();

    float get_MinY();
    float get_MaxY();

    float get_MinZ();
    float get_MaxZ();

    float getSize();


private slots:
    /*!
     * \brief on_lineEdit_editingFinished
     * Ends the dialog
     */
    void on_lineEdit_editingFinished();

private:
    Ui::OptionDialog *ui; /*!< Pointer to the ui >*/
};

#endif // OPTIONDIALOG_H

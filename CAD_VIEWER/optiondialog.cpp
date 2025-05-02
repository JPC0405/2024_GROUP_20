/*! @file optiondialog.cpp
 *
 *  The file contains the functions from the optiondialog.h to create a dialog to set rgb, visisibility and name variables
 *
 *  Jay Chauhan, Charles Egan and Jacob Moore 2025
 */


#include "optiondialog.h"
#include "ui_optiondialog.h"

/*!
 * \brief OptionDialog::OptionDialog
 * Constructor
 * \param parent is the parent widget
 */
OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    ui->spinBox->setRange(0,255);
    ui->spinBox_2->setRange(0,255);
    ui->spinBox_3->setRange(0,255);
}

/*!
 * \brief OptionDialog::~OptionDialog
 * Destructor
 */
OptionDialog::~OptionDialog()
{
    delete ui;
}


void OptionDialog::on_lineEdit_editingFinished()
{
    QString name = ui->lineEdit->text();
}

/*!
 * \brief OptionDialog::setVisibility
 * Sets the visibility variable in the dialog
 * \param vis
 */
void OptionDialog::setVisibility(bool vis)
{
    ui->radioButton->setChecked(vis);
}

/*!
 * \brief OptionDialog::getVisibility
 * Returns the value of the visibility variable
 * \return returns the variable for visibility
 */
bool OptionDialog::getVisibility()
{
    bool vis = ui->radioButton->isChecked();
    return vis;
}

/*!
 * \brief OptionDialog::set_name
 * Sets the name for the file in the dialog
 * \param name is the variable for the file's name
 */
void OptionDialog::set_name(QString name)
{
    ui->lineEdit->setText(name);
}

/*!
 * \brief OptionDialog::get_name
 * Calls the file's name
 * \return returns the variable for the file name
 */
QString OptionDialog::get_name()
{
    QString name = ui->lineEdit->text();
    return name;
}

/*!
 * \brief OptionDialog::set_R
 * Sets the value for the red colour in the dialog
 * \param R is the variable that represents the red colour
 */
void OptionDialog::set_R(unsigned char R)
{
    ui->spinBox->setValue(R);
}

/*!
 * \brief OptionDialog::get_R
 * Calls the value of the red colour
 * \return returns the red colour component of the rgb values
 */
unsigned char OptionDialog::get_R()
{
    unsigned char R = ui->spinBox->value();
    return R;
}

/*!
 * \brief OptionDialog::set_G
 * Sets the value for the green colour in the dialog
 * \param G is the variable that represents the green colour
 */
void OptionDialog::set_G(unsigned char G)
{
    ui->spinBox_2->setValue(G);
}

/*!
 * \brief OptionDialog::get_G
 * Calls the value of the green colour
 * \return returns the green colour component of the rgb values
 */
unsigned char OptionDialog::get_G()
{
    unsigned char G = ui->spinBox_2->value();
    return G;
}

/*!
 * \brief OptionDialog::set_B
 * Sets the value for the blue colour in the dialog
 * \param B is the variable that represents the blue colour
 */
void OptionDialog::set_B(unsigned char B)
{
    ui->spinBox_3->setValue(B);
}

/*!
 * \brief OptionDialog::get_B
 * Calls the value of the green colour
 * \return returns the blue colour component of the rgb values
 */
unsigned char OptionDialog::get_B()
{
    unsigned char B = ui->spinBox_3->value();
    return B;
}
void OptionDialog::set_Clip(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax)
{
    ui->xMinBox->setValue(xmin);
    ui->xMaxBox->setValue(xmax);
    ui->yMinBox->setValue(ymin);
    ui->yMaxBox->setValue(ymax);
    ui->zMinBox->setValue(zmin);
    ui->zMaxBox->setValue(zmax);
}
void OptionDialog::setSize(float size){
    ui->sizeSlider->setValue(size);
    qDebug()<<"1 Set size: "<<size;
}
float OptionDialog::get_MinX()
{
    float x = ui->xMinBox->value();
    return x;
}
float OptionDialog::get_MaxX()
{
    float x = ui->xMaxBox->value();
    return x;
}
float OptionDialog::get_MinY()
{
    float y = ui->yMinBox->value();
    return y;
}
float OptionDialog::get_MaxY()
{
    float y = ui->yMaxBox->value();
    return y;
}
float OptionDialog::get_MinZ()
{
    float z = ui->zMinBox->value();
    return z;
}
float OptionDialog::get_MaxZ()
{
    float z = ui->zMaxBox->value();
    return z;
}

float OptionDialog::getSize(){
    float size = ui->sizeSlider->value();
    qDebug()<<"2 Got size: "<<size;
    return size;
}

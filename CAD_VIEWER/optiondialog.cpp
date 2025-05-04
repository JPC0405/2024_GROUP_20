#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)
{
    ui->setupUi(this);

    ui->spinBox->setRange(0,255);
    ui->spinBox_2->setRange(0,255);
    ui->spinBox_3->setRange(0,255);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}


void OptionDialog::on_lineEdit_editingFinished()
{
    QString name = ui->lineEdit->text();
}

void OptionDialog::setVisibility(bool vis)
{
    ui->radioButton->setChecked(vis);
}

bool OptionDialog::getVisibility()
{
    bool vis = ui->radioButton->isChecked();
    return vis;
}

void OptionDialog::set_name(QString name)
{
    ui->lineEdit->setText(name);
}

QString OptionDialog::get_name()
{
    QString name = ui->lineEdit->text();
    return name;
}

void OptionDialog::set_R(unsigned char R)
{
    ui->spinBox->setValue(R);
}

unsigned char OptionDialog::get_R()
{
    unsigned char R = ui->spinBox->value();
    return R;
}


void OptionDialog::set_G(unsigned char G)
{
    ui->spinBox_2->setValue(G);
}

unsigned char OptionDialog::get_G()
{
    unsigned char G = ui->spinBox_2->value();
    return G;
}
void OptionDialog::set_B(unsigned char B)
{
    ui->spinBox_3->setValue(B);
}

unsigned char OptionDialog::get_B()
{
    unsigned char B = ui->spinBox_3->value();
    return B;
}

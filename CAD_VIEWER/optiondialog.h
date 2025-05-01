#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(QWidget *parent = nullptr);
    ~OptionDialog();

    void setVisibility( bool v );
    bool getVisibility();

    void set_name(QString name);
    QString get_name();

    void set_R(unsigned char R);
    unsigned char get_R();

    void set_G(unsigned char G);
    unsigned char get_G();

    void set_B(unsigned char B);
    unsigned char get_B();

    void set_Clip(float xmin, float xmax, float ymin, float ymax, float zmin, float zmax);
    float get_MinX();
    float get_MaxX();

    float get_MinY();
    float get_MaxY();

    float get_MinZ();
    float get_MaxZ();


private slots:
    void on_lineEdit_editingFinished();

private:
    Ui::OptionDialog *ui;
};

#endif // OPTIONDIALOG_H

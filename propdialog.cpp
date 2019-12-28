#include "propdialog.h"
#include "ui_propdialog.h"
#include "ui_mainwindow.h"
#include <QFontDialog>

PropDialog::PropDialog(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::PropDialog)
{
    ui->setupUi(this);

}

PropDialog::~PropDialog()
{
    delete ui;
}


void PropDialog::on_buttonBox_accepted()
{
    emit property_to_set(m_prop);
}

void PropDialog::on_pushButton_clicked()
{
    QColorDialog m_dialog;
    m_prop.background_color = m_dialog.getColor();
    ui->b_col_label->setText(m_prop.background_color.name());
}

void PropDialog::on_pushButton_2_clicked()
{
    QColorDialog m_dialog;
    m_prop.text_color = m_dialog.getColor();
    ui->t_color_label->setText(m_prop.text_color.name());
}

void PropDialog::on_pushButton_3_clicked()
{
    QFontDialog m_dialog;
    m_prop.text_font = m_dialog.getFont(nullptr, this);
    ui->font_label->setText(m_prop.text_font.toString());
}

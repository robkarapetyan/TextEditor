#ifndef PROPDIALOG_H
#define PROPDIALOG_H

#include <QDialog>
#include "mainwindow.h"
#include "editorproperty.h"
#include <QColorDialog>

namespace Ui {
class PropDialog;
}

class PropDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PropDialog(MainWindow *parent = nullptr);
    ~PropDialog();

private:
    Ui::PropDialog *ui;
    EditorProperty m_prop;

signals:
    void property_to_set(EditorProperty prop);
private slots:
    void on_buttonBox_accepted();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
};

#endif // PROPDIALOG_H

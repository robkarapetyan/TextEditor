#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    int scaling = 100;
private slots:
    void on_actionopen_triggered();

    void on_actionsave_as_triggered();

    void on_actionsave_triggered();

    void on_actionfind_triggered();

    void on_actionundo_triggered();

    void on_actionredo_triggered();
    void scaling_received(int);
private:
    QVector<std::pair<int,int>> vec_for_found_substring_coordinates = {};
    QString current_file = "";
    QString stuff_to_find = "";
    Ui::MainWindow *ui;
protected:
    void keyPressEvent(QKeyEvent *) override;
};
#endif // MAINWINDOW_H

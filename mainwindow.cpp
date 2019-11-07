#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTextBlock>
#include <QInputDialog>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionopen_triggered()
{
    stuff_to_find = "";

    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open", QString(),"(*.txa)");
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        if(file.errorString() != "No file name specified"){
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        }
    }
    QTextStream in(&file);

    current_file = fileName;
    setWindowTitle(fileName);
    ui->plainTextEdit->setPlainText(in.readAll());
    file.close();
}

void MainWindow::on_actionsave_as_triggered()
{
    stuff_to_find = "";

    QString fileName = QFileDialog::getSaveFileName(this, "Save as", QString(),"(*.txa)");
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        if(file.errorString() != "No file name specified"){
            QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
        }
    }
    setWindowTitle(fileName);
    QTextStream out(&file);
    current_file = fileName;
    out << ui->plainTextEdit->toPlainText();
    file.close();
}

void MainWindow::on_actionsave_triggered()
{
    if(current_file != ""){
        QFile file(current_file);
        if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
            if(file.errorString() != "No file name specified"){
                QMessageBox::warning(this, "Warning", "Cannot open file: " + file.errorString());
            }
           return;
        }
        QTextStream out(&file);
        out << ui->plainTextEdit->toPlainText();
        file.close();
    }
    else{
        on_actionsave_as_triggered();
    }
}

void MainWindow::on_actionfind_triggered()
{
    if(stuff_to_find == ""){
        bool ok = 0;
        QString str = QInputDialog::getText(this, "Find","find what", QLineEdit::Normal,"", &ok);
        if (ok)
        {
            stuff_to_find = str;
        }
        ui->plainTextEdit->find(stuff_to_find,QTextDocument::FindFlag::FindCaseSensitively);
    }
    else{
        QTextCursor tmp(ui->plainTextEdit->textCursor());
        tmp.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1);

        if(ui->plainTextEdit->textCursor().position() ==  tmp.position()){
            QTextCursor tmpCursor = ui->plainTextEdit->textCursor();
            tmpCursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor, 1);
            //tmpCursor.position()
            ui->plainTextEdit->setTextCursor(tmpCursor);    //moving  cursor to the start position of the editor

            QMessageBox msgBox;
            msgBox.setText(QString(" End of file reached while searching ' " + stuff_to_find + " '"));
            msgBox.setInformativeText(" Cursor was moved to the start ");
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }
        else if(ui->plainTextEdit->find(stuff_to_find,QTextDocument::FindFlag::FindCaseSensitively) == false){
            QMessageBox msgBox;
            msgBox.setText(QString(" No more ' " + stuff_to_find + " ' found in the file"));
            msgBox.setIcon(QMessageBox::Information);
            msgBox.exec();
        }
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier)){
        if(event->key() == Qt::Key::Key_F){
            on_actionfind_triggered();
        }
    }
    else if(event->key() == Qt::Key::Key_F5){
        on_actionfind_triggered();
    }
    QWidget::keyPressEvent(event);
}

void MainWindow::on_actionundo_triggered()
{
    ui->plainTextEdit->undo();
}

void MainWindow::on_actionredo_triggered()
{
    ui->plainTextEdit->redo();
}

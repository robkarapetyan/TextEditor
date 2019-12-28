#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "propdialog.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QTextBlock>
#include <QInputDialog>
#include <QTextCodec>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Text Editor0_o");
//    ui->centralwidget->setStyleSheet("QWidget { background-color: gray; }");
//    ui->plainTextEdit->setStyleSheet("QPlainTextEdit {background-color: yellow}");
    ui->plainTextEdit->setBackgroundVisible(false);

    lb = new QLabel("",ui->plainTextEdit, Qt::SplashScreen  |  Qt::WindowStaysOnTopHint);

    ui->spinBox->setSuffix("%");
    ui->spinBox->setRange(50, 300);
    ui->spinBox->setValue(100);
    ui->spinBox->setSingleStep(10);

    connect(ui->spinBox, SIGNAL(valueChanged(int)), this,SLOT( scaling_received(int)));

    connect(ui->plainTextEdit, SIGNAL(cursorPositionChanged()), this, SLOT(update_labels()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if(QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
    {
        if(event->delta() >= 0){
            ui->spinBox->setValue(ui->spinBox->value() + 5);
        }
        else{
            ui->spinBox->setValue(ui->spinBox->value() - 5);
        }
        lb->setText(ui->spinBox->text());
        lb->setAlignment(Qt::AlignCenter);
        lb->setGeometry(this->pos().x() + this->width() /2, this->pos().y() + this->height() / 2, 50,50);
        lb->show();
        QTimer::singleShot(1000, lb,  &QLabel::hide);

        return;
    }
    MainWindow::wheelEvent(event);
}

void MainWindow::on_actionopen_triggered()
{
    stuff_to_find = "";

    QString fileName = QFileDialog::getOpenFileName(nullptr, "Open", QString(),tr("Text Documents (*.txt *.txa)"));
    if(fileName == ""){
        return;
    }
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

    QString fileName = QFileDialog::getSaveFileName(this, "Save as", QString(),tr("Text Documents (*.txt *.txa)"));
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
        if(event->key() == Qt::Key::Key_S){
            on_actionsave_triggered();
        }
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

void MainWindow::scaling_received(int a)
{
    if(a > scaling){
        ui->plainTextEdit->zoomIn();
    }
    else{
        ui->plainTextEdit->zoomOut();
    }
    scaling = a;
}

void MainWindow::update_labels()
{
    ui->column->setText(QStringLiteral("Col: %1").arg(ui->plainTextEdit->textCursor().columnNumber() + 1));
    ui->line->setText(QStringLiteral("line: %1 ,").arg(ui->plainTextEdit->textCursor().blockNumber() + 1));
}


void MainWindow::on_actionproperties_triggered()
{
    auto* pd = new PropDialog(this);
    pd->show();
    connect(pd, SIGNAL(property_to_set(EditorProperty)), this, SLOT(set_properties(EditorProperty)));
}

void MainWindow::set_properties(EditorProperty prop)
{
    QPalette p = ui->plainTextEdit->palette();
    p.setColor(QPalette::Base, prop.background_color);
    p.setColor(QPalette::Text, prop.text_color);

    ui->plainTextEdit->setPalette(p);
    ui->plainTextEdit->setFont(prop.text_font);
}

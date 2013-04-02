#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QProcess>
#include <QString>
#include <QFileDialog>
#include <QIntValidator>
#include <QStringList>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QIntValidator *angleInputRange = new QIntValidator(0,90,this);
    QIntValidator *rotationInputRange = new QIntValidator(0,360,this);
    QIntValidator *inputRange = new QIntValidator(0,65535,this);
    ui->setupUi(this);
    ui->widthLineEdit->setValidator(inputRange);
    ui->heightLineEdit->setValidator(inputRange);
    ui->angleLineEdit->setValidator(angleInputRange);
    ui->addRotationLineEdit->setValidator(rotationInputRange);
    this->setWindowTitle("Prerenderer");
    QFile profilesFile("guiProfiles.txt");

    if (!profilesFile.exists()){
        profilesFile.open(QIODevice::ReadWrite);
        QTextStream stream(&profilesFile);

        stream<<"#Default";
    }else{


    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

QStringList rotations;

void MainWindow::on_objSelectButton_clicked()
{
    QString objName = QFileDialog::getOpenFileName(this,tr("Select .obj File"),"/path/to/file/",tr("Obj Files (*.obj)"));
    ui->objectPathTextField->setText(objName);
}

void MainWindow::on_pngSelectButton_clicked()
{
    QString pngName = QFileDialog::getOpenFileName(this,tr("Select .png File"),"/path/to/file/",tr("Png Files (*.png)"));
    ui->texturePathTextField->setText(pngName);
}

void MainWindow::on_renderButton_clicked()
{
    QProcess proces;
    QStringList arguments;
    QString texture;
    QString width;
    QString height;
    QString angle;
    QString objName;

    arguments<<"render";

    if(!ui->texturePathTextField->text().isEmpty()){
        texture="--texture="+ui->texturePathTextField->text();
        arguments<<texture;
    }
    if(!ui->widthLineEdit->text().isEmpty()){
        width="--width="+ui->widthLineEdit->text();
        arguments<<width;
    }
    if(!ui->heightLineEdit->text().isEmpty()){
        height="--height="+ui->heightLineEdit->text();
        arguments<<height;
    }
    if(!ui->angleLineEdit->text().isEmpty()){
        angle="--angle="+ui->angleLineEdit->text();
        arguments<<angle;
    }

    if(!ui->objectPathTextField->text().isEmpty()){
        objName=ui->objectPathTextField->text();
        arguments<<objName;
    }else{
        ui->label->setText("Select obj. file");
        return;
    }

    proces.start("./prerenderer-debug",arguments);
    proces.waitForFinished(-1);
    ui->label->setText("Done");
}

void MainWindow::on_addRotationButton_clicked()
{
    if(!ui->addRotationLineEdit->text().isEmpty()){
        if(rotations.contains(ui->addRotationLineEdit->text())){
            ui->rotationMessageLabel->setText("Cannot add existing \nrotation");
        }else{
            rotations<<ui->addRotationLineEdit->text();
            ui->rotationsListWidget->addItem(ui->addRotationLineEdit->text());
            ui->rotationMessageLabel->setText("Rotation "+ui->addRotationLineEdit->text()+" added");
        }
    }else{
        ui->rotationMessageLabel->setText("Insert rotation value");
    }
}

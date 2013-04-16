#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "profiledialog.h"
#include <QProcess>
#include <QString>
#include <QFileDialog>
#include <QIntValidator>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QList>
#include <fstream>
#include <iostream>

QFile profilesFile("guiProfiles.txt");
QList<profile> listOfProfiles;
profile currentProfile;
Qt::SortOrder order=Qt::AscendingOrder;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setValidators();
    this->setWindowTitle("Prerenderer");

    if (!profilesFile.exists()){
        generateGuiProfileFile();
    }else{
        loadProfilesFromFile();
    }
    for (int i = 0; i < listOfProfiles.size(); ++i) {
        //ui->profilesListWidget->addItem(listOfProfiles.value(i).profileName);
        ui->profilesComboBox->addItem(listOfProfiles.value(i).profileName);
    }
    ui->profilesComboBox->setCurrentIndex(0);

    currentProfile=listOfProfiles.value(0);
    loadCurrentProfileToGui();
}

void MainWindow::setValidators(){
    QIntValidator *angleInputRange = new QIntValidator(0,90,this);
    QIntValidator *rotationInputRange = new QIntValidator(0,359,this);
    QIntValidator *intInputRange = new QIntValidator(this);
    QDoubleValidator *zoomInputRange = new QDoubleValidator(0.01,10.00,2,this);
    ui->widthLineEdit->setValidator(intInputRange);
    ui->heightLineEdit->setValidator(intInputRange);
    ui->angleLineEdit->setValidator(angleInputRange);
    ui->addRotationLineEdit->setValidator(rotationInputRange);
    ui->zoomLineEdit->setValidator(zoomInputRange);
}

void MainWindow::loadProfilesFromFile(){
    profilesFile.open(QIODevice::ReadOnly);
    QTextStream stream(&profilesFile);
    QString line= stream.readLine();
    while(!stream.atEnd()){
        profile readProfile;
        readProfile.profileName=line;
        stream.readLine();
        readProfile.angle=stream.readLine();
        stream.readLine();
        readProfile.height=stream.readLine();
        stream.readLine();
        readProfile.width=stream.readLine();
        stream.readLine();
        readProfile.zoom=stream.readLine();
        stream.readLine();
        readProfile.layer=stream.readLine();
        stream.readLine();
        line=stream.readLine();
        QStringList readRotations;
        while(line!="end"){
            readRotations<<line;
            line=stream.readLine();
        }
        stream.readLine();
        line=stream.readLine();
        readProfile.profileRotations=readRotations;
        listOfProfiles<<readProfile;
    }
    profilesFile.close();
}

void MainWindow::generateGuiProfileFile(){
    profilesFile.open(QIODevice::WriteOnly);
    QTextStream stream(&profilesFile);
    profile defaultProfile;
    defaultProfile.profileName="Default";
    defaultProfile.angle="45";
    defaultProfile.height="48";
    defaultProfile.width="64";
    defaultProfile.zoom="0.08";
    defaultProfile.layer="0";
    QStringList defaultRotations;
    defaultRotations<<"0"<<"45"<<"90"<<"135"<<"180"<<"225"<<"270"<<"315";
    defaultProfile.profileRotations=defaultRotations;
    listOfProfiles<<defaultProfile;
    stream<<defaultProfile.profileName+"\nAngle\n"+defaultProfile.angle+"\nHeight\n"
            +defaultProfile.height+"\nWidth\n"+defaultProfile.width+"\nZoom\n"+defaultProfile.zoom+"\n"
            +"Layer\n"+defaultProfile.layer+"\n"+"rotations"+"\n";
    for (int var = 0; var < defaultProfile.profileRotations.size(); ++var) {
        stream<<defaultProfile.profileRotations.value(var)+"\n";
    }
    stream<<"end"<<"\n"<<"\n";
    stream.flush();
    profilesFile.close();
}

void MainWindow::loadCurrentProfileToGui(){
    ui->rotationsListWidget->clear();
    for (int i = 0; i < currentProfile.profileRotations.size(); ++i) {
        QListWidgetItem *item = new QListWidgetItem;
        item->setData(Qt::DisplayRole, currentProfile.profileRotations.value(i).toInt());
        ui->rotationsListWidget->addItem(item);
    }
    if(currentProfile.angle!="-1") {ui->angleLineEdit->setText(currentProfile.angle);}
    if(currentProfile.height!="-1") {ui->heightLineEdit->setText(currentProfile.height);}
    if(currentProfile.width!="-1") {ui->widthLineEdit->setText(currentProfile.width);}
    if(currentProfile.zoom!="-1") {ui->zoomLineEdit->setText(currentProfile.zoom);}
    ui->layersComboBox->setCurrentIndex(currentProfile.layer.toInt());
    ui->rotationsListWidget->setSortingEnabled(true);
    ui->rotationsListWidget->sortItems(order);
}

void MainWindow::addProfileToFile(profile newProfile){

    profilesFile.open(QIODevice::Append);
    QTextStream stream(&profilesFile);
    stream<<newProfile.profileName+"\nAngle\n"+newProfile.angle+"\nHeight\n"
            +newProfile.height+"\nWidth\n"+newProfile.width+"\nZoom\n"+newProfile.zoom+"\n"
            +"Layer\n"+newProfile.layer+"\n"+"rotations"+"\n";
    for (int var = 0; var < newProfile.profileRotations.size(); ++var) {
        stream<<newProfile.profileRotations.value(var)+"\n";
    }
    stream<<"end"<<"\n"<<"\n";
    stream.flush();
    profilesFile.close();
}


MainWindow::~MainWindow()
{
    delete ui;
}

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
    QStringList arguments;
    QString texture, width, height, angle, objName, zoom;

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

    if(!ui->zoomLineEdit->text().isEmpty()){
        zoom="--zoom="+ui->zoomLineEdit->text();
        arguments<<zoom;
    }

    if(!ui->objectPathTextField->text().isEmpty()){
        objName=ui->objectPathTextField->text();
        arguments<<objName;
    }else{
        ui->label->setText("Select obj. file");
        return;
    }

    QProcess *proces = new QProcess(this);
    proces->start("./prerenderer-debug",arguments);
    proces->waitForFinished(-1);

    ui->label->setText("Done");
}

void MainWindow::on_addRotationButton_clicked()
{
    if(!ui->addRotationLineEdit->text().isEmpty()){
        if(currentProfile.profileRotations.contains(ui->addRotationLineEdit->text())){
        }else{
            currentProfile.profileRotations<<ui->addRotationLineEdit->text();
            QString newRotation=ui->addRotationLineEdit->text();
            QListWidgetItem *item = new QListWidgetItem;
            item->setData(Qt::DisplayRole, newRotation.toInt());
            ui->rotationsListWidget->addItem(item);
            ui->rotationsListWidget->sortItems(order);
        }
    }
}

void MainWindow::on_removeRotationButton_clicked()
{
    QList<QListWidgetItem *> selectedRotations=ui->rotationsListWidget->selectedItems();
    for (int i = 0; i < selectedRotations.size(); ++i) {
        for (int j = 0; j < currentProfile.profileRotations.length(); ++j) {
            QString item=selectedRotations.value(i)->text();
            if(currentProfile.profileRotations.value(j).compare(item)==0){
                currentProfile.profileRotations.removeAt(j);
                break;
            }
        }
    }
    qDeleteAll(selectedRotations);
}

void MainWindow::on_profilesComboBox_currentIndexChanged(const QString &arg1)
{
    for (int i = 0; i < listOfProfiles.size(); ++i) {
        if (listOfProfiles.value(i).profileName==arg1){
            currentProfile=listOfProfiles.value(i);
            loadCurrentProfileToGui();
        }
    }
}

void MainWindow::on_addProfileButton_clicked()
{
    ProfileDialog *pd=new ProfileDialog(this);
    pd->setMessageLabel("Do you want to add new profile with current parameters?");
    pd->setWindowTitle("Add new profile");
    pd->show();

    if(pd->exec()==QDialog::Accepted){
        profile newProfile;
        newProfile.profileName=pd->profileName;

        if(!ui->angleLineEdit->text().isEmpty()){
            newProfile.angle=ui->angleLineEdit->text();
        }else{
            newProfile.angle="-1";
        }

        if(!ui->heightLineEdit->text().isEmpty()){
            newProfile.height=ui->heightLineEdit->text();
        }else{
            newProfile.height="-1";
        }

        if(!ui->widthLineEdit->text().isEmpty()){
            newProfile.width=ui->widthLineEdit->text();
        }else{
            newProfile.width="-1";
        }

        if(!ui->zoomLineEdit->text().isEmpty()){
            newProfile.zoom=ui->zoomLineEdit->text();
        }else{
            newProfile.zoom="-1";
        }

        newProfile.layer=ui->layersComboBox->currentIndex();

        for (int i = 0; i < ui->rotationsListWidget->count(); ++i) {
            QString r=ui->rotationsListWidget->item(i)->text();
            newProfile.profileRotations<<r;
        }

        listOfProfiles<<newProfile;
        ui->profilesComboBox->addItem(newProfile.profileName);
        currentProfile=newProfile;
        ui->profilesComboBox->setCurrentIndex(ui->profilesComboBox->count()-1);
        addProfileToFile(newProfile);
    }


}

void MainWindow::on_editProfileButton_clicked()
{
    ProfileDialog *pd=new ProfileDialog(this);
    pd->setMessageLabel("Do you want to edit profile with current parameters?");
    pd->setWindowTitle("Edit profile");
    pd->show();

    if(pd->exec()==QDialog::Accepted){
        currentProfile.profileName=pd->profileName;

        if(!ui->angleLineEdit->text().isEmpty()){
            currentProfile.angle=ui->angleLineEdit->text();
        }else{
            currentProfile.angle="-1";
        }

        if(!ui->heightLineEdit->text().isEmpty()){
            currentProfile.height=ui->heightLineEdit->text();
        }else{
            currentProfile.height="-1";
        }

        if(!ui->widthLineEdit->text().isEmpty()){
            currentProfile.width=ui->widthLineEdit->text();
        }else{
            currentProfile.width="-1";
        }

        if(!ui->zoomLineEdit->text().isEmpty()){
            currentProfile.zoom=ui->zoomLineEdit->text();
        }else{
            currentProfile.zoom="-1";
        }

        currentProfile.layer=ui->layersComboBox->currentIndex();

        for (int i = 0; i < ui->rotationsListWidget->count(); ++i) {
            QString r=ui->rotationsListWidget->item(i)->text();
            currentProfile.profileRotations<<r;
        }
        //TO DO - change in file
    }

}

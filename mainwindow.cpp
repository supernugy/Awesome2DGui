#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "profiledialog.h"
#include "imagedisplayform.h"
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
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    order(Qt::AscendingOrder),
    profilesFile (new QFile("guiProfiles.txt"))
{
    ui->setupUi(this);
    this->setWindowTitle("Prerenderer");
    setValidators();

    if (!profilesFile->exists()) { generateGuiProfileFile(); }
    else { loadProfilesFromFile(); }

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        ui->profilesComboBox->addItem(listOfProfiles[profileIndex].profileName);
    }

    ui->profilesComboBox->setCurrentIndex(0);
    currentProfile = listOfProfiles.value(0);
    loadCurrentProfileToGui();
}

void MainWindow::setValidators()
{
    QIntValidator *angleInputRange    = new QIntValidator(0,90,this);
    QIntValidator *rotationInputRange = new QIntValidator(0,359,this);
    QIntValidator *intInputRange      = new QIntValidator(this);
    QDoubleValidator *zoomInputRange  = new QDoubleValidator(0.01,10.00,2,this);

    ui->widthLineEdit->setValidator(intInputRange);
    ui->heightLineEdit->setValidator(intInputRange);
    ui->angleLineEdit->setValidator(angleInputRange);
    ui->addRotationLineEdit->setValidator(rotationInputRange);
    ui->zoomLineEdit->setValidator(zoomInputRange);
}

void MainWindow::loadProfilesFromFile()
{
    profilesFile->open(QIODevice::ReadOnly);
    QTextStream stream(profilesFile);
    QString line = stream.readLine();

    while(!stream.atEnd())
    {
        Profile readProfile;
        QStringList readLayers;
        QStringList readRotations;

        readProfile.profileName = line;
        stream.readLine();
        readProfile.angle   = stream.readLine();
        stream.readLine();
        readProfile.height  = stream.readLine();
        stream.readLine();
        readProfile.width   = stream.readLine();
        stream.readLine();
        readProfile.zoom    = stream.readLine();
        stream.readLine();

        line = stream.readLine();
        while(line != "rotations")
        {
            readLayers << line;
            line = stream.readLine();
        }

        line = stream.readLine();
        while(line != "end")
        {
            readRotations << line;
            line = stream.readLine();
        }

        stream.readLine();
        line = stream.readLine();
        readProfile.listOfLayers     = readLayers;
        readProfile.profileRotations = readRotations;
        listOfProfiles << readProfile;
    }
    profilesFile->close();
}

void MainWindow::generateGuiProfileFile()
{
    profilesFile->open(QIODevice::WriteOnly);
    QTextStream stream(profilesFile);
    Profile defaultProfile;

    defaultProfile.profileName = "Default";
    defaultProfile.angle       = "45";
    defaultProfile.height      = "48";
    defaultProfile.width       = "64";
    defaultProfile.zoom        = "0.08";
    defaultProfile.listOfLayers << "diffuse";

    defaultProfile.profileRotations << "0"   << "45"  << "90"  << "135"
                                    << "180" << "225" << "270" << "315";
    listOfProfiles << defaultProfile;

    stream << defaultProfile.profileName     + "\nAngle\n"
            + defaultProfile.angle           + "\nHeight\n"
            + defaultProfile.height          + "\nWidth\n"
            + defaultProfile.width           + "\nZoom\n"
            + defaultProfile.zoom            + "\nLayer\n"
            + defaultProfile.listOfLayers[0] + "\nrotations\n";

    for (int rotationIndex = 0; rotationIndex < defaultProfile.profileRotations.size(); ++rotationIndex)
    {
        stream << defaultProfile.profileRotations[rotationIndex] + "\n";
    }

    stream << "end\n\n";
    stream.flush();
    profilesFile->close();
}

void MainWindow::loadCurrentProfileToGui()
{
    ui->rotationsListWidget->clear();

    for (int rotationIndex = 0; rotationIndex < currentProfile.profileRotations.size(); ++rotationIndex)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->rotationsListWidget, 0);
        item->setData(Qt::DisplayRole, currentProfile.profileRotations[rotationIndex].toInt());
    }

    if(currentProfile.angle  != "-1"){ui->angleLineEdit->setText(currentProfile.angle);}
    if(currentProfile.height != "-1"){ui->heightLineEdit->setText(currentProfile.height);}
    if(currentProfile.width  != "-1"){ui->widthLineEdit->setText(currentProfile.width);}
    if(currentProfile.zoom   != "-1"){ui->zoomLineEdit->setText(currentProfile.zoom);}

    ui->diffuseLayerCheckBox->setChecked(false);
    ui->normalLayerCheckBox->setChecked(false);
    ui->offsetLayerCheckBox->setChecked(false);

    for (int layerIndex = 0; layerIndex < currentProfile.listOfLayers.size(); ++layerIndex) {
        if(currentProfile.listOfLayers[layerIndex] == "diffuse") {ui->diffuseLayerCheckBox->setChecked(true);}
        if(currentProfile.listOfLayers[layerIndex] == "normal")  {ui->normalLayerCheckBox->setChecked(true);}
        if(currentProfile.listOfLayers[layerIndex] == "offset")  {ui->offsetLayerCheckBox->setChecked(true);}
    }

    ui->rotationsListWidget->setSortingEnabled(true);
    ui->rotationsListWidget->sortItems(order);
}

void MainWindow::addProfileToFile(const Profile &newProfile)
{
    if(!profilesFile->open(QIODevice::Append)){
        return;
    }

    QTextStream stream(profilesFile);

    stream << newProfile.profileName + "\nAngle\n"
            + newProfile.angle       + "\nHeight\n"
            + newProfile.height      + "\nWidth\n"
            + newProfile.width       + "\nZoom\n"
            + newProfile.zoom        + "\nLayer\n";

    for (int layerIndex = 0; layerIndex < newProfile.listOfLayers.size(); ++layerIndex) {
        stream << newProfile.listOfLayers[layerIndex] + "\n";
    }
    stream << "rotations\n";

    for (int rotationIndex = 0; rotationIndex < newProfile.profileRotations.size(); ++rotationIndex)
    {
        stream << newProfile.profileRotations[rotationIndex] + "\n";
    }

    stream << "end\n\n";
    stream.flush();
    profilesFile->close();
}

void MainWindow::addAllProfilesToFile()
{
    profilesFile->open(QIODevice::WriteOnly);
    QTextStream stream(profilesFile);

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        stream << listOfProfiles[profileIndex].profileName + "\nAngle\n"
                + listOfProfiles[profileIndex].angle       + "\nHeight\n"
                + listOfProfiles[profileIndex].height      + "\nWidth\n"
                + listOfProfiles[profileIndex].width       + "\nZoom\n"
                + listOfProfiles[profileIndex].zoom        + "\nLayer\n";

        for (int layerIndex = 0; layerIndex < listOfProfiles[profileIndex].listOfLayers.size(); ++layerIndex) {
            stream << listOfProfiles[profileIndex].listOfLayers[layerIndex] + "\n";
        }
        stream << "rotations\n";

        for (int rotationIndex = 0; rotationIndex < listOfProfiles.value(profileIndex).profileRotations.size(); ++rotationIndex)
        {
            stream << listOfProfiles[profileIndex].profileRotations[rotationIndex] + "\n";
        }
        stream << "end\n\n";
    }

    stream.flush();
    profilesFile->close();
}

void MainWindow::updateProfileCombobox(int selectedIndex)
{
    ui->profilesComboBox->clear();

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        ui->profilesComboBox->addItem(listOfProfiles[profileIndex].profileName);
    }

    ui->profilesComboBox->setCurrentIndex(selectedIndex);
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
    if(ui->objectPathTextField->text().isEmpty())
    {
        ui->label->setText("Select .obj file");
        return;
    }

    QStringList arguments("render");

    if(!ui->texturePathTextField->text().isEmpty()) {arguments << "--texture="+ui->texturePathTextField->text();}
    if(!ui->widthLineEdit->text().isEmpty())        {arguments << "--width="+ui->widthLineEdit->text();}
    if(!ui->heightLineEdit->text().isEmpty())       {arguments << "--height="+ui->heightLineEdit->text();}
    if(!ui->angleLineEdit->text().isEmpty())        {arguments << "--angle="+ui->angleLineEdit->text();}
    if(!ui->zoomLineEdit->text().isEmpty())         {arguments << "--zoom="+ui->zoomLineEdit->text();}

    if(!ui->diffuseLayerCheckBox->isChecked() && !ui->normalLayerCheckBox->isChecked() && !ui->offsetLayerCheckBox->isChecked())
    {
        ui->label->setText("Select layers");
        return;
    }
    else
    {
        QString layers("--layer=");
        if (ui->diffuseLayerCheckBox->isChecked()) {layers.append("diffuse,");}
        if (ui->normalLayerCheckBox->isChecked()) {layers.append("normal,");}
        if (ui->offsetLayerCheckBox->isChecked()) {layers.append("offset,");}
        layers.chop(1);
        arguments << layers;
    }
    arguments << ui->objectPathTextField->text();

    QProcess *proces = new QProcess(this);
    QObject::connect(proces,SIGNAL(finished(int)),this,SLOT(prerenderer_finished()));
    proces->start("./prerenderer-debug",arguments);
    qDebug()<<"Proces started";
}

void MainWindow::prerenderer_finished()
{
    ui->label->setText("Done");

    QStringList parts = ui->objectPathTextField->text().split("/");
    QString imageDir = parts.at(parts.size()-1);
    QString pathToImageDir;

    parts.clear();
    parts = imageDir.split(".");
    imageDir.clear();

    for (int index = 0; index < parts.size()-1; ++index)
    {
       imageDir.append(parts[index]);
       imageDir.append(".");
    }

    imageDir.chop(1);
    imageDir.append("_prerender");

    pathToImageDir = QApplication::applicationDirPath();
    pathToImageDir.append("/");
    pathToImageDir.append(imageDir);
    pathToImageDir.append("/");

    ImageDisplayForm *displayForm = new ImageDisplayForm(0,pathToImageDir);
    displayForm->show();
}

void MainWindow::on_addRotationButton_clicked()
{
    if(!ui->addRotationLineEdit->text().isEmpty())
    {
        if(!currentProfile.profileRotations.contains(ui->addRotationLineEdit->text()))
        {
            currentProfile.profileRotations<<ui->addRotationLineEdit->text();
            QString newRotation = ui->addRotationLineEdit->text();
            QListWidgetItem *item = new QListWidgetItem(ui->rotationsListWidget, 0);
            item->setData(Qt::DisplayRole, newRotation.toInt());
            ui->rotationsListWidget->sortItems(order);
        }
    }
}

void MainWindow::on_removeRotationButton_clicked()
{
    QList<QListWidgetItem *> selectedRotations = ui->rotationsListWidget->selectedItems();

    for (int selectedRotationIndex = 0; selectedRotationIndex < selectedRotations.size(); ++selectedRotationIndex)
    {
        for (int rotationIndex = 0; rotationIndex < currentProfile.profileRotations.size(); ++rotationIndex)
        {
            QString item=selectedRotations[selectedRotationIndex]->text();
            if(currentProfile.profileRotations.value(rotationIndex).compare(item) == 0)
            {
                currentProfile.profileRotations.removeAt(rotationIndex);
                break;
            }
        }
    }

    qDeleteAll(selectedRotations);
}

void MainWindow::on_profilesComboBox_currentIndexChanged(const QString &selectedProfile)
{
    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        if (listOfProfiles.value(profileIndex).profileName == selectedProfile)
        {
            currentProfile = listOfProfiles[profileIndex];
            loadCurrentProfileToGui();
        }
    }
}

void MainWindow::on_removeProfileButton_clicked()
{
    ProfileDialog dialogWindow(this, "", "Remove current profile?", false);
    dialogWindow.setWindowTitle("Remove profile");
    dialogWindow.show();
    if(dialogWindow.exec() != QDialog::Accepted) {return;}

    QString oldName = currentProfile.profileName;
    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        if(listOfProfiles.value(profileIndex).profileName == oldName)
        {
            listOfProfiles.removeAt(profileIndex);
            break;
        }
    }
    currentProfile = listOfProfiles.value(0);
    updateProfileCombobox(0);
    loadCurrentProfileToGui();
    addAllProfilesToFile();
}

void MainWindow::on_saveProfileButton_clicked()
{
    ProfileDialog dialogWindow(this, "", "Save profile?", true);
    dialogWindow.setWindowTitle("Save profile");
    dialogWindow.show();
    if(dialogWindow.exec() != QDialog::Accepted) {return;}

    if(dialogWindow.isOverwriten())
    {
        int overwritenProfileIndex = 0;
        for (int index = 0; index < listOfProfiles.size(); ++index)
        {
            //searching for overwriten profile
            if(listOfProfiles[index].profileName == dialogWindow.getProfileName())
            {
                overwritenProfileIndex = index;
                break;
            }
        }

        ui->angleLineEdit->text().isEmpty()   ? listOfProfiles[overwritenProfileIndex].angle   = -1 : listOfProfiles[overwritenProfileIndex].angle   = ui->angleLineEdit->text();
        ui->heightLineEdit->text().isEmpty()  ? listOfProfiles[overwritenProfileIndex].height  = -1 : listOfProfiles[overwritenProfileIndex].height  = ui->heightLineEdit->text();
        ui->widthLineEdit->text().isEmpty()   ? listOfProfiles[overwritenProfileIndex].width   = -1 : listOfProfiles[overwritenProfileIndex].width   = ui->widthLineEdit->text();
        ui->zoomLineEdit->text().isEmpty()    ? listOfProfiles[overwritenProfileIndex].zoom    = -1 : listOfProfiles[overwritenProfileIndex].zoom    = ui->zoomLineEdit->text();

        listOfProfiles[overwritenProfileIndex].listOfLayers.clear();
        if (ui->diffuseLayerCheckBox->isChecked()) {listOfProfiles[overwritenProfileIndex].listOfLayers << "diffuse";}
        if (ui->normalLayerCheckBox->isChecked())  {listOfProfiles[overwritenProfileIndex].listOfLayers << "normal";}
        if (ui->offsetLayerCheckBox->isChecked())  {listOfProfiles[overwritenProfileIndex].listOfLayers << "offset";}

        listOfProfiles[overwritenProfileIndex].profileRotations.clear();
        for (int index = 0; index < ui->rotationsListWidget->count(); ++index)
        {
            listOfProfiles[overwritenProfileIndex].profileRotations << ui->rotationsListWidget->item(index)->text();
        }

        addAllProfilesToFile();
    }
    else
    {
        Profile newProfile;
        newProfile.profileName = dialogWindow.getProfileName();

        ui->angleLineEdit->text().isEmpty()  ? newProfile.angle  = -1 : newProfile.angle  = ui->angleLineEdit->text();
        ui->heightLineEdit->text().isEmpty() ? newProfile.height = -1 : newProfile.height = ui->heightLineEdit->text();
        ui->widthLineEdit->text().isEmpty()  ? newProfile.width  = -1 : newProfile.width  = ui->widthLineEdit->text();
        ui->zoomLineEdit->text().isEmpty()   ? newProfile.zoom   = -1 : newProfile.zoom   = ui->zoomLineEdit->text();

        if (ui->diffuseLayerCheckBox->isChecked()) {newProfile.listOfLayers << "diffuse";}
        if (ui->normalLayerCheckBox->isChecked())  {newProfile.listOfLayers << "normal";}
        if (ui->offsetLayerCheckBox->isChecked())  {newProfile.listOfLayers << "offset";}

        for (int index = 0; index < ui->rotationsListWidget->count(); ++index)
        {
            newProfile.profileRotations << ui->rotationsListWidget->item(index)->text();
        }

        listOfProfiles << newProfile;
        ui->profilesComboBox->addItem(newProfile.profileName);
        currentProfile = newProfile;
        ui->profilesComboBox->setCurrentIndex(ui->profilesComboBox->count()-1);
        addProfileToFile(newProfile);
    }
}

QStringList MainWindow::getProfileNames()
{
    QStringList list;
    for (int index = 0; index < listOfProfiles.size(); ++index) {
        list << listOfProfiles[index].profileName;
    }
    return list;
}

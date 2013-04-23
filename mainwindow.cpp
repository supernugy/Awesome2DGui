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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    order = Qt::AscendingOrder;

    this->setWindowTitle("Prerenderer");

    profilesFile=new QFile("guiProfiles.txt");

    setValidators();

    if (!profilesFile->exists())
    {
        generateGuiProfileFile();
    }
    else
    {
        loadProfilesFromFile();
    }

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        ui->profilesComboBox->addItem(listOfProfiles.value(profileIndex).profileName);
    }

    ui->profilesComboBox->setCurrentIndex(0);

    currentProfile = listOfProfiles.value(0);

    loadCurrentProfileToGui();
}

/**
 * @brief Sets input validators to lineEdits in gui
 */
void MainWindow::setValidators(){

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

/**
 * @brief Loads all profiles from "guiProfiles.txt" into the listOfProfiles
 */
void MainWindow::loadProfilesFromFile()
{

    profilesFile->open(QIODevice::ReadOnly);

    QTextStream stream(profilesFile);

    QString line = stream.readLine();

    while(!stream.atEnd())
    {

        Profile readProfile;

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
        readProfile.layer   = stream.readLine();

        stream.readLine();
        line = stream.readLine();

        QStringList readRotations;

        while(line != "end")
        {
            readRotations << line;
            line = stream.readLine();
        }

        stream.readLine();

        line = stream.readLine();

        readProfile.profileRotations = readRotations;

        listOfProfiles << readProfile;

    }

    profilesFile->close();

}

/**
 * @brief Generates "guiProfiles.txt" file with a default profile
 */
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

    defaultProfile.layer       = "0";

    QStringList defaultRotations;

    defaultRotations << "0"   << "45"  << "90"  << "135"
                     << "180" << "225" << "270" << "315";

    defaultProfile.profileRotations = defaultRotations;

    listOfProfiles << defaultProfile;

    stream << defaultProfile.profileName + "\nAngle\n"
            + defaultProfile.angle       + "\nHeight\n"
            + defaultProfile.height      + "\nWidth\n"
            + defaultProfile.width       + "\nZoom\n"
            + defaultProfile.zoom        + "\nLayer\n"
            + defaultProfile.layer       + "\nrotations\n";

    for (int rotationIndex = 0; rotationIndex < defaultProfile.profileRotations.size(); ++rotationIndex)
    {
        stream << defaultProfile.profileRotations.value(rotationIndex) + "\n";
    }

    stream << "end" << "\n" << "\n";

    stream.flush();

    profilesFile->close();

}

/**
 * @brief Sets values in lineEdits according to values in currentProfile
 */
void MainWindow::loadCurrentProfileToGui(){

    ui->rotationsListWidget->clear();

    for (int rotationIndex = 0; rotationIndex < currentProfile.profileRotations.size(); ++rotationIndex)
    {
        QListWidgetItem *item = new QListWidgetItem(ui->rotationsListWidget, 0);

        item->setData(Qt::DisplayRole, currentProfile.profileRotations.value(rotationIndex).toInt());

    }

    if(currentProfile.angle != "-1")
    {
        ui->angleLineEdit->setText(currentProfile.angle);
    }

    if(currentProfile.height != "-1")
    {
        ui->heightLineEdit->setText(currentProfile.height);
    }

    if(currentProfile.width != "-1")
    {
        ui->widthLineEdit->setText(currentProfile.width);
    }

    if(currentProfile.zoom != "-1")
    {
        ui->zoomLineEdit->setText(currentProfile.zoom);
    }

    ui->layersComboBox->setCurrentIndex(currentProfile.layer.toInt());

    ui->rotationsListWidget->setSortingEnabled(true);

    ui->rotationsListWidget->sortItems(order);
}

/**
 * @brief Adds new profile to the end of an existing "guiProfiles.txt" file
 * @param newProfile - New profile with new parameters
 */
void MainWindow::addProfileToFile(const Profile &newProfile){

    profilesFile->open(QIODevice::Append);

    QTextStream stream(profilesFile);

    stream << newProfile.profileName + "\nAngle\n"
            + newProfile.angle       + "\nHeight\n"
            + newProfile.height      + "\nWidth\n"
            + newProfile.width       + "\nZoom\n"
            + newProfile.zoom        + "\nLayer\n"
            + newProfile.layer       + "\nrotations\n";

    for (int rotationIndex = 0; rotationIndex < newProfile.profileRotations.size(); ++rotationIndex)
    {
        stream << newProfile.profileRotations.value(rotationIndex) + "\n";
    }

    stream << "end" << "\n" << "\n";

    stream.flush();

    profilesFile->close();

}

/**
 * @brief Rewrites whole "guiProfiles.txt" file with all profiles in listOfProfiles
 */
void MainWindow::addAllProfilesToFile(){

    profilesFile->open(QIODevice::WriteOnly);

    QTextStream stream(profilesFile);

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        stream << listOfProfiles.value(profileIndex).profileName + "\nAngle\n"
                + listOfProfiles.value(profileIndex).angle       + "\nHeight\n"
                + listOfProfiles.value(profileIndex).height      + "\nWidth\n"
                + listOfProfiles.value(profileIndex).width       + "\nZoom\n"
                + listOfProfiles.value(profileIndex).zoom        + "\nLayer\n"
                + listOfProfiles.value(profileIndex).layer       + "\nrotations\n";

        for (int rotationIndex = 0; rotationIndex < listOfProfiles.value(profileIndex).profileRotations.size(); ++rotationIndex)
        {
            stream << listOfProfiles.value(profileIndex).profileRotations.value(rotationIndex) + "\n";
        }

        stream << "end" << "\n" << "\n";
    }

    stream.flush();

    profilesFile->close();

}

/**
 * @brief Updates profile comboBox by clearing it and than adding all profile names into it
 * @param selectedIndex - comboBox index that will be set as current
 */
void MainWindow::updateProfileCombobox(int selectedIndex){

    ui->profilesComboBox->clear();

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        ui->profilesComboBox->addItem(listOfProfiles.value(profileIndex).profileName);
    }

    ui->profilesComboBox->setCurrentIndex(selectedIndex);

}


MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief Opens file dialog window that accepts only .obj files
 */
void MainWindow::on_objSelectButton_clicked()
{

    QString objName = QFileDialog::getOpenFileName(this,tr("Select .obj File"),"/path/to/file/",tr("Obj Files (*.obj)"));

    ui->objectPathTextField->setText(objName);

}

/**
 * @brief Opens file dialog window that accepts only .png files
 */
void MainWindow::on_pngSelectButton_clicked()
{

    QString pngName = QFileDialog::getOpenFileName(this,tr("Select .png File"),"/path/to/file/",tr("Png Files (*.png)"));

    ui->texturePathTextField->setText(pngName);

}

/**
 * @brief Launches prerenderer with parameters currently displayed in gui
 */
void MainWindow::on_renderButton_clicked()
{

    if(ui->objectPathTextField->text().isEmpty())
    {
        ui->label->setText("Select .obj file");
        return;
    }

    QStringList arguments("render");

    if(!ui->texturePathTextField->text().isEmpty())
    {
        arguments << "--texture="+ui->texturePathTextField->text();
    }

    if(!ui->widthLineEdit->text().isEmpty())
    {
        arguments << "--width="+ui->widthLineEdit->text();
    }

    if(!ui->heightLineEdit->text().isEmpty())
    {
        arguments << "--height="+ui->heightLineEdit->text();
    }

    if(!ui->angleLineEdit->text().isEmpty())
    {
        arguments << "--angle="+ui->angleLineEdit->text();
    }

    if(!ui->zoomLineEdit->text().isEmpty())
    {
        arguments << "--zoom="+ui->zoomLineEdit->text();
    }

    arguments << ui->objectPathTextField->text();

    QProcess *proces = new QProcess(this);

    QObject::connect(proces,SIGNAL(finished(int)),this,SLOT(prerendere_finished()));

    proces->start("./prerenderer-debug",arguments);

}

/**
 * @brief This slot is started after prerenderering .obj file. Shows ImageDisplayForm.
 */
void MainWindow::prerendere_finished()
{
    ui->label->setText("Done");

    QStringList parts = ui->objectPathTextField->text().split("/");

    QString imageDir = parts.at(parts.size()-1);

    parts.clear();

    parts = imageDir.split(".");

    imageDir.clear();

    for (int index = 0; index < parts.size()-1; ++index)
    {
       imageDir.append(parts.value(index));
       imageDir.append(".");
    }

    imageDir.chop(1);

    imageDir.append("_prerender");

    QString pathToImageDir;

    pathToImageDir = QApplication::applicationDirPath();

    pathToImageDir.append("/");

    pathToImageDir.append(imageDir);

    pathToImageDir.append("/");

    ImageDisplayForm *displayForm = new ImageDisplayForm(0,pathToImageDir);

    displayForm->show();
}

/**
 * @brief Adds new rotation from lineEdit to the list of rotations of currentProfile
 */
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

/**
 * @brief Removes rotation selected in rotationsListWidget from currentProfile
 */
void MainWindow::on_removeRotationButton_clicked()
{
    QList<QListWidgetItem *> selectedRotations = ui->rotationsListWidget->selectedItems();

    for (int selectedRotationIndex = 0; selectedRotationIndex < selectedRotations.size(); ++selectedRotationIndex)
    {

        for (int rotationIndex = 0; rotationIndex < currentProfile.profileRotations.size(); ++rotationIndex)
        {

            QString item=selectedRotations.value(selectedRotationIndex)->text();

            if(currentProfile.profileRotations.value(rotationIndex).compare(item) == 0)
            {
                currentProfile.profileRotations.removeAt(rotationIndex);
                break;
            }

        }

    }

    qDeleteAll(selectedRotations);

}

/**
 * @brief Changes currentProfile to one selected in comboBox and loads it to the gui
 * @param selectedProfile - name of profile selected in comboBox
 */
void MainWindow::on_profilesComboBox_currentIndexChanged(const QString &selectedProfile)
{

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {

        if (listOfProfiles.value(profileIndex).profileName == selectedProfile)
        {
            currentProfile = listOfProfiles.value(profileIndex);

            loadCurrentProfileToGui();
        }

    }

}

/**
 * @brief Opens a dialog window. If dialog accepts then it will add new profile with parameters in gui
 *        to the listOfProfiles as weel as "guiProfiles.txt"
 */
void MainWindow::on_addProfileButton_clicked()
{

    ProfileDialog dialogWindow(this, "", "Do you want to add new profile with current parameters?", true);

    dialogWindow.setWindowTitle("Add new profile");

    dialogWindow.show();

    if(dialogWindow.exec() != QDialog::Accepted)
    {
        return;
    }

    Profile newProfile;

    newProfile.profileName = dialogWindow.getProfileName();

    ui->angleLineEdit->text().isEmpty()  ? newProfile.angle  = -1 : newProfile.angle  = ui->angleLineEdit->text();

    ui->heightLineEdit->text().isEmpty() ? newProfile.height = -1 : newProfile.height = ui->heightLineEdit->text();

    ui->widthLineEdit->text().isEmpty()  ? newProfile.width  = -1 : newProfile.width  = ui->widthLineEdit->text();

    ui->zoomLineEdit->text().isEmpty()   ? newProfile.zoom   = -1 : newProfile.zoom   = ui->zoomLineEdit->text();

    QString layerIndex;

    layerIndex.setNum(ui->layersComboBox->currentIndex());

    newProfile.layer=layerIndex;

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

/**
 * @brief Opens a dialog window. If dialog accepts then it will edit current profile with parameters in gui
 *        and apply changes in "guiProfiles.txt"
 */
void MainWindow::on_editProfileButton_clicked()
{
    const QString oldName = currentProfile.profileName;

    int currentComboBoxIndex = ui->profilesComboBox->currentIndex();

    ProfileDialog dialogWindow(this, oldName, "Do you want to edit profile with current parameters?", true);

    dialogWindow.setWindowTitle("Edit profile");

    dialogWindow.show();

    if(dialogWindow.exec() != QDialog::Accepted)
    {
        return;
    }

    currentProfile.profileName = dialogWindow.getProfileName();

    ui->angleLineEdit->text().isEmpty()  ? currentProfile.angle  = -1 : currentProfile.angle  = ui->angleLineEdit->text();

    ui->heightLineEdit->text().isEmpty() ? currentProfile.height = -1 : currentProfile.height = ui->heightLineEdit->text();

    ui->widthLineEdit->text().isEmpty()  ? currentProfile.width  = -1 : currentProfile.width  = ui->widthLineEdit->text();

    ui->zoomLineEdit->text().isEmpty()   ? currentProfile.zoom   = -1 : currentProfile.zoom   = ui->zoomLineEdit->text();

    QString layerIndex;

    layerIndex.setNum(ui->layersComboBox->currentIndex());

    currentProfile.layer = layerIndex;

    currentProfile.profileRotations.clear();

    for (int index = 0; index < ui->rotationsListWidget->count(); ++index)
    {
        currentProfile.profileRotations << ui->rotationsListWidget->item(index)->text();
    }

    loadCurrentProfileToGui();

    for (int profileIndex = 0; profileIndex < listOfProfiles.size(); ++profileIndex)
    {
        if(listOfProfiles.value(profileIndex).profileName == oldName)
        {
            listOfProfiles.replace(profileIndex,currentProfile);
            break;
        }
    }

    updateProfileCombobox(currentComboBoxIndex);

    addAllProfilesToFile();

}

/**
 * @brief Opens a dialog window. If dialog accepts then it will remove current profile from listOfProfiles
 *        as well as from "guiProfiles.txt"
 */
void MainWindow::on_removeProfileButton_clicked()
{

    ProfileDialog dialogWindow(this, "", "Do you want remove current profile?", false);

    dialogWindow.setWindowTitle("Remove profile");

    dialogWindow.show();

    if(dialogWindow.exec() != QDialog::Accepted)
    {
        return;
    }

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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QStringList>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QStringList getProfileNames();
    
private slots:

    //! Opens file dialog window that accepts only .obj files
    void on_objSelectButton_clicked();

    //! Opens file dialog window that accepts only .png files
    void on_pngSelectButton_clicked();

    //! Launches prerenderer with parameters currently displayed in gui
    void on_renderButton_clicked();

    //! Adds new rotation from lineEdit to the list of rotations of currentProfile
    void on_addRotationButton_clicked();

    //! Removes rotation selected in rotationsListWidget from currentProfile
    void on_removeRotationButton_clicked();

    //! Changes currentProfile to one selected in comboBox and loads it to the gui
    //!
    //! @param selectedProfile - name of profile selected in comboBox
    void on_profilesComboBox_currentIndexChanged(const QString &selectedProfile);

    //! Opens a dialog window. If dialog accepts then it will remove current profile
    //! from listOfProfiles as well as from "guiProfiles.txt"
    void on_removeProfileButton_clicked();

    //! This slot is started after prerenderering .obj file. Shows ImageDisplayForm.
    void prerenderer_finished();

    //! Opens a dialog window with line edit. If profile name in line edit is
    //! new the it will create new profile else it will ask for overwrite.
    void on_saveProfileButton_clicked();

private:

    struct Profile
    {
        QString profileName;
        QString height;
        QString width;
        QString angle;
        QString zoom;
        QStringList listOfLayers;
        QStringList profileRotations;
    };

    //! Generates "guiProfiles.txt" file with a default profile
    void generateGuiProfileFile();

    //! Sets input validators to lineEdits in gui
    void setValidators();

    //! Loads all profiles from "guiProfiles.txt" into the listOfProfiles
    void loadProfilesFromFile();

    //! Sets values in lineEdits according to values in currentProfile
    void loadCurrentProfileToGui();

    //! Adds new profile to the end of an existing "guiProfiles.txt" file
    void addProfileToFile(const Profile& newProfile);

    //! Rewrites whole "guiProfiles.txt" file with all profiles in listOfProfiles
    void addAllProfilesToFile();

    //! Updates profile comboBox by clearing it and then adding all profile names into it
    //!
    //! @param selectedIndex - comboBox index that will be set as current
    void updateProfileCombobox(int selectedIndex);

    Ui::MainWindow *ui;

    Profile currentProfile;

    QList<Profile> listOfProfiles;

    Qt::SortOrder rotationListWidgetSortOrder;

    QFile *profilesFile;

    QString previouslyUsedDirectory;
};

#endif // MAINWINDOW_H

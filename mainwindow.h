#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_objSelectButton_clicked();

    void on_pngSelectButton_clicked();

    void on_renderButton_clicked();

    void on_addRotationButton_clicked();

    void on_removeRotationButton_clicked();

    void on_profilesComboBox_currentIndexChanged(const QString &selectedProfile);

    void on_addProfileButton_clicked();

    void on_editProfileButton_clicked();

    void on_removeProfileButton_clicked();

    void prerendere_finished();

private:

    struct Profile{
        QString profileName, height, width, angle, zoom, layer;
        QStringList profileRotations;
    };

    QFile *profilesFile;

    Qt::SortOrder order;

    QList<Profile> listOfProfiles;

    Profile currentProfile;

    void generateGuiProfileFile();

    void setValidators();

    void loadProfilesFromFile();

    void loadCurrentProfileToGui();

    void addProfileToFile(const Profile& newProfile);

    void addAllProfilesToFile();

    void updateProfileCombobox(int selectedIndex);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

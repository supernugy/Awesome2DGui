#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

struct profile{
    QString profileName, height, width, angle, zoom, layer;
    QStringList profileRotations;
};

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

    void on_profilesComboBox_currentIndexChanged(const QString &arg1);

    void on_addProfileButton_clicked();

private:

    void generateGuiProfileFile();

    void setValidators();

    void loadProfilesFromFile();

    void loadCurrentProfileToGui();

    void addProfileToFile(profile newProfile);

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

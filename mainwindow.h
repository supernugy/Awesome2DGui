#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void generateGuiProfileFile();

    void setValidators();

    void loadProfilesFromFile();

    void loadCurrentProfileToGui();

    void on_objSelectButton_clicked();

    void on_pngSelectButton_clicked();

    void on_renderButton_clicked();

    void on_addRotationButton_clicked();

    void on_removeRotationButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H

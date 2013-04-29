#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QList>
#include "mainwindow.h"

namespace Ui {
class ProfileDialog;
}

class ProfileDialog : public QDialog
{
    Q_OBJECT
    
public:
    //! This constructor is called from mainWindow in save and remove profile cases
    explicit ProfileDialog(MainWindow *parent = 0, const QString& name = "", const QString& message = "", bool setLineEditAndLabelVisible = true);

    //! This constructor is called from this profile dialog in case of overwrite
    explicit ProfileDialog(QWidget *parent = 0, const QString& name = "", const QString& message = "", bool setLineEditAndLabelVisible = true);

    ~ProfileDialog();

    const QString& getProfileName() const;

    bool isOverwriten();
    
private slots:

    //! If lineEdit is visible and not empty accept else refuse
    //! If lineEdit is not visible then accept
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ProfileDialog *ui;

    QString selectedProfileName;

    MainWindow *parentWindow;

    bool overwrite;

};

#endif // PROFILEDIALOG_H

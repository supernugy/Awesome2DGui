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
    explicit ProfileDialog(MainWindow *parent = 0, const QString& name = "", const QString& message = "", bool setLineEditAndLabelVisible = true);

    explicit ProfileDialog(QWidget *parent = 0, const QString& name = "", const QString& message = "", bool setLineEditAndLabelVisible = true);

    ~ProfileDialog();

    const QString& getProfileName() const;

    bool isOverwriten();
    
private slots:

    //! If lineEdit is not empty accept else refuse
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ProfileDialog *ui;

    QString selectedProfileName;

    MainWindow *parentWindow;

    bool overwrite;

};

#endif // PROFILEDIALOG_H

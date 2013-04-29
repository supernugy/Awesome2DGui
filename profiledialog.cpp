#include "profiledialog.h"
#include "ui_profiledialog.h"
#include "mainwindow.h"
#include <QList>
#include <QStringList>

ProfileDialog::ProfileDialog(MainWindow *parent, const QString& name, const QString& message, bool setLineEditAndLabelVisible) :
    QDialog(parent),
    ui(new Ui::ProfileDialog),
    parentWindow(parent)
{
    ui->setupUi(this);
    ui->messageLabel->setText(message);
    ui->profileNameLineEdit->setText(name);
    ui->profileNameLineEdit->setVisible(setLineEditAndLabelVisible);
    ui->profileNameLabel->setVisible(setLineEditAndLabelVisible);
}

ProfileDialog::ProfileDialog(QWidget *parent, const QString& name, const QString& message, bool setLineEditAndLabelVisible) :
    QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    ui->setupUi(this);
    ui->messageLabel->setText(message);
    ui->profileNameLineEdit->setText(name);
    ui->profileNameLineEdit->setVisible(setLineEditAndLabelVisible);
    ui->profileNameLabel->setVisible(setLineEditAndLabelVisible);
}

ProfileDialog::~ProfileDialog()
{
    delete ui;
}

const QString &ProfileDialog::getProfileName() const
{
     return selectedProfileName;
}

bool ProfileDialog::isOverwriten()
{
    return overwrite;
}

void ProfileDialog::on_okButton_clicked()
{
    // This if is for save profile case (profileNameLineEdit is visible)
    if(!ui->profileNameLineEdit->text().isEmpty() && ui->profileNameLineEdit->isVisible())
    {
        QStringList listOfNames(parentWindow->getProfileNames());
        if(listOfNames.contains(ui->profileNameLineEdit->text()))
        {
            ProfileDialog dialogWindow(this, "", "Overwrite?", false);
            dialogWindow.setWindowTitle("Overwrite");
            dialogWindow.show();
            if(dialogWindow.exec() == QDialog::Accepted)
            {
                selectedProfileName = ui->profileNameLineEdit->text();
                overwrite = true;
                this->accept();
            }
        }
        else
        {
            selectedProfileName = ui->profileNameLineEdit->text();
            overwrite = false;
            this->accept();
        }
    }
    /*
     * This if is for remove profile and overwrite profile cases (no profileNameLineEdit).
     * This if need to be here because if we are going to save profile and we click
     * the OK button with empty profileNameLineEdit it will accept and it
     * shouldn't do that...
     */
    else if(!ui->profileNameLineEdit->isVisible())
    {
        this->accept();
    }
}

void ProfileDialog::on_cancelButton_clicked()
{
    this->reject();
}


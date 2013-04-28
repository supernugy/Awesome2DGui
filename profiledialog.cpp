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
    else if(!ui->profileNameLineEdit->isVisible())
    {
        this->accept();
    }
}

void ProfileDialog::on_cancelButton_clicked()
{
    this->reject();
}


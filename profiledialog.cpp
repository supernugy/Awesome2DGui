#include "profiledialog.h"
#include "ui_profiledialog.h"

ProfileDialog::ProfileDialog(QWidget *parent,const QString& name, const QString& message, bool setLineEditVisible) :
    QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    ui->setupUi(this);

    ui->messageLabel->setText(message);

    if(name != "")
    {
        ui->profileNameLineEdit->setText(name);
    }

    ui->profileNameLineEdit->setVisible(setLineEditVisible);
    ui->profileNameLabel->setVisible(setLineEditVisible);

}

ProfileDialog::~ProfileDialog()
{
    delete ui;
}

 const QString &ProfileDialog::getProfileName() const
 {
     return selectedProfileName;
 }

void ProfileDialog::on_okButton_clicked()
{
    if(!ui->profileNameLineEdit->text().isEmpty()&&ui->profileNameLineEdit->isVisible()){
        selectedProfileName=ui->profileNameLineEdit->text();
        this->accept();
    }else if(!ui->profileNameLineEdit->isVisible()){
        this->accept();
    }
}

void ProfileDialog::on_cancelButton_clicked()
{
    this->reject();
}


#include "profiledialog.h"
#include "ui_profiledialog.h"

QString profileName;

ProfileDialog::ProfileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProfileDialog)
{
    ui->setupUi(this);

}

ProfileDialog::~ProfileDialog()
{
    delete ui;
}

void ProfileDialog::setMessageLabel(QString message){
    ui->messageLabel->setText(message);
}

void ProfileDialog::on_okButton_clicked()
{
    if(!ui->profileNameLineEdit->text().isEmpty()){
        profileName=ui->profileNameLineEdit->text();
        this->accept();
    }
}

void ProfileDialog::on_cancelButton_clicked()
{
    this->reject();
}

void ProfileDialog::setName(QString name){
    ui->profileNameLineEdit->setText(name);
}

#include "imagedisplayform.h"
#include "ui_imagedisplayform.h"
#include <QListWidgetItem>
#include <QString>
#include <QStringList>
#include <QDir>

ImageDisplayForm::ImageDisplayForm(QWidget *parent, QString imagePath) :
    QWidget(parent),
    ui(new Ui::ImageDisplayForm)
{
    ui->setupUi(this);
    path=imagePath;
    this->setWindowTitle("Image viewer");
    QStringList nameFilter("*.png");
    QDir directory(imagePath);
    QStringList pngFiles = directory.entryList(nameFilter);
    ui->imageListWidget->addItems(pngFiles);
    ui->imageListWidget->setMinimumWidth(ui->imageListWidget->sizeHintForColumn(0)+5);
}

ImageDisplayForm::~ImageDisplayForm()
{
    delete ui;
}

void ImageDisplayForm::on_imageListWidget_itemClicked(QListWidgetItem *item)
{
    const QPixmap pixMap(path+item->text());
    ui->imageLabel->setPixmap(pixMap);
}

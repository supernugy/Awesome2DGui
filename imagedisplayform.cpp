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

}

ImageDisplayForm::~ImageDisplayForm()
{
    delete ui;
}

/**
 * @brief Displays .png image after clicking on it in the list
 */
void ImageDisplayForm::on_imageListWidget_itemClicked(QListWidgetItem *item)
{
    const QPixmap pixMap(path+item->text());

    ui->imageLabel->setPixmap(pixMap);
}

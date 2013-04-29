#ifndef IMAGEDISPLAYFORM_H
#define IMAGEDISPLAYFORM_H

#include <QWidget>
#include <QListWidgetItem>
#include <QString>

namespace Ui {
class ImageDisplayForm;
}

class ImageDisplayForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageDisplayForm(QWidget *parent = 0, QString imagePath="");
    ~ImageDisplayForm();
    
private slots:
    //! Displays .png image after clicking on it in the list
    void on_imageListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::ImageDisplayForm *ui;

    QString path;
};

#endif // IMAGEDISPLAYFORM_H

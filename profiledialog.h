#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>

namespace Ui {
class ProfileDialog;
}

class ProfileDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ProfileDialog(QWidget *parent = 0);
    ~ProfileDialog();

    void setMessageLabel(QString message);

    void setName(QString name);

    void setLineEditVisible(bool en);

    QString profileName;
    
private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ProfileDialog *ui;
};

#endif // PROFILEDIALOG_H

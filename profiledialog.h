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
    explicit ProfileDialog(QWidget *parent = 0, const QString& name = "", const QString& message = "", bool setLineEditAndLabelVisible = true);

    ~ProfileDialog();

    const QString& getProfileName() const;
    
private slots:

    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ProfileDialog *ui;

    QString selectedProfileName;
};

#endif // PROFILEDIALOG_H

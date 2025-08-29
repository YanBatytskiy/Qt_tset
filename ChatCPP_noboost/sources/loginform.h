#ifndef LOGINFORM_H
#define LOGINFORM_H

#include <QWidget>
#include <memory>
#include "Database.h"

namespace Ui {
class loginForm;
}

class loginForm : public QWidget
{
    Q_OBJECT

public:
    explicit loginForm(QWidget *parent = nullptr);
    ~loginForm();
    void setDatabase(std::shared_ptr<Database> dbPtr);


signals:
    void registerRequested();
    void accepted(int userId, QString userName);
    void rejected();


private slots:
    void on_registrationButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::loginForm *ui;
    std::shared_ptr<Database> m_dbPtr;

};

#endif // LOGINFORM_H

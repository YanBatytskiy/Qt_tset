#ifndef REGISTRATIONFORM_H
#define REGISTRATIONFORM_H

#include <QWidget>
#include <memory>
#include "Database.h"

namespace Ui {
class registrationForm;
}

class registrationForm : public QWidget
{
    Q_OBJECT

public:
    explicit registrationForm(QWidget *parent = nullptr);
    ~registrationForm();
    void setDatabase(std::shared_ptr<Database> dbPtr);

signals:
    void loginRequested();
    void accepted(int userId, QString userName);
    void rejected();

private slots:
    void on_loginButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

private:
    Ui::registrationForm *ui;
    std::shared_ptr<Database> m_dbPtr;
};

#endif // REGISTRATIONFORM_H

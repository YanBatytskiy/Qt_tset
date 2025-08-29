#include "loginform.h"
#include "ui_loginform.h"
#include <QMessageBox>

loginForm::loginForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::loginForm)
{
    ui->setupUi(this);
}

loginForm::~loginForm()
{
    delete ui;
}

void loginForm::setDatabase(std::shared_ptr<Database> dbPtr)
{
    m_dbPtr = dbPtr;

}

void loginForm::on_registrationButton_clicked()
{
    emit registerRequested();
}


void loginForm::on_buttonBox_accepted()
{

    auto userId = m_dbPtr->checkPassword(ui->loginEdit->text().toStdString(),
                                         ui->passwordEdit->text().toStdString());
    if(userId = -1)
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Password is wrong"));
        return;
    }

    emit accepted(userId, ui->loginEdit->text());
}


void loginForm::on_buttonBox_rejected()
{
    emit rejected();
}


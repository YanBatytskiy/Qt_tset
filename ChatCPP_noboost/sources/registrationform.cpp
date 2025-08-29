#include "registrationform.h"
#include "ui_registrationform.h"
#include <QMessageBox>


registrationForm::registrationForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::registrationForm)
{
    ui->setupUi(this);
}

registrationForm::~registrationForm()
{
    delete ui;
}

void registrationForm::setDatabase(std::shared_ptr<Database> dbPtr)
{
    m_dbPtr = dbPtr;
}



void registrationForm::on_loginButton_clicked()
{
    emit loginRequested();
}


void registrationForm::on_buttonBox_accepted()
{
    if (ui->passwordEdit->text() !=
        ui->passwordConfirmEdit->text())
    {
        QMessageBox::critical(this, tr("Error"),
                              tr("Password not match"));
        return;
    }
    auto userId = m_dbPtr->addUser(ui->loginEdit->text().toStdString(),
                     ui->passwordEdit->text().toStdString());

    switch(userId)
    {
    case -1:
        QMessageBox::critical(this, tr("Error"),
                              tr("Incorrect login"));
        return;
    case -2:
        QMessageBox::critical(this, tr("Error"),
                              tr("Login already exists"));
        return;

    default:
        emit accepted(userId, ui->loginEdit->text());
    }
}


void registrationForm::on_buttonBox_rejected()
{
    emit rejected();
}


#include "startscreen.h"
#include "ui_startscreen.h"

StartScreen::StartScreen(std::shared_ptr<Database> dbPtr, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::StartScreen)
{
    ui->setupUi(this);

    if (dbPtr)
        m_dbPtr = dbPtr;
    else m_dbPtr = make_shared<Database>();

    ui->loginWidget->setDatabase(m_dbPtr);
    ui->registerWidget->setDatabase(m_dbPtr);


    connect(ui->loginWidget, &loginForm::registerRequested, this, &StartScreen::setRegistrationForm);
    connect(ui->loginWidget, &loginForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->loginWidget, &loginForm::rejected, this, &StartScreen::onRejectedRequested);

    connect(ui->registerWidget, &registrationForm::loginRequested, this, &StartScreen::setLoginForm);
    connect(ui->registerWidget, &registrationForm::accepted, this, &StartScreen::onLoggedIn);
    connect(ui->registerWidget, &registrationForm::rejected, this, &StartScreen::onRejectedRequested);

}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void StartScreen::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StartScreen::onLoggedIn(uint userId, QString userName)
{
    m_userId = userId;
    m_userName = userName;
    accept();
}

void StartScreen::onRejectedRequested()
{
    reject();
}

std::shared_ptr<Database> StartScreen::getDatabase() const
{
    return m_dbPtr;
}

QString StartScreen::userName() const
{
    return m_userName;
}

int StartScreen::userId() const
{
    return m_userId;
}

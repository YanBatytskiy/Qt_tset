#include "mainwindow.h"
#include "startscreen.h"
#include "ui_mainwindow.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QTimer>

int MainWindow::kInstanceCount = 0;

MainWindow::MainWindow(int userId, QString userName, std::shared_ptr<Database> dbPtr, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), m_userId(userId), m_userName(userName)
{
    ui->setupUi(this);
    kInstanceCount++;

    if (dbPtr)
        m_dbPtr = dbPtr;
    else m_dbPtr = make_shared<Database>();
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChats);
    timer->start(10);
}

MainWindow::~MainWindow()
{
    delete ui;
    kInstanceCount--;
    if (kInstanceCount <= 0)
        qApp->exit(0);
}

MainWindow *MainWindow::createClient(std::shared_ptr<Database> dbPtr)
{
    StartScreen s(dbPtr);
    auto result = s.exec();
    if (result == QDialog::Rejected)
    {
        return nullptr;
    }

    auto w = new MainWindow(s.userId(), s.userName(), s.getDatabase());
    w->setAttribute(Qt::WA_DeleteOnClose);
    return w;
}

void MainWindow::on_messageLineEdit_returnPressed()
{
    on_sendMessageButton_clicked();
}


void MainWindow::on_sendMessageButton_clicked()
{
    m_dbPtr->addChatMessage(m_userName.toStdString(),
                            ui->messageLineEdit->text().toStdString() );
}


void MainWindow::on_privateMessageSendButton_clicked()
{

    QDialog dial(this);
    dial.setModal(true);
    auto *l = new QVBoxLayout();
    dial.setLayout(l);
    auto userListWgt = new QListWidget(&dial);
    l->addWidget(userListWgt);
    auto ButtonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dial);
    l->addWidget(ButtonBox);

    connect(ButtonBox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
    connect(ButtonBox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);

    auto userList = m_dbPtr->getUserList();
    for (const auto& user : userList) {
        userListWgt->addItem(QString::fromStdString(user));
    }

    userListWgt->setCurrentRow(0);

    auto result = dial.exec();

    if (result == QDialog::Accepted && userListWgt->currentItem()){
        m_dbPtr->addPrivateMessage(m_userName.toStdString(), userListWgt->currentItem()->text().toStdString(), ui->messageLineEdit->text().toStdString());
    }

    // m_dbPtr->addChatMessage(m_userName.toStdString(),
                            // ui->messageLineEdit->text().toStdString() );

}


void MainWindow::on_actionOpen_another_client_triggered()
{
    auto w = createClient(m_dbPtr);
    if (w)
    w->show();
}


void MainWindow::on_actionClose_this_client_triggered()
{
    this->close();
}

void MainWindow::updateChats()
{
    auto chatMessages = m_dbPtr->getChatMessages();
    QString chat;
    for (const auto &msg : chatMessages)
    {
        chat.append(QString::fromStdString(msg)+ "\n");
    }
    if (ui->commonChatBrowser->toPlainText() != chat)
    ui->commonChatBrowser->setText(chat);

    chat.clear();
    auto privateMessages = m_dbPtr->getPrivateMessage();
    for (const auto& msg : privateMessages)
    {
        if (QString::fromStdString(msg.getSender()) != m_userName && msg.getDest() != m_userId)
            continue;
        QString prefix;
        if (m_userName == QString::fromStdString(msg.getSender()) && m_userId == msg.getDest()){
            prefix = tr("self message") + ": ";
        }
        else if (m_userName == QString::fromStdString(msg.getSender()))
                   {
            prefix = tr("messagr to") + QString(" <%1>:  ").
                        arg(QString::fromStdString(m_dbPtr->getUserName(msg.getDest())));
                   }
        else
        {
            prefix = "<"+ QString::fromStdString(msg.getSender()) + ">: " + tr("say to you") + ": ";
                   }
                   chat.append(prefix + QString::fromStdString(msg.getText())+ "\n");
    }
    auto current = ui->privateChatBrowser->toPlainText();
    if (ui->privateChatBrowser->toPlainText() != chat)
        ui->privateChatBrowser->setText(chat);
}


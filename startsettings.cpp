#include "startsettings.h"

#include <limits.h> // HOST_NAME_MAX

#include<QVBoxLayout>
#include<QHBoxLayout>
#include<QPushButton>

#include<QIntValidator> // check port

#include<QDesktopWidget> // desktop()
#include<QApplication> // QApplication access

#include<QStringList> // for split reply of sam

#include<QDebug>

#include"sockets.hpp"

#define SAM_DEFAULT_HOST "127.0.0.1"
#define SAM_DEFAULT_PORT 7656

static const char
 * samWork = "<b>OK</b>",
 * samNoWork = "<b>ERROR</b>",
 * samWorkStyleOK = "QLabel {color:green;};",
 * samWorkStyleBad = "QLabel {color:red;};";



inline void StartSettings::setLayouts(void){
    vBox = new QVBoxLayout(this);
    hBox = new QHBoxLayout(this);
    this->setLayout(vBox);
    vBox->addWidget(mainText);
    vBox->addWidget(username);
    vBox->addWidget(password);

    vBox->addLayout(hBox);

    hBox->addWidget(host);
    hBox->addWidget(port);
    hBox->addWidget(OK);
    hBox->addWidget(CheckSAM);

    vBox->addWidget(SAMWork);


}
inline void StartSettings::setLabels(void){
    mainText = new QLabel(this),
    SAMWork = new QLabel(this);

    mainText->setFont(QFont("Arial Black",10));
    SAMWork->setFont(QFont("Arial",12));


    mainText->setAlignment( Qt::AlignCenter );
    SAMWork->setAlignment( Qt::AlignCenter );


    mainText->setText(
    tr("Hello is start settings "
    "Here you should set your SAM "
    "host:port ")
                     );



    SAMWork->hide();
    mainText->show();

}
inline void StartSettings::setConnects(void){
    connect(
     CheckSAM,
     SIGNAL(clicked(bool)),
     SLOT( CheckSam() ) );
}

inline void StartSettings::setWindowHint(void){
  //  this->setMinimumHeight(300);
   // this->setMinimumWidth(300);
    this->setWindowTitle( tr("First steps") );
    this->setFixedSize(500,300);

    QRect ScreenRect
         = QApplication::desktop()->screenGeometry();
    this->move(
                ScreenRect.width()/4,
                ScreenRect.height()/4
                );
}



inline void StartSettings::setButtons(void){
    OK = new QPushButton(this),
    CheckSAM = new QPushButton(this);
    OK->setText( tr("OK") );
    CheckSAM->setText( tr("CheckSam") );
}

inline void StartSettings::setLineEdits(void){

    QValidator * isPort
            = new QIntValidator(1, 65535, this);

    host = new QLineEdit(this);
    port = new QLineEdit(this);
    username = new QLineEdit(this);
    password = new QLineEdit(this);


    host->setPlaceholderText( tr("host") );
    port->setPlaceholderText( tr("port") );

    username->setPlaceholderText( tr("username") );
    password->setPlaceholderText( tr("password") );

    password->setEchoMode(QLineEdit::Password);


    port->setValidator(isPort);
    host->setMaxLength(HOST_NAME_MAX);
    username->setMaxLength(30);
    password->setMaxLength(128);

    host->setText(SAM_DEFAULT_HOST);
    port->setText( QString::number(SAM_DEFAULT_PORT) );
}

bool StartSettings::CheckSam(void){


    qDebug() << host->text();
    qDebug() << port->text();

    int fds = MyOwnTCPSocket::Connect(
     (char*)host->text().toStdString().c_str(),
     atoi( port->text().toStdString().c_str() )
                );
    if(fds == -1)
        return false;

    MyOwnTCPSocket::Write(fds,(char*)"HELLO VERSION");

    char * data = MyOwnTCPSocket::Read(fds,40);

    QStringList listdata
        = QString(data).split(" ");

    MyOwnTCPSocket::stopClient(&fds);

    qDebug() << data;
    free((void*)data);

    SAMWork->show();
    qDebug() << listdata[2];
    if(
            listdata.size() > 3
            &&
            listdata[2] == "RESULT=OK"
            ) {
           SAMWork->setStyleSheet(samWorkStyleOK);
           SAMWork->setText(samWork);
           return true;
    }

    SAMWork->setStyleSheet(samWorkStyleBad);
    SAMWork->setText(samNoWork);
    return false;

}

StartSettings::StartSettings(QWidget *parent) :
    QWidget(parent)
{
        this->setWindowHint();
        this->setLabels();
        this->setButtons();
        this->setLineEdits();
        this->setLayouts();
        this->setConnects();
}

StartSettings::~StartSettings(){
   // delete [] this->vBox;
   // delete [] this->hBox;
}
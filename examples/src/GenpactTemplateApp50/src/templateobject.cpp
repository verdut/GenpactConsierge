#include "include/templateobject.h"


TemplateObject::TemplateObject()
{
    Q_INIT_RESOURCE( resources );
}

TemplateObject::~TemplateObject()
{
    stop();
    delete dialog;
}

void TemplateObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );

    dialog = new PromobotDialogModule( parent );
    // Sets the speech synthesis service
    dialog->setTTSService( this->TTSService() );
    // Sets the user database
    dialog->setUsersDatabase( this->databaseUsers() );
    // Sets the macro processor service
    dialog->setMacrosReplacer( this->macrosReplacer() );
    // We connect dialogue signals with plug-in slots

    connectDialogWithOP();
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("remotemysql.com");
    db.setDatabaseName("PeU7vbLfNN");
    db.setUserName("PeU7vbLfNN");
    db.setPassword("zlYzmg6Ikh");
    db.setPort(3306);

    //Pages
        mainWidget = new QWidget;
        mainWidget->setStyleSheet("background-color:white");
        employeeWidget=new QWidget;
        employeeWidget->setStyleSheet("background-color:white");
        widget_gdpr = new QWidget;
        widget_gdpr->setStyleSheet("background-color:white");
        widget_sso=new QWidget();
        widget_sso->setStyleSheet("background-color:white");
        widget_sso->setObjectName("widget_sso");
        widget_sso->installEventFilter(this);
        widget_visitor_data = new QWidget;
        widget_visitor_data->setStyleSheet("background-color:white");
        widget_visitor_data->setObjectName("widget_visitor_data");
        widget_visitor_data->installEventFilter(this);

    //end Pages


    //Stack of pages
        pages = new QStackedWidget;
        pages->addWidget(mainWidget);
        pages->addWidget(widget_visitor_data);
        pages->addWidget(employeeWidget);
        pages->addWidget(widget_gdpr);
        pages->addWidget(widget_sso);
        setCentralWidget(pages);

    //end Stack of pages



    //Main Screen

        QPixmap pixmap_title("/home/promobot/Pictures/genpact.png");
        label_title = new QLabel( mainWidget );
        label_title->setPixmap(pixmap_title);
        label_title->setScaledContents(true);
        label_title->setGeometry(QRect(490,70,300,100));

        buttonMainVisitor= new QPushButton(mainWidget);
        buttonMainVisitor->setText("VISITOR");
        buttonMainVisitor->setStyleSheet(buttonsStyleSheet);
        buttonMainVisitor->setGeometry(QRect(265,325,300,150));

        buttonMainEmployee=new QPushButton(mainWidget);
        buttonMainEmployee->setText("EMPLOYEE");
        buttonMainEmployee->setStyleSheet(buttonsStyleSheet);
        buttonMainEmployee->setGeometry(QRect(715,325,300,150));

    //end Main Screen



    //GDPR Screen
        textGdpr = new QTextBrowser(widget_gdpr);
        textGdpr->setGeometry(QRect(150,150,980,400));

        label_gdpr = new QLabel(widget_gdpr);
        label_gdpr->setText("GDPR AGREEMENT");
        label_gdpr->setFont(fontFormLabel);
        label_gdpr->setGeometry(QRect(515,50,230,50));

        buttonGdprAgree = new QPushButton(widget_gdpr);
        buttonGdprAgree->setText("AGREE");
        buttonGdprAgree->setStyleSheet(buttonsStyleSheet);
        buttonGdprAgree->setFont(fontSmallButtons);
        buttonGdprAgree->setGeometry(QRect(200,650,300,50));

        buttonGdprDisagree = new QPushButton(widget_gdpr);
        buttonGdprDisagree->setText("DISAGREE");
        buttonGdprDisagree->setStyleSheet(buttonsStyleSheet);
        buttonGdprDisagree->setFont(fontSmallButtons);
        buttonGdprDisagree->setGeometry(QRect(780,650,300,50));

    //end GDPR Screen

    /*buttonBackGdpr = new QPushButton(widget_gdpr);
    buttonBackGdpr->setText("BACK");
    buttonBackGdpr->setFont(fontSmallButtons);
    buttonBackGdpr->setStyleSheet(buttonsStyleSheet);
    buttonBackGdpr->setGeometry(QRect(50,50,150,60));
    buttonBackGdpr->raise(); */

    //Visitor getData screen - after gdpr

        backButtonVisitor = new QPushButton(widget_visitor_data);
        backButtonVisitor->setText("BACK");
        backButtonVisitor->setStyleSheet(buttonsStyleSheet+fontSmallButtons);
        backButtonVisitor->setGeometry(QRect(20,20,150,60));

        form_reg_title_label=new QLabel(widget_visitor_data);
        form_reg_title_label->setText("CONFIRMATION FORM");
        form_reg_title_label->setGeometry(QRect(465,100,300,50));
        form_reg_title_label->setStyleSheet(fontTitleLabel);
        if (db.open())
        {
            qDebug()<<"M-am conectat la baza de date";
        }
        confirm_reg_label = new QLabel(widget_visitor_data);
        confirm_reg_label->setText("Please complete the following fields: ");
        confirm_reg_label->setGeometry(QRect(50,235,100,100));
        confirm_reg_label->setStyleSheet(fontTitleLabel);
        confirm_reg_label->adjustSize();

        first_name_label = new QLabel(widget_visitor_data);
        first_name_label->setText("First name: ");
        first_name_label->setGeometry(QRect(100,350,200,50));
        first_name_label->setStyleSheet(fontTitleLabel);

        first_name_edit = new QTextEdit(widget_visitor_data);
        first_name_edit->setGeometry(QRect(350,350,500,50));
        first_name_edit->setStyleSheet(fontEdit);

        last_name_label = new QLabel(widget_visitor_data);
        last_name_label->setText("Last name: ");
        last_name_label->setGeometry(QRect(100,450,200,50));
        last_name_label->setStyleSheet(fontTitleLabel);

        last_name_edit = new QTextEdit(widget_visitor_data);
        last_name_edit->setGeometry(QRect(350,450,500,50));
        last_name_edit->setStyleSheet(fontEdit);

        code_label = new QLabel(widget_visitor_data);
        code_label->setText("E-mail code: ");
        code_label->setGeometry(QRect(100,550,200,50));
        code_label->setStyleSheet(fontTitleLabel);

        code_edit = new QTextEdit(widget_visitor_data);
        code_edit->setGeometry(QRect(350,550,500,50));
        code_edit->setStyleSheet(fontEdit);
        code_edit->setObjectName("code_edit");
        code_edit->installEventFilter(this);

        buttonConfirmReg = new QPushButton(widget_visitor_data);
        buttonConfirmReg->setText("OK");
        buttonConfirmReg->setStyleSheet(buttonsStyleSheet+fontSmallButtons);
        buttonConfirmReg->setGeometry(QRect(600,650,80,50));



    //end Visitor getData screen


    //Employee screen

    backButtonEmployee = new QPushButton(employeeWidget);
    backButtonEmployee->setText("BACK");
    backButtonEmployee->setStyleSheet(buttonsStyleSheet);
    backButtonEmployee->setGeometry(QRect(50,50,150,60));
    backButtonEmployee->raise();


        //Camera widget

            camera=new Camera(employeeWidget);
            camera->setFixedSize(1280,800);
            camera->setGeometry(QRect(0,0,1280,800));
            camera->stackUnder(backButtonEmployee);

        //end Camera widget

    //end Employee screen

    //SSO screen
            sso_label = new QLabel(widget_sso);
            sso_label->setText("SSO Code: ");
            sso_label->setGeometry(QRect(100,350,200,50));
            sso_label->setStyleSheet(fontTitleLabel);

            sso_edit = new QTextEdit(widget_sso);
            sso_edit->setGeometry(QRect(350,350,500,50));
            sso_edit->setStyleSheet(fontEdit);
            sso_edit->setObjectName("sso_edit");
            sso_edit->installEventFilter(this);
            backButtonSSO = new QPushButton(widget_sso);
            backButtonSSO->setText("BACK");
            backButtonSSO->setStyleSheet(buttonsStyleSheet+fontSmallButtons);
            backButtonSSO->setGeometry(QRect(20,20,150,60));
            buttonSSOConfirm = new QPushButton(widget_sso);
            buttonSSOConfirm->setText("CONFIRM");
            buttonSSOConfirm->setStyleSheet(buttonsStyleSheet+fontSmallButtons);
            buttonSSOConfirm->setGeometry(QRect(600,650,80,50));

            //End SSO screen
    connect(buttonMainVisitor,&QPushButton::clicked,this,&TemplateObject::handleVisitorButton);
    connect(buttonMainEmployee,&QPushButton::clicked,this,&TemplateObject::handleEmployeeButton);
    connect(backButtonEmployee,&QPushButton::clicked,this,&TemplateObject::handleBackButton);
    connect(backButtonSSO,&QPushButton::clicked,this,&TemplateObject::handleBackButton);
    connect(buttonSSOConfirm,&QPushButton::clicked,this,&TemplateObject::handleSSOConfirm);
    connect(backButtonVisitor,&QPushButton::clicked,this,&TemplateObject::handleBackButton);
    connect( camera, &Camera::error, this, &TemplateObject::cameraErrorProcessing );
    connect(buttonGdprAgree,&QPushButton::clicked,this,&TemplateObject::handleAgreeGdpr);
    connect(buttonGdprDisagree,&QPushButton::clicked,this,&TemplateObject::handleDisagreeGdpr);
    connect(buttonConfirmReg,&QPushButton::clicked,this,&TemplateObject::handleConfirmReg);

    //connect(buttonBackGdpr,&QPushButton::clicked,this,&TemplateObject::handleGdprBack);

}
void TemplateObject::handleSSOConfirm()
{
    if (sso_edit->toPlainText()!=QString::number(currentSSO))

    {
        emit signalTTSStart("The SSO you have entered is wrong!Please try again!");
        handleBackButton();
    }
    else
       {
        QString textToSpeak="Welcome back" + currentName + " " + currentLastName + ". Your current email adress is: " + currentEmail+ ".Thank your for coming to Genpact today!";
        emit signalTTSStart(textToSpeak);
          handleBackButton();
    }
}
void TemplateObject::handleVisitorButton()
{


   pages->setCurrentWidget(widget_gdpr);
   QFile gdpr("/home/promobot/Documents/gdpr.txt");

   if(!gdpr.open(QIODevice::ReadOnly))
       return;

   QTextStream in(&gdpr);
   textGdpr->setText(in.readAll());


}
void TemplateObject::handleEmployeeButton()
{

  pages->setCurrentWidget(employeeWidget);
  QString mesaj_eroare;
  passed=1;
  if( camera->open() ) camera->start();
    else {
           camera->error(mesaj_eroare);
           qDebug()<<"Eroare la camera"+mesaj_eroare ;
           }

}

void TemplateObject::handleBackButton()
{


   pages->setCurrentWidget(mainWidget);
   passed=0;
   contor=0;
   if(camera->isOpen())camera->stop();


    if(first_name_edit->toPlainText()!="")
        first_name_edit->clear();

    if(last_name_edit->toPlainText()!="")
        last_name_edit->clear();

    if(code_edit->toPlainText()!="")
        code_edit->clear();
    if(sso_edit->toPlainText()!="")
        sso_edit->clear();

}

void TemplateObject::handleAgreeGdpr(){

          qInfo()<<"GDPR Acceptat";
          pages->setCurrentWidget(widget_visitor_data);

}

void TemplateObject::handleDisagreeGdpr(){

  qInfo()<<"GDPR Refuzat";
  pages->setCurrentWidget(mainWidget);

}

void TemplateObject::handleConfirmReg(){



    emit signalTTSStart("Your name is "+first_name_edit->toPlainText()+" "+last_name_edit->toPlainText()+" and the code you inserted is "+code_edit->toPlainText());
    last_name_edit->clear();
    first_name_edit->clear();
    code_edit->clear();

}

void TemplateObject::cameraErrorProcessing( const QString &error_text )
{
    qDebug()<<"Eroare la camera ";
}

bool TemplateObject::eventFilter(QObject *watched, QEvent *event){

    if(event->type()==QEvent::MouseButtonPress && (watched->objectName()=="widget_visitor_data" || watched->objectName()=="widget_sso"))
    {
        last_name_edit->clearFocus();
        first_name_edit->clearFocus();
        code_edit->clearFocus();
        sso_edit->clearFocus();

    }

    if(event->type()==QEvent::FocusIn && watched->objectName()=="code_edit")
    {
        int x,y,w,h;
        code_edit->geometry().getRect(&x,&y,&w,&h);
        code_edit->setGeometry(QRect(x,y-280,w,h));
        //code_edit->setFocus(Qt::FocusReason::);
    }

    if(event->type()==QEvent::FocusOut && watched->objectName()=="code_edit")
    {
        int x,y,w,h;
        code_edit->geometry().getRect(&x,&y,&w,&h);
        code_edit->setGeometry(QRect(x,y+280,w,h));
    }

    return false;
}

void TemplateObject::start()
{
    PluginInterface::start();
    initDialog();

}
void TemplateObject::callbackFaceRecognizeStatus(const FaceRecognizeStatus &status)
{
    if (status!=FaceRecognizeStatus::START || status!=FaceRecognizeStatus::FINISH)
    {
        emit signalFaceRecognize();
    }
}
void TemplateObject::callbackFaceInfo(const QVector<FaceInfoPtr> &list)
{
    if (list.isEmpty()) return;

    /*for (int i=0;i<=list.size();i++)
        {
            qint64 id = list[0].data()->id();
            Gender gender=list[0].data()->gender();
            double age= list[0].data()->age();
          if(gender==Gender::MALE)
              label_in_employee->setText("Barbat cu varsta de: " + QString::number(age));
          else if (gender==Gender::FEMALE)
              label_in_employee->setText("Femeie cu varsta de: "+ QString::number(age));
          else label_in_employee->setText("Necunoscut");

          if(list[0].data()->type()== FaceInfo::Type::GENERAL || list[0].data()->type()== FaceInfo::Type::TEMP)
           {
              UserData myUser;
              PluginInterface::databaseUsers()->getUserData(id,myUser);

              QString nume = myUser.name();
              label_with_name->setText("Numele este: " + nume);
           }

        }*/


        QString users="";
        for (int i=0;i<list.size();i++)
        {
            qint64 id = list[i].data()->id();
            if(list[i].data()->type()== FaceInfo::Type::GENERAL || list[i].data()->type()== FaceInfo::Type::TEMP)
             {
                UserData myUser;
                PluginInterface::databaseUsers()->getUserData(id,myUser);

                QString nume = myUser.name();
                if (nume!="")
                {
                    QSqlQuery query ;
                    QString queryString = "SELECT name , last_name, email, sso from ivisitor WHERE name = '" + nume +"'";
                    query.exec(queryString);
                    while (query.next())
                    {
                        QString name = query.value(0).toString();
                        QString last_name = query.value(1).toString();
                        QString email = query.value(2).toString();
                        qint64 sso = query.value(3).toInt();
                        currentName=name;
                        currentLastName=last_name;
                        currentEmail=email;
                        currentSSO=sso;
                    }
                users=nume;
                }
             }
           // if ((i+1)!=list.size() && users!=""  )
             //   users=users+" and ";
        }

        if (contor==0 && users!="" && passed==1)
        {
            emit signalTTSStart("How are you doing in this fine day " + users);
            emit signalTTSStart("Please enter your SSO to confirm your identity");
            pages->setCurrentWidget(widget_sso);
            contor=1;
            passed=0;
        }
        else if (contor==0 && users=="" && passed==1)
        {
            emit signalTTSStart("I can not identify you. Please enter your SSO code so that I know who you are!");
            pages->setCurrentWidget(widget_sso);
            contor=1;
            passed=0;

        }



    dialog->setFaceInfo( list );



}
void TemplateObject::connectDialogWithOP()
{
    connect( dialog, &PromobotDialogModule::signalStartApp, this, &TemplateObject::slotStartApp );
    connect( dialog, &PromobotDialogModule::signalDriveMode, this, &TemplateObject::signalDriveMode );
    connect( dialog, &PromobotDialogModule::signalDrivePause, this, &TemplateObject::signalDrivePause );
    connect( dialog, &PromobotDialogModule::signalDrivePoint, this, &TemplateObject::signalDrivePoint );
    connect( dialog, &PromobotDialogModule::signalTTSStart, this, &TemplateObject::signalTTSStart );
    connect( dialog, &PromobotDialogModule::signalTTSCancel, this, &TemplateObject::signalTTSCancel );
    connect( dialog, &PromobotDialogModule::signalASRStart, this, &TemplateObject::signalASRStart );
    connect( dialog, &PromobotDialogModule::signalLanguage, this, &TemplateObject::signalLanguage );
    connect( dialog, &PromobotDialogModule::signalUsername, this, &TemplateObject::signalUsername );
    connect( dialog, &PromobotDialogModule::signalUsernames, this, &TemplateObject::signalUsernames );
    connect( dialog, &PromobotDialogModule::signalAnswersQuery, this, &TemplateObject::slotAnswersQuery );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const ID & )>( &PromobotDialogModule::signalAnswersAnchor ),
             this, static_cast<void ( TemplateObject::* )( const ID & )>( &TemplateObject::signalAnswersAnchor ) );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const QString & )>( &PromobotDialogModule::signalAnswersAnchor ),
             this, static_cast<void ( TemplateObject::* )( const QString & )>( &TemplateObject::signalAnswersAnchor ) );
    connect( dialog, &PromobotDialogModule::signalAnswersReset, this, &TemplateObject::signalAnswersReset );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const ID & )>( &PromobotDialogModule::signalAnswersCase ),
             this, static_cast<void ( TemplateObject::* )( const ID & )>( &TemplateObject::signalAnswersCase ) );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const QString & )>( &PromobotDialogModule::signalAnswersCase ),
             this, static_cast<void ( TemplateObject::* )( const QString & )>( &TemplateObject::signalAnswersCase ) );
    connect( dialog, &PromobotDialogModule::signalAnswersCaseType, this, &TemplateObject::signalAnswersCaseType );
    connect( dialog, &PromobotDialogModule::signalAnswersCaseReset, this, &TemplateObject::signalAnswersCaseReset );
    connect( dialog, &PromobotDialogModule::signalGetReplica, this, &TemplateObject::signalGetReplica );
    connect( dialog, &PromobotDialogModule::signalGetAppReplica, this, &TemplateObject::signalGetAppReplica );
    connect( dialog, &PromobotDialogModule::signalAnswersCompare, this, &TemplateObject::signalAnswersCompare );
    connect( dialog, &PromobotDialogModule::signalEyes, this, static_cast<void ( TemplateObject::* )( const Animation & )>( &TemplateObject::signalAnimation ) );
    connect( dialog, &PromobotDialogModule::signalScriptStart, this, &TemplateObject::signalScriptStart );
    connect( dialog, &PromobotDialogModule::signalScriptCancel, this, &TemplateObject::signalScriptCancel );
    connect( dialog, &PromobotDialogModule::updateInteractions, this, &TemplateObject::updateInteractions );
    connect( dialog, &PromobotDialogModule::finishInteractions, this, &TemplateObject::finishInteractions );

    connect( dialog, &PromobotDialogModule::signalHead, this, &TemplateObject::signalHead );
    connect( dialog, &PromobotDialogModule::signalLed, this, &TemplateObject::signalLed );
    connect( dialog, &PromobotDialogModule::signalAppParameter, this, &TemplateObject::appParameter );
    connect( dialog, &PromobotDialogModule::signalSysParameter, this, &TemplateObject::sysParameter );
    connect( dialog, &PromobotDialogModule::signalFaceTracking, this, &TemplateObject::signalFaceTracking );
    connect( dialog, &PromobotDialogModule::signalFaceTrackingAuto, this, &TemplateObject::signalFaceTrackingAuto );
}
void TemplateObject::initDialog()
{
    // Sets the standard state of the RGB backlight
    dialog->setDefaultLed( this->sysParameter( "/dled/default_red", 0 ).toInt(), this->sysParameter( "/led/default_green", 0 ).toInt(), this->sysParameter( "/led/default_blue", 255 ).toInt() );
    // Launches the dialog module
    dialog->start();
    // Sets the flag for using the dating procedure
    dialog->setAcquaintance( false );
    // Sets the loyalty program usage flag
    dialog->setLoyaltyProgram( false );
    // Sets the interrupt pronunciation flag by speech recognition signal
    dialog->setInterruptTTSbyASR( this->sysParameter( "/dialog/interruptTTSbyASR", false ).toBool() );
    use_interrupt_tts = this->sysParameter( "/dialog/interruptTTSbyVAD", true ).toBool();
    // Sets a time limit for promo calls.
    dialog->setTimeoutPromo( this->sysParameter( "/dialog/timeoutPromo", 30 ).toInt() );
    // Sets the navigation usage flag
    dialog->setUseNavigation( this->sysParameter( "/driving/useMap", false ).toBool() );
    // Sets the main point id
    dialog->setIdMainPoint( this->sysParameter( "/navigation/idMainPoint", 1 ).toInt() );
    // Sets the flag to return to the main point.
    dialog->setBackToMainPoint( this->sysParameter( "/navigation/backMainPoint", true ).toBool() );
    // Sets the flag to ignore interactions
    dialog->setIgnoreInteractions( this->sysParameter( "/navigation/ignoreInteractions", true ).toBool() );
}
void TemplateObject::callbackLanguage( const QString &lang )
{
    //Sets the current language
    dialog->setLanguage( lang );
}

void TemplateObject::callbackInteraction( const bool &value )
{
    // Sets the interaction status
    dialog->setInteraction( value );
}

bool TemplateObject::slotStartApp( const QString &name, const QStringList &args )
{
    // Runs the application
    bool state          = false;
    IApplicationPtr app = application( name );
    if( app ) state = app->start( args );
    return state;
}

void TemplateObject::slotAnswersQuery( const Question &q )
{
    // Sends a request to the question-answer system
    emit signalAnswersQuery( q );
}

void TemplateObject::callbackNetworkPing( const double &value )
{
    // Sets the network connection delay
    dialog->setNetworkPing( value );
}

void TemplateObject::callbackBatteryStatus( const BatteryState &value )
{
    // Sets low battery status
    dialog->setBatteryLow( value );
}

void TemplateObject::callbackDriveMode( const DriveMode &value )
{
    // Sets the mode of movement of the robot
    dialog->setDriveMode( value );
}

void TemplateObject::callbackDriveStatus( const MovementStatus &value )
{
    // Sets the movement status of the robot.
    dialog->setDriveStatus( value );
}

void TemplateObject::callbackFindFrontObstacle()
{
    // Sets the detection status of an obstacle in front of the robot.
    dialog->setFindFrontObstacle();
}

void TemplateObject::callbackPeoplePresenceChanged( const bool &p )
{
    // Sets the human presence status in front of the robot.
    dialog->setPeoplePresence( false );
}

void TemplateObject::callbackTTSProcess( const bool &value, const QUuid &uuid )
{
    // Sets the status of the speech synthesis process.
    dialog->setTTSProcess( value, uuid );
}

void TemplateObject::callbackASRVariant( const QUuid &uuid, const QString &text )
{
    // Sets the option obtained from speech recognition
    dialog->setVariant( uuid, text, 0, 0.0 );
}

void TemplateObject::callbackASRResult( const QUuid &uuid, const QString &text )
{
    // Accepts Speech Recognition

    dialog->slotASRResult( uuid, text );
}

void TemplateObject::callbackASRProcess( const bool &state )
{
    // Sets the status of the speech recognition process.
    dialog->setASRProcess( state );
}

void TemplateObject::callbackASRSpeech( const bool &value )
{
    if( use_interrupt_tts && dialog->isSpeaking() && value )
    {
        qDebug() << "!!! ExampleDialogObject::callbackASRSpeech -> emit signalTTSCancel()";
        // Interrupts speech
        dialog->stopSpeaking( "" );
    }
}

void TemplateObject::callbackAnswerResult( const AnswerPtr &value )
{
    if( value.isNull() ) return;
    if( value->replica().isNull() ) return;
    // Sets the final answer to a user phrase.
    dialog->setAnswerResult( value );
}

void TemplateObject::callbackFaceGeneral( const qint64 &id )
{
    general_id_prev = general_id;
    general_id      = id;

    qDebug() << "New general id:" << general_id;
    qDebug() << "Prev general id:" << general_id_prev;
    // Sets the main user
    dialog->setFaceGeneral( general_id );
}



void TemplateObject::callbackScriptEnd()
{
    // Sets the end of script flag
    dialog->setScriptEnd();
}

RetData TemplateObject::stop()
{
    dialog->stop();
    return PluginInterface::stop();
}

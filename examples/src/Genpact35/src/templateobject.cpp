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

    if (db.open())
    {
        qDebug()<<"M-am conectat la baza de date";
    }
    ///############# Pages #######################

        welcomePage = new QWidget;
        welcomePage->setStyleSheet(welcomeStyle);
        welcomePage->setObjectName("welcomePage");
        welcomePage->installEventFilter(this);



        optionPage = new QWidget;
        optionPage->setStyleSheet(optionStyle);



        faceRecognitionPage = new QWidget;
        faceRecognitionPage->setObjectName("faceRecognitionPage");
        faceRecognitionPage->installEventFilter(this);


        twoFAPage = new QWidget;
        twoFAPage->setObjectName("twoFAPage");
        twoFAPage->setStyleSheet(authStyle);
        twoFAPage->installEventFilter(this);


        lastPage = new QWidget;
        lastPage->setStyleSheet(lastStyle);


      qDebug() << "Am initializat pages";

    ///########## Stack of pages ########


            pages = new QStackedWidget;
            pages->addWidget(welcomePage);
            pages->addWidget(optionPage);
            pages->addWidget(faceRecognitionPage);
            pages->addWidget(twoFAPage);
            pages->addWidget(lastPage);
            setCentralWidget(pages);

     qDebug() << "Am initializat stack of pages";

    ///########## Welcome page #########

         welcomeButton= new QPushButton(welcomePage);
         welcomeButton->setGeometry(QRect(0,0,1280,800));
         welcomeButton->setStyleSheet(opac);

       qDebug() << "Am initializat welcome page";

     ///######## Option Page ###############

         //Title
           optionTitleLabel = new QLabel(optionPage);
           optionTitleLabel->setText("Happy to see you!");
           optionTitleLabel->setStyleSheet(textStyleSheet75);

           updateLabel(optionTitleLabel);
           labelWidth = computeWidth(optionTitleLabel);
           labelHeight = computeHeigth(optionTitleLabel);
           optionTitleLabel->setGeometry(QRect((1280-labelWidth)/2,120,labelWidth,labelHeight));

           qDebug() << "Option-page title label done";

        //Buttons

           visitorButton = new QPushButton(optionPage);
           visitorButton->setText("Client");
           visitorButton->setStyleSheet(buttonStyleSheet);
           visitorButton->setGeometry(QRect(315,560,250,50));

           employeeButton = new QPushButton(optionPage);
           employeeButton->setText("Employee");
           employeeButton ->setStyleSheet(buttonStyleSheet);
           employeeButton->setGeometry(QRect(715,560,250,50));

           qDebug() << "Option-page buttons done";

        //Label instructiuni

           optionInstructionLabel = new QLabel(optionPage);
           optionInstructionLabel->setText("Select one of the above to continue");
           optionInstructionLabel->setStyleSheet(textStyleSheet20);

           updateLabel(optionInstructionLabel);
           labelWidth = computeWidth(optionInstructionLabel);
           labelHeight = computeHeigth(optionInstructionLabel);
           optionInstructionLabel->setGeometry(QRect((1280-labelWidth)/2,700,labelWidth,labelHeight));

         qDebug() << "Option-page instr-label done";

     ///######### Face Rec Page ###########

         faceBackLabel = new QLabel(faceRecognitionPage);
         faceBackLabel->setGeometry(QRect(0,0,1280,800));
         faceBackLabel->setStyleSheet("background-image:url(/home/promobot/Pictures/cutout.png);");

          qDebug() << "Face Rec Page";

         camera= new Camera(faceRecognitionPage);
         camera->setGeometry(QRect(0,200,1280,800));

         qDebug() << "Camera create si geometry";

     ///######### Two FA Page #############

       fullNameLabel = new QLabel(twoFAPage);
       fullNameLabel->setText("Please enter your name");
       fullNameLabel->setStyleSheet(textStyleSheet20);

       updateLabel(fullNameLabel);
       labelWidth = computeWidth(fullNameLabel);
       labelHeight = computeHeigth(fullNameLabel);
       fullNameLabel->setGeometry(QRect(120,250,labelWidth,50));

        qDebug() << "2FA-page fullNameLabel done";

       fullNameEdit = new QLineEdit(twoFAPage);
       fullNameEdit->setStyleSheet(editStyleSheet);
       fullNameEdit->setAlignment(Qt::AlignCenter);
       fullNameEdit->setGeometry(QRect(100,300,350,50));

       qDebug() << "2FA-page fullNameEdit done";

       ssoLabel = new QLabel(twoFAPage);
       ssoLabel->setObjectName("ssoLabel");
       ssoLabel->setText("Please enter your SSO");
       ssoLabel->setStyleSheet(textStyleSheet20);

       qDebug() << "2FA-page ssoLabel done";

       updateLabel(ssoLabel);
       labelWidth = computeWidth(ssoLabel);
       labelHeight = computeHeigth(ssoLabel);
       ssoLabel->setGeometry(QRect(120,450,labelWidth,50));

       ssoEdit = new QLineEdit(twoFAPage);
       ssoEdit->setStyleSheet(editStyleSheet);
       ssoEdit->setObjectName("ssoEdit");
       ssoEdit->setAlignment(Qt::AlignCenter);
       ssoEdit->setGeometry(QRect(100,500,350,50));
       ssoEdit->installEventFilter(this);

       qDebug() << "2FA-page ssoEdit done";


     ///####### Last Page ##############

     //nothing to add yet


     ///####### Testing #####


    //optionButton = new QPushButton(optionPage);
    //faceButton = new QPushButton(faceRecognitionPage);
    //twoFAButton= new QPushButton(twoFAPage);

    qDebug() << "Creat butoane invizibile";

    //optionButton->setGeometry(QRect(0,0,1280,800));
    //faceButton->setGeometry(QRect(0,0,1280,800));
    //twoFAButton->setGeometry(QRect(0,0,1280,800));

    qDebug() << "Geometrie butoane invizibile";



    //optionButton->setStyleSheet(opac);
    //faceButton->setStyleSheet(opac);
    //twoFAButton->setStyleSheet(opac);

    //faceButton->raise();
    //camera->stackUnder(faceButton);
    camera->stackUnder(faceBackLabel);
    //faceBackLabel->stackUnder(faceButton);



    qDebug() << "Style butoane invizibile";



    connect(welcomeButton,&QPushButton::clicked,this,&TemplateObject::switchToOption);
    connect(visitorButton,&QPushButton::clicked,this,&TemplateObject::handleClient);
    connect(employeeButton,&QPushButton::clicked,this,&TemplateObject::handleEmployee);
    //connect(faceButton,&QPushButton::clicked,this,&TemplateObject::switchTo2FA);
    //connect(twoFAButton,&QPushButton::clicked,this,&TemplateObject::switchToWelcome);
    connect( camera, &Camera::error, this, &TemplateObject::cameraErrorProcessing );

     qDebug() << "Connecturi";
}

void TemplateObject::printLabel(QString nume, QString prenume, QString functie1, QString functie2, QString valabilitate, QString pathPoza)
{

  QPainter paint;
  QPrinter printer(QPrinter::PrinterResolution);


  printer.setPageSizeMM(QSizeF(89,36));


  //QImage image("/home/costin/Pictures/Etichete/eticheta1.jpg");

  paint.begin(&printer);

  paint.setFont(QFont("Georgia",10,QFont::Bold));
  paint.drawText(10,10,nume);
  paint.drawText(10,20,prenume);
  paint.setFont(QFont("Georgia",8));
  paint.drawText(10,35,functie1);
  paint.drawText(10,45,functie2);
  paint.setFont(QFont("Georgia",6));
  paint.drawText(10,60,"Valid From:"+valabilitate);
  paint.drawText(30,70,"To:"+valabilitate);

  paint.drawImage(100,30,QImage(pathPoza));

  //paint.drawImage(250,30,scaledFace);
  //paint.drawImage(0,0,image);

  paint.end();

}

bool TemplateObject::eventFilter(QObject *watched, QEvent *event){

    if(event->type()==QEvent::MouseButtonPress &&  watched->objectName()=="twoFAPage")
    {

        fullNameEdit->clearFocus();
        ssoEdit->clearFocus();

    }

    if(event->type()==QEvent::FocusIn && watched->objectName()=="ssoEdit")
    {
        int x,y,w,h;


        fullNameLabel->geometry().getRect(&x,&y,&w,&h);
        fullNameLabel->setGeometry(QRect(x,y-100,w,h));

        fullNameEdit->geometry().getRect(&x,&y,&w,&h);
        fullNameEdit->setGeometry(QRect(x,y-100,w,h));

        ssoLabel->geometry().getRect(&x,&y,&w,&h);
        ssoLabel->setGeometry(QRect(x,y-100,w,h));

        ssoEdit->geometry().getRect(&x,&y,&w,&h);
        ssoEdit->setGeometry(QRect(x,y-100,w,h));

        //ssoEdit->setFocus();
        //ssoEdit->setFocus();
        PluginInterface::keyboard()->show();


    }

    if(event->type()==QEvent::FocusOut && watched->objectName()=="ssoEdit")
    {
        int x,y,w,h;

        fullNameLabel->geometry().getRect(&x,&y,&w,&h);
        fullNameLabel->setGeometry(QRect(x,y+100,w,h));

        fullNameEdit->geometry().getRect(&x,&y,&w,&h);
        fullNameEdit->setGeometry(QRect(x,y+100,w,h));

        ssoLabel->geometry().getRect(&x,&y,&w,&h);
        ssoLabel->setGeometry(QRect(x,y+100,w,h));

        ssoEdit->geometry().getRect(&x,&y,&w,&h);
        ssoEdit->setGeometry(QRect(x,y+100,w,h));

        if(checkEdits())
        {
            validateEdits();
        }
    }


    ///#### Exemplu de cod - cand if = true inseamna ca pagina activa e faceRecognitionPage

    if(event->type()==QEvent::ShowToParent &&  watched->objectName()=="faceRecognitionPage")
    {
        inFaceRec = true;
    }
    else
      inFaceRec = false;

    return false;
}

void TemplateObject::validateEdits()
{
    QString debugString=fullNameEdit->text() + "     " + currentName + "      " + ssoEdit->text() + "        " + currentSSO;
    qDebug()<<debugString;
    if(fullNameEdit->text().toLower()==currentName.toLower() && ssoEdit->text().toLower()==currentSSO.toLower() )
    {
        pages->setCurrentWidget(lastPage);
        emit signalTTSStart("I hope you will have a great day " + currentName + ". Please don't forget your barcode!");
        emit signalSavePhoto("/home/Promobot/Pictures");
        printLabel(currentLastName,currentName,currentFunction1,currentFunction2,"19.04.2019",barcodePath);
        currentName="";
        QTimer::singleShot(4000,this,SLOT(switchToStart()));
    }
    else {
        emit signalTTSStart("The data you have entered is wrong, please enter your data again.");
        pages->setCurrentWidget(welcomePage);
        contor=0;
        passed=0;
        interactiv=0;
    }
    /// ### printare aici ###
    fullNameEdit->clear();
    ssoEdit->clear();
}

bool TemplateObject::checkEdits()
{
    if(!fullNameEdit->text().isEmpty() && !ssoEdit->text().isEmpty())
    {

        return 1;
    }
    return 0;
}

void TemplateObject::switchToOption()
{
    interactiv=1;
    pages->setCurrentWidget(optionPage);
    emit signalTTSStart("Happy to see you, please select your position for further registration.");
}

void TemplateObject::handleClient()
{
    qDebug()<<"Clicked Client";

    pages->setCurrentWidget(faceRecognitionPage);
    emit signalTTSStart("Now please look into my eyes for a better friendship.");
    passed=1;

    qDebug()<<"In handleClient passed = " + passed;

    if( camera->open() ) camera->start();
      else {

             qDebug()<<"Eroare la camera";
             }
}

void TemplateObject::handleEmployee()
{
    qDebug()<<"Clicked Employee";
    passed=1;
    pages->setCurrentWidget(faceRecognitionPage);
    emit signalTTSStart("Now please look into my eyes for a better friendship.");


    qDebug()<<"In handleEmployee passed = " + passed;

    if( camera->open() ) camera->start();
      else {

             qDebug()<<"Eroare la camera";
             }
}

void TemplateObject::switchTo2FA()
{
    emit signalTTSStart("I need to know your name and code! Thank you!");
    pages->setCurrentWidget(twoFAPage);
}
void TemplateObject::switchToStart()
{
    contor=0;
    passed=0;
    interactiv=0;
    pages->setCurrentWidget(welcomePage);
}
void TemplateObject::switchToWelcome()
{
    contor=0;
    passed=0;
    pages->setCurrentWidget(welcomePage);
}
void TemplateObject::cameraErrorProcessing( const QString &error_text )
{
    qDebug()<<"Eroare la camera ";
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
    if(list.isEmpty()) return;
    QString users="";
    qint64 id = list[0].data()->id();
        if(list[0].data()->type()== FaceInfo::Type::GENERAL || list[0].data()->type()== FaceInfo::Type::TEMP)
         {
            UserData myUser;
            PluginInterface::databaseUsers()->getUserData(id,myUser);

            QString nume = myUser.name();
            if (nume!="")
            {
                QSqlQuery query ;
                QString queryString = "SELECT name , last_name, email, sso, functie1, functie2 from ivisitor WHERE name = '" + nume +"'";
                query.exec(queryString);
                while (query.next())
                {
                    QString name = query.value(0).toString();
                    QString last_name = query.value(1).toString();
                    QString email = query.value(2).toString();
                    QString sso = query.value(3).toString();
                    QString function1=query.value(4).toString();
                    QString function2=query.value(5).toString();
                    currentName=name;
                    currentLastName=last_name;
                    currentEmail=email;
                    currentSSO=sso;
                    currentFunction1=function1;
                    currentFunction2=function2;
                }
            users=nume;
            }
         }


        if(inFaceRec == true)
        {
           qDebug()<<"Userii sunt : " + users + " si contor = " + contor + " si passed= "+ passed;
        }

        if (contor==0 && users!="" && passed==1)
        {


            emit signalTTSStart("How are you doing in this fine day " + users);
            emit signalTTSStart("Please enter your data to confirm your identity");
            QTimer::singleShot(3000,this,SLOT(changeToSSO()));
            contor=1;
            passed=0;
        }
        else if (contor==0 && users=="" && passed==1)
        {
            emit signalTTSStart("I can not identify you. Please enter your name and your SSO code so that I know who you are!");
            QTimer::singleShot(3000,this,SLOT(changeToSSO()));
            contor=1;
            passed=0;

        }
    dialog->setFaceInfo(list);
}
void TemplateObject::changeToSSO(){
    pages->setCurrentWidget(twoFAPage);
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

void TemplateObject::start()
{
    PluginInterface::start();
    initDialog();

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
    if (!interactiv)
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
    //if (p && currentName=="") emit signalTTSStart("Hello , I am the new Concierge of the future, don't be afraid of the robot!");
    dialog->setPeoplePresence( false );
}

void TemplateObject::callbackTTSProcess( const bool &value, const QUuid &uuid )
{
    // Sets the status of the speech synthesis process.
    if (!interactiv)
    dialog->setTTSProcess( value, uuid );
}

void TemplateObject::callbackASRVariant( const QUuid &uuid, const QString &text )
{
    // Sets the option obtained from speech recognition
    if (!interactiv)
    dialog->setVariant( uuid, text, 0, 0.0 );
}

void TemplateObject::callbackASRResult( const QUuid &uuid, const QString &text )
{
    // Accepts Speech Recognition
    if (!interactiv)
    dialog->slotASRResult( uuid, text );
}

void TemplateObject::callbackASRProcess( const bool &state )
{
    // Sets the status of the speech recognition process.
    if (!interactiv)
    dialog->setASRProcess( state );
}

void TemplateObject::callbackASRSpeech( const bool &value )
{
    if( use_interrupt_tts && dialog->isSpeaking() && value  )
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
    if (!interactiv)
        dialog->setAnswerResult( value );
    else return;
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

void TemplateObject::updateLabel(QLabel *label)
{
   label->style()->unpolish(label);
   label->style()->polish(label);
   label->update();
}

int TemplateObject::computeWidth(QLabel *label)
{
    int width = label->fontMetrics().width(label->text());

    return width;
}

int TemplateObject::computeHeigth(QLabel *label)
{
    int height = label->fontMetrics().height();

    return height;
}




RetData TemplateObject::stop()
{
    dialog->stop();
    return PluginInterface::stop();
}

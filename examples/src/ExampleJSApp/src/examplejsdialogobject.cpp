#include "include/examplejsdialogobject.h"

#include <libqtpromobot/databasedefaults.h>

ExampleJSDialogObject::ExampleJSDialogObject():
    use_interrupt_tts( false ),
    use_additional( false ),
    use_approach( true ),
    count_answer( 0 ),
    frequency_additional( 3 )
{
    Q_INIT_RESOURCE( resources );
}

ExampleJSDialogObject::~ExampleJSDialogObject()
{
    stop();

    delete dialog;
    delete wgt;
    delete view_js_api;
}

void ExampleJSDialogObject::createWidget( QWidget *parent )
{
    wgt = new PromobotMainScreen( parent );
    wgt->setVisible( false );

    view_js_api = new lightview::LightView( parent );
    this->setCentralWidget( view_js_api );

    // Создаем объект диалога
    dialog = new PromobotDialogModule( parent );
    // Устанавливает сервис синтеза речи
    dialog->setTTSService( this->TTSService() );
    // Задаёт базу данных пользователей
    dialog->setUsersDatabase( this->databaseUsers() );
    // Задаёт сервис обработчика макросов
    dialog->setMacrosReplacer( this->macrosReplacer() );

    // Соединяем сигналы диалога со слотами плагина
    connectDialogWithOP();
}

void ExampleJSDialogObject::start()
{
    PluginInterface::start();
    current_volume  = promobot::getSoundVolume();
    current_mic     = promobot::getMicVolume();
    is_close        = false;
    // Инициализируем диалог
    initDialog();

    wgt->setAutohideEnabled( this->sysParameter( "/dialog/useAutoHidingPromo", true ).toBool() );
    wgt->setAutohideTimeout( this->sysParameter( "/dialog/timeoutAutoHidingPromo", 15.0 ).toDouble() );
    wgt->start();
    use_additional          = this->sysParameter( "/dialog/useAdditional", true ).toBool();
    use_approach            = this->sysParameter( "/dialog/useApproach", true ).toBool();
    frequency_additional    = this->sysParameter( "/dialog/frequencyAdditional", 3 ).toInt();


    view_js_api->setStatisticPeriodicity( this->sysParameter( "/js/statisticPeriodicity", "day" ).toString() );

    if( lastApplication() )
    {
        qDebug() << "lastApplication()->info()->systemName()" << lastApplication()->info()->systemName() << "lastApplication()->returnData()" << lastApplication()->returnData();
        view_js_api->applicationFinish( lastApplication()->info()->systemName(), lastApplication()->finishStatus(), lastApplication()->returnData() );
    }
    else
    {
        view_js_api->load( appParameter( "/basic/url", "http://localhost/example/index.html" ).toString(), "", false );
        view_js_api->defaultHandlers( this, dialog );
        view_js_api->registerEventHandler( lightview::constants::CUSTOM_EVENT_JS, std::bind( &ExampleJSDialogObject::customEventHandler, this, std::placeholders::_1 ) );
    }
}

RetData ExampleJSDialogObject::stop()
{
    dialog->stop();
    wgt->stop();
    view_js_api->pageHidden( is_close );
    if( !current_lang.isEmpty() )
    {
        emit signalLanguage( current_lang );
        current_lang.clear();
    }
    if( current_volume > 0 )
    {
        promobot::setSoundVolume( current_volume );
        current_volume = -1;
    }
    if( current_mic > 0 )
    {
        promobot::setMicVolume( current_mic );
        current_mic = -1;
    }
    qDebug() << "Promobot Dialog stopped";
    qDebug() << "Promobot MainScreen stopped";
    qDebug() << "Plugin stopped";

    return PluginInterface::stop();
}

void ExampleJSDialogObject::connectDialogWithOP()
{
    connect( dialog, &PromobotDialogModule::signalStartApp, this, &ExampleJSDialogObject::slotStartApp );
    connect( dialog, &PromobotDialogModule::signalDriveMode, this, &ExampleJSDialogObject::signalDriveMode );
    connect( dialog, &PromobotDialogModule::signalDrivePause, this, &ExampleJSDialogObject::signalDrivePause );
    connect( dialog, &PromobotDialogModule::signalDrivePoint, this, &ExampleJSDialogObject::signalDrivePoint );
    connect( dialog, &PromobotDialogModule::signalTTSStart, this, &ExampleJSDialogObject::signalTTSStart );
    connect( dialog, &PromobotDialogModule::signalTTSCancel, this, &ExampleJSDialogObject::signalTTSCancel );
    connect( dialog, &PromobotDialogModule::signalASRStart, this, &ExampleJSDialogObject::signalASRStart );
    connect( dialog, &PromobotDialogModule::signalLanguage, this, &ExampleJSDialogObject::signalLanguage );
    connect( dialog, &PromobotDialogModule::signalUsername, this, &ExampleJSDialogObject::signalUsername );
    connect( dialog, &PromobotDialogModule::signalUsernames, this, &ExampleJSDialogObject::signalUsernames );
    connect( dialog, &PromobotDialogModule::signalAnswersQuery, this, &ExampleJSDialogObject::slotAnswersQuery );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const ID & )>( &PromobotDialogModule::signalAnswersAnchor ),
             this, static_cast<void ( ExampleJSDialogObject::* )( const ID & )>( &ExampleJSDialogObject::signalAnswersAnchor ) );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const QString & )>( &PromobotDialogModule::signalAnswersAnchor ),
             this, static_cast<void ( ExampleJSDialogObject::* )( const QString & )>( &ExampleJSDialogObject::signalAnswersAnchor ) );
    connect( dialog, &PromobotDialogModule::signalAnswersReset, this, &ExampleJSDialogObject::signalAnswersReset );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const ID & )>( &PromobotDialogModule::signalAnswersCase ),
             this, static_cast<void ( ExampleJSDialogObject::* )( const ID & )>( &ExampleJSDialogObject::signalAnswersCase ) );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const QString & )>( &PromobotDialogModule::signalAnswersCase ),
             this, static_cast<void ( ExampleJSDialogObject::* )( const QString & )>( &ExampleJSDialogObject::signalAnswersCase ) );
    connect( dialog, &PromobotDialogModule::signalAnswersCaseType, this, &ExampleJSDialogObject::signalAnswersCaseType );
    connect( dialog, &PromobotDialogModule::signalAnswersCaseReset, this, &ExampleJSDialogObject::signalAnswersCaseReset );
    connect( dialog, &PromobotDialogModule::signalGetReplica, this, &ExampleJSDialogObject::signalGetReplica );
    connect( dialog, &PromobotDialogModule::signalGetAppReplica, this, &ExampleJSDialogObject::signalGetAppReplica );
    connect( dialog, &PromobotDialogModule::signalAnswersCompare, this, &ExampleJSDialogObject::signalAnswersCompare );
    connect( dialog, &PromobotDialogModule::signalEyes, this, static_cast<void ( ExampleJSDialogObject::* )( const Animation & )>( &ExampleJSDialogObject::signalAnimation ) );
    connect( dialog, &PromobotDialogModule::signalScriptStart, this, &ExampleJSDialogObject::signalScriptStart );
    connect( dialog, &PromobotDialogModule::signalScriptCancel, this, &ExampleJSDialogObject::signalScriptCancel );
    connect( dialog, &PromobotDialogModule::updateInteractions, this, &ExampleJSDialogObject::updateInteractions );
    connect( dialog, &PromobotDialogModule::finishInteractions, this, &ExampleJSDialogObject::finishInteractions );

    connect( dialog, &PromobotDialogModule::signalHead, this, &ExampleJSDialogObject::signalHead );
    connect( dialog, &PromobotDialogModule::signalLed, this, &ExampleJSDialogObject::signalLed );
    connect( dialog, &PromobotDialogModule::signalAppParameter, this, &ExampleJSDialogObject::appParameter );
    connect( dialog, &PromobotDialogModule::signalSysParameter, this, &ExampleJSDialogObject::sysParameter );
    connect( dialog, &PromobotDialogModule::signalFaceTracking, this, &ExampleJSDialogObject::signalFaceTracking );
    connect( dialog, &PromobotDialogModule::signalFaceTrackingAuto, this, &ExampleJSDialogObject::signalFaceTrackingAuto );

    connect( dialog, &PromobotDialogModule::signalPromo, wgt, &PromobotMainScreen::loadPromo );
    connect( dialog, &PromobotDialogModule::signalPromoStart, wgt, &PromobotMainScreen::restart );
    connect( wgt, &PromobotMainScreen::signalPromoFinished, dialog, &PromobotDialogModule::setPromoFinished );

    connect( this->TTSService().data(), static_cast<void ( ITTSService::* )( const QString &text, const bool &state )>( &ITTSService::process ), this,
             [&]( const QString &text, const bool &state )
             {
                 if( state ) view_js_api->robotReplicStart( text );
                 else view_js_api->robotReplicFinish();
             } );
    connect( dialog, &PromobotDialogModule::lostMainUser, view_js_api, &lightview::LightView::lostUser );
    connect( dialog, &PromobotDialogModule::signalUserAction, this, &ExampleJSDialogObject::userActionHandler );
}

void ExampleJSDialogObject::initDialog()
{
    // Устанавливает стандартное состояние RGB подсветки
    dialog->setDefaultLed( this->sysParameter( "/led/default_red", 0 ).toInt(), this->sysParameter( "/led/default_green", 0 ).toInt(), this->sysParameter( "/led/default_blue", 255 ).toInt() );
    // Запускает модуль диалогов
    dialog->start();
    // Устанавливает флаг использования процедуры знакомства
    dialog->setAcquaintance( this->sysParameter( "/dialog/useAcquaintance", true ).toBool() );
    // Устанавливает флаг использования прогаммы лояльности
    dialog->setLoyaltyProgram( this->sysParameter( "/dialog/useLoyaltyProgram", false ).toBool() );
    // Устанавливает флаг прерывания произношения по сигналу от распознавания речи
    dialog->setInterruptTTSbyASR( this->sysParameter( "/dialog/interruptTTSbyASR", false ).toBool() );
    use_interrupt_tts = this->sysParameter( "/dialog/interruptTTSbyVAD", true ).toBool();
    // Устанавливает ограничение времени для промо-речёвок
    dialog->setTimeoutPromo( this->sysParameter( "/dialog/timeoutPromo", 30 ).toInt() );
    // Устанавливает флаг использования навигации
    dialog->setUseNavigation( this->sysParameter( "/driving/useMap", false ).toBool() );
    // Устанавливает идентификатор главной точки
    dialog->setIdMainPoint( this->sysParameter( "/navigation/idMainPoint", 1 ).toInt() );
    // Устанавливает флаг возвращения к главной точке
    dialog->setBackToMainPoint( this->sysParameter( "/navigation/backMainPoint", true ).toBool() );
    // Устанавливает флаг игнорирования взаимодействий
    dialog->setIgnoreInteractions( this->sysParameter( "/navigation/ignoreInteractions", true ).toBool() );
}

void ExampleJSDialogObject::callbackLanguage( const QString &lang )
{
    // Устанавливает текущий язык
    current_lang = lang;
    dialog->setLanguage( lang );
}

void ExampleJSDialogObject::callbackInteraction( const bool &value )
{
    // Устанавливает статус взаимодействия
    dialog->setInteraction( value );
}

bool ExampleJSDialogObject::slotStartApp( const QString &name, const QStringList &args )
{
    // Запускает приложение
    bool state          = false;
    IApplicationPtr app = application( name );
    if( app ) state = app->start( args );
    return state;
}

void ExampleJSDialogObject::slotAnswersQuery( const Question &q )
{
    // Отправляет запрос в вопрос-ответную систему
    emit signalAnswersQuery( q );
}

void ExampleJSDialogObject::callbackNetworkPing( const double &value )
{
    // Устанавливает значение задержки подключения к сети
    dialog->setNetworkPing( value );
}

void ExampleJSDialogObject::callbackBatteryStatus( const BatteryState &value )
{
    // Устанавливает статус низкого заряда батареи
    dialog->setBatteryLow( value );
}

void ExampleJSDialogObject::callbackDriveMode( const DriveMode &value )
{
    // Устанавливает режим передвижения робота
    dialog->setDriveMode( value );
    view_js_api->driveMode( value );
}

void ExampleJSDialogObject::callbackDriveStatus( const MovementStatus &value )
{
    // Устанавливает статус передвижения робота
    dialog->setDriveStatus( value );
    view_js_api->driveStatus( value );
}

void ExampleJSDialogObject::callbackDriveChargerStatus( const ChargerMovementStatus &status )
{
    view_js_api->driveChargerStatus( status );
}

void ExampleJSDialogObject::callbackFindFrontObstacle()
{
    // Устанавливает статус обнаружения возникшего препятствия перед роботом
    dialog->setFindFrontObstacle();
}

void ExampleJSDialogObject::callbackPeoplePresenceChanged( const bool &p )
{
    // Устанавливает статус присутствия человека перед роботом
    dialog->setPeoplePresence( p );
}

void ExampleJSDialogObject::callbackTTSProcess( const bool &value, const QUuid &uuid )
{
    // Устанавливает статус процесса синтеза речи
    dialog->setTTSProcess( value, uuid );
    if( !value )
    {
        if( !approach_speech.isNull() )
        {
            view_js_api->extraReplica( approach_speech );
            approach_speech.reset();
        }
        else if( !additional_speech.isNull() )
        {
            view_js_api->extraReplica( additional_speech );
            additional_speech.reset();
        }
        else if( dialog->isNavigationSpeech() ) dialog->startNavigationSpeech();
    }
}

void ExampleJSDialogObject::callbackASRVariant( const QUuid &uuid, const QString &text )
{
    // Устанавливает вариант, полученный от системы распознавания речи
    view_js_api->userSaid( text, false );
    dialog->setVariant( uuid, text, 0, 0.0 );
}

void ExampleJSDialogObject::callbackASRResult( const QUuid &uuid, const QString &text )
{
    // Устанавливает результат распознавания речи
    view_js_api->userSaid( text, true );
    dialog->slotASRResult( uuid, text );
}

void ExampleJSDialogObject::callbackASRProcess( const bool &state )
{
    // Устанавливает статус процесса распознавания речи
    dialog->setASRProcess( state );
}

void ExampleJSDialogObject::callbackASRSpeech( const bool &value )
{
    if( use_interrupt_tts && dialog->isSpeaking() && value )
    {
        qDebug() << "!!! ExampleDialogObject::callbackASRSpeech -> emit signalTTSCancel()";
        // Прерывает речь
        dialog->stopSpeaking( "" );
    }
}

void ExampleJSDialogObject::callbackAnswerResult( const AnswerPtr &value )
{
    qDebug() << "ANSWER ANSWER -" << value.data()->replica().data()->text();
    view_js_api->answerResult( value );
    if( use_additional ||  use_approach )
    {
        approach_speech.reset();
        additional_speech.reset();

        if( dialog->anchor() == DatabaseDefaults::defaultAnchor() &&
            value->replica()->actions().isEmpty() &&
            !value->replica()->url().isValid() &&
            ++count_answer > frequency_additional )
        {
            if( use_approach )
            {
                qDebug() << "approach category" << value->replica()->category();

                approach_speech = signalGetReplica( Replica::APPROACH, QVariant::fromValue( value->replica()->category() ), true, true, dialog->userInfo().info() );
                if( approach_speech.isNull() ) approach_speech = signalGetReplica( Replica::APPROACH, QVariant::fromValue( DatabaseDefaults::defaultCategory() ), true, true, dialog->userInfo().info() );
                if( !approach_speech.isNull() )
                {
                    qDebug() << "approach:" << approach_speech.data()->text();
                }
            }

            if( use_additional )
            {
                additional_speech = signalGetReplica( Replica::ADDITIONAL, QVariant::fromValue( DatabaseDefaults::defaultCategory() ), true, true, dialog->userInfo().info() );

                if( !additional_speech.isNull() )
                {
                    qDebug() << "additional:" << additional_speech.data()->text();
                }
            }

            count_answer = 0;
        }
    }
}

void ExampleJSDialogObject::callbackFaceAddStatus( const FaceTrainStatus &status )
{
    view_js_api->faceAddStatus( status );
}

void ExampleJSDialogObject::callbackFaceGeneral( const qint64 &id )
{
    general_id_prev = general_id;
    general_id      = id;

    qDebug() << "New general id:" << general_id;
    qDebug() << "Prev general id:" << general_id_prev;

    if( general_id != -2 && general_id != general_id_prev )
    {
        this->databaseUsers()->getUserData( id, general_user );
        view_js_api->hasUser( general_user );
    }

    // Устанавливает главного пользователя
    dialog->setFaceGeneral( general_id );
    view_js_api->faceGeneral( general_id );
}

void ExampleJSDialogObject::callbackFaceInfo( const QVector<FaceInfoPtr> &value )
{
    // Устанавливает информацию о лице
    dialog->setFaceInfo( value );
    view_js_api->faceInfo( value );
}

void ExampleJSDialogObject::callbackFaceRect( const QVector<FaceRectPtr> &list )
{
    if( saved_rect_list != list )
    {
        saved_rect_list = list;
        view_js_api->faceRect( list );
    }
}

void ExampleJSDialogObject::callbackScriptEnd()
{
    // Устанавливает флаг окончания скрипта
    dialog->setScriptEnd();
    view_js_api->scriptEnd();
}

void ExampleJSDialogObject::userActionHandler( const Action &a )
{
    QStringList args = a.data()["args"].toString().split( "&", QString::SkipEmptyParts );
    view_js_api->userAction( args );
}

void ExampleJSDialogObject::customEventHandler( const lightview::Message &msg )
{
    qCDebug( lightview_debug ) << "customEventHandler" << msg.attributes();
}

void ExampleJSDialogObject::callbackParametersUpdate()
{
    view_js_api->updateSettings();
}

void ExampleJSDialogObject::callbackOperatorControl( const bool &status )
{
    view_js_api->operatorControl( status );
}

void ExampleJSDialogObject::callbackOperatorResponse( const quint8 &response )
{
    view_js_api->operatorResponse( response );
}

void ExampleJSDialogObject::closeEvent( QCloseEvent *event )
{
    is_close = true;
}

void ExampleJSDialogObject::callbackSipStatus( const SipStatus &status, const QString &info )
{
    view_js_api->sipStatus( status, info );
}

void ExampleJSDialogObject::callbackJoySpeech( const bool &status )
{
    view_js_api->joySpeech( status );
}

void ExampleJSDialogObject::callbackSipDtmf( const QString &value )
{
    view_js_api->sipDtmf( value );
}

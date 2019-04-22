#include "include/exampledialogobject.h"

#include <libqtpromobot/databasedefaults.h>

ExampleDialogObject::ExampleDialogObject():
    use_interrupt_tts( false )
{
    Q_INIT_RESOURCE( resources );
}

ExampleDialogObject::~ExampleDialogObject()
{
    stop();

    delete dialog;
}

void ExampleDialogObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );
    // Создем объект виджета для вывода статусной информации
    lbl_status = new QLabel( this->centralWidget() );
    lbl_status->setAlignment( Qt::AlignCenter );

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

    lbl_status->setText( tr( "Поговорите с роботом." ) );

    QGridLayout *layout_main = new QGridLayout;
    layout_main->setSpacing( 0 );
    layout_main->addWidget( lbl_status, 0, 0, 1, 1 );
    this->centralWidget()->setLayout( layout_main );
}

void ExampleDialogObject::start()
{
    PluginInterface::start();
    // Инициализируем диалог
    initDialog();
}

RetData ExampleDialogObject::stop()
{
    dialog->stop();

    qDebug() << "Promobot Dialog stopped";
    qDebug() << "Promobot MainScreen stopped";
    qDebug() << "Plugin stopped";

    return PluginInterface::stop();
}

void ExampleDialogObject::connectDialogWithOP()
{
    connect( dialog, &PromobotDialogModule::signalStartApp, this, &ExampleDialogObject::slotStartApp );
    connect( dialog, &PromobotDialogModule::signalDriveMode, this, &ExampleDialogObject::signalDriveMode );
    connect( dialog, &PromobotDialogModule::signalDrivePause, this, &ExampleDialogObject::signalDrivePause );
    connect( dialog, &PromobotDialogModule::signalDrivePoint, this, &ExampleDialogObject::signalDrivePoint );
    connect( dialog, &PromobotDialogModule::signalTTSStart, this, &ExampleDialogObject::signalTTSStart );
    connect( dialog, &PromobotDialogModule::signalTTSCancel, this, &ExampleDialogObject::signalTTSCancel );
    connect( dialog, &PromobotDialogModule::signalASRStart, this, &ExampleDialogObject::signalASRStart );
    connect( dialog, &PromobotDialogModule::signalLanguage, this, &ExampleDialogObject::signalLanguage );
    connect( dialog, &PromobotDialogModule::signalUsername, this, &ExampleDialogObject::signalUsername );
    connect( dialog, &PromobotDialogModule::signalUsernames, this, &ExampleDialogObject::signalUsernames );
    connect( dialog, &PromobotDialogModule::signalAnswersQuery, this, &ExampleDialogObject::slotAnswersQuery );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const ID & )>( &PromobotDialogModule::signalAnswersAnchor ),
             this, static_cast<void ( ExampleDialogObject::* )( const ID & )>( &ExampleDialogObject::signalAnswersAnchor ) );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const QString & )>( &PromobotDialogModule::signalAnswersAnchor ),
             this, static_cast<void ( ExampleDialogObject::* )( const QString & )>( &ExampleDialogObject::signalAnswersAnchor ) );
    connect( dialog, &PromobotDialogModule::signalAnswersReset, this, &ExampleDialogObject::signalAnswersReset );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const ID & )>( &PromobotDialogModule::signalAnswersCase ),
             this, static_cast<void ( ExampleDialogObject::* )( const ID & )>( &ExampleDialogObject::signalAnswersCase ) );
    connect( dialog, static_cast<void ( PromobotDialogModule::* )( const QString & )>( &PromobotDialogModule::signalAnswersCase ),
             this, static_cast<void ( ExampleDialogObject::* )( const QString & )>( &ExampleDialogObject::signalAnswersCase ) );
    connect( dialog, &PromobotDialogModule::signalAnswersCaseType, this, &ExampleDialogObject::signalAnswersCaseType );
    connect( dialog, &PromobotDialogModule::signalAnswersCaseReset, this, &ExampleDialogObject::signalAnswersCaseReset );
    connect( dialog, &PromobotDialogModule::signalGetReplica, this, &ExampleDialogObject::signalGetReplica );
    connect( dialog, &PromobotDialogModule::signalGetAppReplica, this, &ExampleDialogObject::signalGetAppReplica );
    connect( dialog, &PromobotDialogModule::signalAnswersCompare, this, &ExampleDialogObject::signalAnswersCompare );
    connect( dialog, &PromobotDialogModule::signalEyes, this, static_cast<void ( ExampleDialogObject::* )( const Animation & )>( &ExampleDialogObject::signalAnimation ) );
    connect( dialog, &PromobotDialogModule::signalScriptStart, this, &ExampleDialogObject::signalScriptStart );
    connect( dialog, &PromobotDialogModule::signalScriptCancel, this, &ExampleDialogObject::signalScriptCancel );
    connect( dialog, &PromobotDialogModule::updateInteractions, this, &ExampleDialogObject::updateInteractions );
    connect( dialog, &PromobotDialogModule::finishInteractions, this, &ExampleDialogObject::finishInteractions );

    connect( dialog, &PromobotDialogModule::signalHead, this, &ExampleDialogObject::signalHead );
    connect( dialog, &PromobotDialogModule::signalLed, this, &ExampleDialogObject::signalLed );
    connect( dialog, &PromobotDialogModule::signalAppParameter, this, &ExampleDialogObject::appParameter );
    connect( dialog, &PromobotDialogModule::signalSysParameter, this, &ExampleDialogObject::sysParameter );
    connect( dialog, &PromobotDialogModule::signalFaceTracking, this, &ExampleDialogObject::signalFaceTracking );
    connect( dialog, &PromobotDialogModule::signalFaceTrackingAuto, this, &ExampleDialogObject::signalFaceTrackingAuto );
}

void ExampleDialogObject::initDialog()
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

void ExampleDialogObject::callbackLanguage( const QString &lang )
{
    // Устанавливает текущий язык
    dialog->setLanguage( lang );
}

void ExampleDialogObject::callbackInteraction( const bool &value )
{
    // Устанавливает статус взаимодействия
    dialog->setInteraction( value );
}

bool ExampleDialogObject::slotStartApp( const QString &name, const QStringList &args )
{
    // Запускает приложение
    bool state          = false;
    IApplicationPtr app = application( name );
    if( app ) state = app->start( args );
    return state;
}

void ExampleDialogObject::slotAnswersQuery( const Question &q )
{
    // Отправляет запрос в вопрос-ответную систему
    emit signalAnswersQuery( q );
}

void ExampleDialogObject::callbackNetworkPing( const double &value )
{
    // Устанавливает значение задержки подключения к сети
    dialog->setNetworkPing( value );
}

void ExampleDialogObject::callbackBatteryStatus( const BatteryState &value )
{
    // Устанавливает статус низкого заряда батареи
    dialog->setBatteryLow( value );
}

void ExampleDialogObject::callbackDriveMode( const DriveMode &value )
{
    // Устанавливает режим передвижения робота
    dialog->setDriveMode( value );
}

void ExampleDialogObject::callbackDriveStatus( const MovementStatus &value )
{
    // Устанавливает статус передвижения робота
    dialog->setDriveStatus( value );
}

void ExampleDialogObject::callbackFindFrontObstacle()
{
    // Устанавливает статус обнаружения возникшего препятствия перед роботом
    dialog->setFindFrontObstacle();
}

void ExampleDialogObject::callbackPeoplePresenceChanged( const bool &p )
{
    // Устанавливает статус присутствия человека перед роботом
    dialog->setPeoplePresence( p );
}

void ExampleDialogObject::callbackTTSProcess( const bool &value, const QUuid &uuid )
{
    // Устанавливает статус процесса синтеза речи
    dialog->setTTSProcess( value, uuid );
}

void ExampleDialogObject::callbackASRVariant( const QUuid &uuid, const QString &text )
{
    // Устанавливает вариант, полученный от системы распознавания речи
    dialog->setVariant( uuid, text, 0, 0.0 );
}

void ExampleDialogObject::callbackASRResult( const QUuid &uuid, const QString &text )
{
    // Принимает результат распознавания речи
    dialog->slotASRResult( uuid, text );
}

void ExampleDialogObject::callbackASRProcess( const bool &state )
{
    // Устанавливает статус процесса распознавания речи
    dialog->setASRProcess( state );
}

void ExampleDialogObject::callbackASRSpeech( const bool &value )
{
    if( use_interrupt_tts && dialog->isSpeaking() && value )
    {
        qDebug() << "!!! ExampleDialogObject::callbackASRSpeech -> emit signalTTSCancel()";
        // Прерывает речь
        dialog->stopSpeaking( "" );
    }
}

void ExampleDialogObject::callbackAnswerResult( const AnswerPtr &value )
{
    if( value.isNull() ) return;
    if( value->replica().isNull() ) return;
    // Устанавливает конечный ответ на фразу пользователя
    dialog->setAnswerResult( value );
}

void ExampleDialogObject::callbackFaceGeneral( const qint64 &id )
{
    general_id_prev = general_id;
    general_id      = id;

    qDebug() << "New general id:" << general_id;
    qDebug() << "Prev general id:" << general_id_prev;
    // Устанавливает главного пользователя
    dialog->setFaceGeneral( general_id );
}

void ExampleDialogObject::callbackFaceInfo( const QVector<FaceInfoPtr> &value )
{
    // Устанавливает информацию о лице
    dialog->setFaceInfo( value );
}

void ExampleDialogObject::callbackScriptEnd()
{
    // Устанавливает флаг окончания скрипта
    dialog->setScriptEnd();
}

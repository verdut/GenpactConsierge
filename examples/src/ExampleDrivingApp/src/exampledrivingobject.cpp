#include "include/exampledrivingobject.h"

ExampleDrivingObject::ExampleDrivingObject()
{
    Q_INIT_RESOURCE( resources );
}

ExampleDrivingObject::~ExampleDrivingObject()
{
    stop();
}

void ExampleDrivingObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );
    // Создаем объект статусного виджета
    lbl_status = new QLabel( this->centralWidget() );
    lbl_status->setAlignment( Qt::AlignCenter );
    // Инициализируем приложение для подъезда к зарядной станции
    app = application( "org.promobot.charging" );

    // Создаем диалоговое окно
    dialog = new Dialog( this->centralWidget() );
    dialog->setTextAlignment( Qt::AlignVCenter | Qt::AlignHCenter );
    // Соединяем сигнал завершения диалога на определенные действия
    connect( dialog, &Dialog::finished, this,
             [&]( int code )
             {
                 if( code == QDialog::Accepted ) // Если диалог подтвержден
                 {
                     dialog_accepted = true;
                     if( charging_app_started ) // И до этого было запущено приложение подъезда к зарядке, то запускаем процесс заново (нужно для повторного подъезда, в случае неудачи)
                     {
                         timer_auto_driving->start( 1000 );
                     }
                     else // если же прилодения подъезда к зарядке не было запущено, то запускаем скрипт который опускает актуатор и приводит руки в исходное положение
                     {
                         startScript( "act_down" );
                         lbl_status->setText( tr( "Выполняю скрипт..." ) );
                     }
                 }
                 else
                 {
                     dialog_accepted = false;
                     QTimer::singleShot( 100, this, &ExampleDrivingObject::signalClose );
                 }
             } );
    // Создаем таймеры автоматичекого передвижения и таймер подзарядки
    timer_auto_driving = new QTimer( this );
    timer_auto_driving->setSingleShot( true );
    connect( timer_auto_driving, &QTimer::timeout, this, &ExampleDrivingObject::slotStartCharging );

    timer_charging = new QTimer( this );
    timer_charging->setSingleShot( true );
    connect( timer_charging, &QTimer::timeout, this, &ExampleDrivingObject::slotStartDriving );
    player = new QMediaPlayer( this );
    // Распологаем статусный виджет на лэйауте плагина
    QGridLayout *layout_main = new QGridLayout;
    layout_main->setSpacing( 0 );
    layout_main->addWidget( lbl_status, 0, 0, 1, 1 );
    this->centralWidget()->setLayout( layout_main );
}

void ExampleDrivingObject::start()
{
    PluginInterface::start();
    dialog_accepted = false;
    is_find_face    = false;
    is_script       = false;
    timer_auto_driving->stop();
    timer_charging->stop();
    lbl_status->setText( "" );
    if( charging_app_started ) // Если до этого было запущено приложение подъезда к зарядке
    {
        if( app->finishStatus() == RetData::Success ) // И завершилось успешно, то стоим на зарядке определенное время
        {
            lbl_status->setText( tr( "Стою на зарядке %1 минут" ).arg( pause_time ) );
            charging_app_started    = false;
            mode                    = DriveMode::MANUAL;
            emit signalDriveMode( mode );
            timer_charging->start( pause_time * 60 * 1000 );
        }
        else // И завершилось не успешно, то выводим диалоговое окно с предложением повторить попытку
        {
            dialog->setText( tr( "Во время подъезда к зарядки произошла ошибка, вы хотите повторить попытку?" ) );
            dialog->open();
        }
    }
    else
    {
        // Если же это превый запуск приложения, то выводим диалоговое окно с вопросом
        dialog->setText( tr( "Вы действительно хотите чтобы я начал автоматическое передвижение?" ) );
        dialog->open();
    }
}

RetData ExampleDrivingObject::stop()
{
    timer_auto_driving->stop();
    timer_charging->stop();
    return PluginInterface::stop();
}

void ExampleDrivingObject::callbackScriptEnd()
{
    // по заершению скрипта либо начинаем автоматическое передвижение, либо сигнализируем что не нашли человека (сигналом foto.wav)
    is_script = false;
    if( dialog_accepted && !charging_app_started && !is_find_face )
    {
        slotStartDriving();
    }
    if( is_find_face )
    {
        emit signalDrivePause( false );
        playAudio( "foto.wav" );
        is_find_face = false;
    }
}

void ExampleDrivingObject::callbackFindFrontObstacle()
{
    // если сработали датчики, то пытаемся найти лицо с помощью скрипта face
    if( startScript( "face" ) )
    {
        emit signalDrivePause( true );
        is_find_face = true;
    }
}

void ExampleDrivingObject::callbackFaceInfo( const QVector<FaceInfoPtr> &list )
{
    // если нашли лицо во время скрипта поиска лица, то сигнализируем об этом с помощью аудиофайла zhit_horosho.mp3
    if( !list.isEmpty() && is_find_face )
    {
        emit signalScriptCancel();
        emit signalDrivePause( false );
        playAudio( "zhit_horosho.mp3" );
    }
}

void ExampleDrivingObject::slotStartDriving()
{
    // Начинаем автоматическое передвижение
    mode = DriveMode::AUTONOMOUS;
    emit signalDriveMode( mode );
    emit signalDrivePause( false );
    lbl_status->setText( tr( "Двигаюсь автоматически %1 минут" ).arg( drive_time ) );
    timer_auto_driving->start( drive_time * 60 * 1000 );
}

void ExampleDrivingObject::slotStartCharging()
{
    // запускаем приложение подъезда к зарядной станции
    charging_app_started = true;
    if( app ) app->start( QStringList() );
}

bool ExampleDrivingObject::startScript( const QString &script_name )
{
    // запускаем скрипт
    bool state = false;
    if( !is_script )
    {
        QStringList parts = emit signalScriptStart( script_name );
        state = !parts.isEmpty();
        if( state ) is_script = true;
    }
    return state;
}

void ExampleDrivingObject::playAudio( const QString &filename )
{
    // воспроизводим аудиофайл
    double duration = promobot::getDuration( filename );
    if( duration > 0.0 )
    {
        player->setMedia( QMediaContent( QUrl::fromLocalFile( Locations::path( Locations::Music ) + QDir::separator() + filename ) ) );
        player->setVolume( 60 );
        player->play();
    }
}

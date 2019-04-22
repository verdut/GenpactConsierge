#ifndef EXAMPLEDRIVINGOBJECT_H
#define EXAMPLEDRIVINGOBJECT_H

#include <QtWidgets>
#include <QMediaPlayer>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtpromobot/promobot.h>
#include <libqtpromobotwidgets/dialog.h>

class ExampleDrivingObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleDrivingObject();
    ~ExampleDrivingObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;
    // Получает сигнал окончания скрипта
    void callbackScriptEnd() Q_DECL_OVERRIDE;
    //  Получает сигнал, если перед роботом есть препятствие
    void callbackFindFrontObstacle() Q_DECL_OVERRIDE;
    // Получает информацию от модуля распознавания лиц
    void callbackFaceInfo( const QVector<FaceInfoPtr> & ) Q_DECL_OVERRIDE;

    // Запускает автоматическое передвижение
    void slotStartDriving();
    // Запускает процесс зарядки
    void slotStartCharging();
    // Запускает скрипт
    bool startScript( const QString & );
    // Воспроизводит аудиофайл
    void playAudio( const QString & );

private:
    QLabel *lbl_status;
    Dialog *dialog = nullptr;
    QTimer *timer_auto_driving;
    QTimer *timer_charging;
    bool dialog_accepted        = false;
    bool charging_app_started   = false;
    bool is_find_face           = false;
    bool is_script              = false;
    DriveMode mode              = DriveMode::MANUAL;
    double drive_time           = 5;
    double pause_time           = 5;
    IApplicationPtr app;
    QMediaPlayer *player;
};

#endif // DRIVINGOBJECT_H

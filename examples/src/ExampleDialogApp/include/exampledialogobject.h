#ifndef EXAMPLEDIALOGOBJECT_H
#define EXAMPLEDIALOGOBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtpromobot/promobot.h>

#include <libqtpromobotdialogmodule/promobotdialogmodule.h>

class ExampleDialogObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleDialogObject();
    ~ExampleDialogObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;
    // Соединяет сигналы диалога со слотами плагина
    void connectDialogWithOP();
    // Инициализирует диалог
    void initDialog();
    // Получает текущий язык
    void callbackLanguage( const QString & ) Q_DECL_OVERRIDE;
    // Получает текущее состояние взаимодействия
    void callbackInteraction( const bool & ) Q_DECL_OVERRIDE;
    // Получает текущий уровень задержки до выбранного сервера
    void callbackNetworkPing( const double & ) Q_DECL_OVERRIDE;
    // Получает текущий статус заряда батареи
    void callbackBatteryStatus( const BatteryState & ) Q_DECL_OVERRIDE;
    // Получает текущий режим передвижения
    void callbackDriveMode( const DriveMode & ) Q_DECL_OVERRIDE;
    // Получает текущий статус передвижения робота
    void callbackDriveStatus( const MovementStatus & ) Q_DECL_OVERRIDE;
    //  Получает сигнал, если перед роботом есть препятствие
    void callbackFindFrontObstacle() Q_DECL_OVERRIDE;
    // Получает статус присутствия человека перед роботом в случае его изменения
    void callbackPeoplePresenceChanged( const bool &p ) Q_DECL_OVERRIDE;
    // Получает процесс синтеза речи
    void callbackTTSProcess( const bool &, const QUuid & ) Q_DECL_OVERRIDE;
    // Получает промежуточный результат процесса распознавания речи
    void callbackASRVariant( const QUuid &, const QString & ) Q_DECL_OVERRIDE;
    // Получает конечный результат процесса распознавания речи
    void callbackASRResult( const QUuid &, const QString & ) Q_DECL_OVERRIDE;
    // Получает статус процесса распознавания речи
    void callbackASRProcess( const bool & ) Q_DECL_OVERRIDE;
    // Получает статус наличия речи
    void callbackASRSpeech( const bool & ) Q_DECL_OVERRIDE;
    // Получает результат поиска ответа
    void callbackAnswerResult( const AnswerPtr & ) Q_DECL_OVERRIDE;
    // Получает информацию текущем главном лице
    void callbackFaceGeneral( const qint64 &id ) Q_DECL_OVERRIDE;
    // Получает информацию от модуля распознавания лиц
    void callbackFaceInfo( const QVector<FaceInfoPtr> & ) Q_DECL_OVERRIDE;
    // Получает сигнал окончания скрипта
    void callbackScriptEnd() Q_DECL_OVERRIDE;

private:
    PromobotDialogModule *dialog;
    QLabel *lbl_status;

    qint64 general_id       = -2,
           general_id_prev  = -2;
    UserData general_user;

    bool use_interrupt_tts;

private slots:
    // Запускает приложение
    bool slotStartApp( const QString &name, const QStringList &args );
    // Отправляет запрос в вопрос-ответную систему
    void slotAnswersQuery( const Question & );
};

#endif // DIALOGOBJECT_H

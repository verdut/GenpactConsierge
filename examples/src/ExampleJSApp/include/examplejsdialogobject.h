#ifndef EXAMPLEJSDIALOGOBJECT_H
#define EXAMPLEJSDIALOGOBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtpromobot/promobot.h>

#include <libqtpromobotdialogmodule/promobotdialogmodule.h>
#include <libqtpromobotdialogmodule/greeting.h>
#include <libqtpromobotmainscreen/promobotmainscreen.h>
#include <lightview/LightView.h>
#include <lightview/Logging.h>

class ExampleJSDialogObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleJSDialogObject();
    ~ExampleJSDialogObject();

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
    // Получает текущий статус передвижения робота к зарядной станции
    void callbackDriveChargerStatus( const ChargerMovementStatus & ) Q_DECL_OVERRIDE;
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
    // Получает информацию о статусах запоминания лица
    void callbackFaceAddStatus( const FaceTrainStatus &status ) Q_DECL_OVERRIDE;
    // Получает информацию текущем главном лице
    void callbackFaceGeneral( const qint64 &id ) Q_DECL_OVERRIDE;
    // Получает информацию от модуля распознавания лиц
    void callbackFaceInfo( const QVector<FaceInfoPtr> & ) Q_DECL_OVERRIDE;
    // Получает информацию о расположении распознанных лиц
    void callbackFaceRect( const QVector<FaceRectPtr> &list ) Q_DECL_OVERRIDE;
    // Получает сигнал окончания скрипта
    void callbackScriptEnd() Q_DECL_OVERRIDE;
    // Получвет сигнал об изменении настроек в БД
    void callbackParametersUpdate() Q_DECL_OVERRIDE;
    // Получает статус управляется ли робот оператором удаленно
    void callbackOperatorControl( const bool &status ) Q_DECL_OVERRIDE;
    // Получает ответ от оператора
    void callbackOperatorResponse( const quint8 &response ) Q_DECL_OVERRIDE;

    void callbackSipDtmf( const QString &value ) Q_DECL_OVERRIDE;

    void callbackSipStatus( const SipStatus &status, const QString &info ) Q_DECL_OVERRIDE;

    // Получает статус использования реплик с джойстика
    void callbackJoySpeech( const bool & ) Q_DECL_OVERRIDE;

protected:
    void closeEvent( QCloseEvent * );

private:
    void userActionHandler( const Action &a );
    void customEventHandler( const lightview::Message &msg );

    PromobotDialogModule *dialog;
    PromobotMainScreen *wgt;
    lightview::LightView *view_js_api;

    qint64 general_id       = -2,
           general_id_prev  = -2;
    UserData general_user;

    bool use_interrupt_tts;
    QVector<FaceRectPtr> saved_rect_list;

    QString current_lang;
    int current_volume  = -1;
    int current_mic     = -1;
    bool is_close;

    bool use_additional,
         use_approach;
    int count_answer,
        frequency_additional;
    ReplicaPtr additional_speech,
               approach_speech;

private slots:
    // Запускает приложение
    bool slotStartApp( const QString &name, const QStringList &args );
    // Отправляет запрос в вопрос-ответную систему
    void slotAnswersQuery( const Question & );
};

#endif // EXAMPLEJSDIALOGOBJECT_H

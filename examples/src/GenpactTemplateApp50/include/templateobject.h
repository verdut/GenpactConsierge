#ifndef TEMPLATEOBJECT_H
#define TEMPLATEOBJECT_H

#include <QtWidgets>
#include <QFont>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtpromobot/led.h>
#include <libqtapplicationwidgets/camera.h>
#include <libqtpromobotdialogmodule/promobotdialogmodule.h>
#include <QSqlDatabase>
#include <QSqlQuery>

class TemplateObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit TemplateObject();
    ~TemplateObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    bool eventFilter(QObject *watched, QEvent *event);

     void start() Q_DECL_OVERRIDE;
     RetData stop() Q_DECL_OVERRIDE;

     void callbackFaceRecognizeStatus(const FaceRecognizeStatus &status) Q_DECL_OVERRIDE;
     void callbackFaceInfo(const QVector<FaceInfoPtr> &list) Q_DECL_OVERRIDE;
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
     void callbackScriptEnd() Q_DECL_OVERRIDE;

private:


    QLabel *label_title,*label_gdpr;
    QLabel *first_name_label, *last_name_label,*code_label,*confirm_reg_label,*form_reg_title_label,*sso_label;
    QTextEdit *first_name_edit, *last_name_edit,*code_edit,*sso_edit;

    QPushButton *buttonMainVisitor, *buttonMainEmployee,*buttonGdprDisagree,*buttonGdprAgree,*buttonConfirmReg,*buttonSSOConfirm;
    QPushButton *buttonBackGdpr,*backButtonVisitor,*backButtonEmployee,*backButtonSSO;

    QWidget *mainWidget, *widget_visitor_data, *employeeWidget,*widget_gdpr,*widget_sso;
    QStackedWidget *pages;

    QTextBrowser *textGdpr;

    Camera *camera= Q_NULLPTR;

    QSqlDatabase db;


    qint64 currentSSO;
    QString currentName;
    QString currentLastName;
    QString currentEmail;
    PromobotDialogModule *dialog;
    qint64 contor=0;
    bool use_interrupt_tts;
    qint64 general_id       = -2, general_id_prev  = -2;
    qint64 passed=0;
    QString fontEdit = "font-family:Verdana;font-size:28px;padding:3px 0;";
    QString fontTitleLabel = "font-family:Verdana;font-size:18px;font-weight:bold;";
    QString fontFormLabel  = "font-family:Verdana;font-size:16px;font-weight:bold;";
    QString fontMainButtons = "font-family:Verdana; font-size:30px;font-weight:bold;";
    QString fontSmallButtons = "font-family:Verdana; font-size:18px;font-weight:bold;";
    QString buttonsStyleSheet = "background-color:#20a7be; color:#ecf0f1; border-width:0; outline:none; border-radius:10px;";
    QString labelStyleSheet = " background-color:#20a7be; ";

private slots:
    void handleVisitorButton();
    void handleEmployeeButton();
    void handleBackButton();
    //void handleGdprBack();
    void handleAgreeGdpr();
    void handleDisagreeGdpr();
    void cameraErrorProcessing( const QString & );
    bool slotStartApp( const QString &name, const QStringList &args );
    // Отправляет запрос в вопрос-ответную систему
    void slotAnswersQuery( const Question & );
    void handleConfirmReg();
    void handleSSOConfirm();


};

#endif // TEMPLATEOBJECT_H



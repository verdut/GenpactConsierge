#ifndef TEMPLATEOBJECT_H
#define TEMPLATEOBJECT_H

#include <QtWidgets>
#include <QFont>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtpromobot/led.h>
#include <libqtapplicationwidgets/camera.h>
#include <libqtpromobotdialogmodule/promobotdialogmodule.h>
#include <QPixmap>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QInputMethod>
#include <QPrinter>


class TemplateObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit TemplateObject();
    ~TemplateObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

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


    ///### Variabile boolene #######
    bool inFaceRec = false;

    ///####### Functii custom ##########


    void updateLabel(QLabel *label);
    int  computeWidth(QLabel *label);
    int computeHeigth(QLabel *label);
    bool eventFilter(QObject *watched, QEvent *event);
    bool checkEdits();
    void validateEdits();
    void printLabel(QString nume, QString prenume, QString functie1, QString functie2, QString valabilitate, QString pathPoza);


private:

    QString currentSSO;
    QString currentName;
    QString currentLastName;
    QString currentEmail;
    QString currentFunction1,currentFunction2;
    QString barcodePath="/home/promobot/Pictures/barcode.png";

    QSqlDatabase db;

    QWidget *welcomePage, *optionPage, *faceRecognitionPage, *twoFAPage, *lastPage;
    QStackedWidget *pages;

    QPixmap backgroundImage=  QPixmap("/home/promobot/Pictures/index.jpg");

    Camera *camera= Q_NULLPTR;

    ///###### Welcome Page #######


    QPixmap welcomeImage= QPixmap("/home/promobot/Pictures/welcome.jpg");

    ///###### Option Page ########

    QPixmap optionImage= QPixmap("/home/promobot/Pictures/option.jpg");

    QLabel *optionTitleLabel,*optionInstructionLabel;
    //QLabel *optionVisitorLabel, *optionEmpLabel;
    //QPixmap visitorImage= QPixmap("/home/promobot/Pictures/visitor.png");
    //QString visitorImage= "border-image:url(/home/bpr/Pictures/visitor.jpg) 0 0 0 0 stretch stretch;";
    //QPixmap employeeImage= QPixmap("/home/promobot/Pictures/employee.png");
    //QString employeeImage = "border-image:url(/home/bpr/Pictures/employee.jpg) 0 0 0 0 stretch stretch;";
    QPushButton *visitorButton, *employeeButton;


    ///##### FaceRec Page #########

    QLabel *faceBackLabel;
    QPixmap faceRecImage =QPixmap("/home/promobot/Pictures/cutout.png");

    ///###### Two FA Page ########

    QLabel *fullNameLabel,*ssoLabel ;
    QLineEdit *fullNameEdit, *ssoEdit;

    QPixmap twoFAImage = QPixmap("/home/promobot/Pictures/auth.jpg");

    ///###### Last Page ########


    QPixmap robotImage = QPixmap("/home/promobot/Pictures/last.jpg");


    ///##### Assets #######

    int labelWidth, labelHeight;

    QString color = "color:#002E5E";



    QString textStyleSheet75 = "font-family:Georgia;font-size:75px;" + color;
    QString textStyleSheet75B = "font-family:Georgia;font-size:75px;font-weight:bold;" +color;
    QString textStyleSheet45 = "font-family:Georgia;font-size:45px;"+ color;
    QString textStyleSheet45B = "font-family:Georgia;font-size:45px;font-weight:bold;" + color;
    QString textStyleSheet30 = "font-family:Georgia;font-size:30px;"+color;
    QString textStyleSheet30B = "font-family:Georgia;font-size:30px;font-weight:bold;"+color;
    QString textStyleSheet20 = "font-family:Georgia;font-size:20px;"+color;
    QString textStyleSheet20B = "font-family:Georgia;font-size:20px;font-weight:bold;"+color;

    QString buttonStyleSheet= "font-family: Georgia; font-size:20px; color: white;  border-radius: 25% ; background-color:white; "\
                              "background:qlineargradient(spread:pad, x1:0 y1:0, x2:1 y2:0, stop:0 rgb(0,46,94), stop:1 rgb(34,168,225)); ";

    QString opac = "opacity:0;background-color:rgba(255,255,255,0);";

    QString editStyleSheet = "font-family:Georgia;color:#FB2E49;border:1.5px solid rgb(0,46,94);border-radius:25%;background-color:white;";

    QString welcomeStyle = ".QWidget{background-image:url(/home/promobot/Pictures/welcome.jpg);}";
   // QString commonStyle  = ".QWidget{background-image:url(/home/promobot/Pictures/index.jpg);}";
    QString optionStyle  = ".QWidget{background-image:url(/home/promobot/Pictures/option.jpg);}";
    QString cutoutStyle  = ".QWidget{background-image:url(/home/promobot/Pictures/cutout.png);}";
    QString authStyle  = ".QWidget{background-image:url(/home/promobot/Pictures/auth.jpg);}";
    QString lastStyle    = ".QWidget{background-image:url(/home/promobot/Pictures/last.jpg);}";



    QPalette pallete;

    ///####### Back-end #######
    qint64 general_id       = -2, general_id_prev  = -2;
    PromobotDialogModule *dialog;
    bool use_interrupt_tts;
    qint64 passed=0,contor=0,interactiv=0;

    ///###### Auxiliary Buttons for Testing - to be commented #####
    QPushButton *welcomeButton,*optionButton, *faceButton, *twoFAButton,*finishRegistration;


private slots:
    void switchToOption();
    void handleClient();
    void handleEmployee();
    void switchTo2FA();
    void switchToWelcome();
    void switchToStart();
    void changeToSSO();
    void cameraErrorProcessing( const QString & );
    bool slotStartApp( const QString &name, const QStringList &args );
    // Отправляет запрос в вопрос-ответную систему
    void slotAnswersQuery( const Question & );
};

#endif // TEMPLATEOBJECT_H

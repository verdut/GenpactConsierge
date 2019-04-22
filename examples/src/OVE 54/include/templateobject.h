#ifndef TEMPLATEOBJECT_H
#define TEMPLATEOBJECT_H

#include <QtWidgets>
#include <QtPrintSupport/qprinter.h>
#include <QtPrintSupport/qprintdialog.h>
#include <QtPrintSupport/QPrinterInfo>
#include <libqtapplicationbase/plugininterface.h>
#include <QNetworkAccessManager>
#include <QUrlQuery>
#include <QHttpMultiPart>
#include <QNetworkReply>
#include <libqtapplicationwidgets/camera.h>
#include <QtPrintSupport/QtPrintSupport>



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

    void makeDecodeRequest(QString s);
    void checkIN(QString QRCode);
    QImage medfilt2(QImage img );
private:

    QString path= "/home/promobot/Pictures/qr/";
    QString formData = "form-data;name=\"file\"; filename=\"";
    QString urlName = "http://api.qrserver.com/v1/read-qr-code/";
    const QString extension = ".png";
    const QString fileName="picture";
    const QString fileName2 = "aux";
    int picVersion = 1;
    QWidget *central;
    QPrinter printer;
    QNetworkReply *reply,*reply1,*reply2,*replyCheckIn;
    QLabel *text_primit;
    Camera *camera = Q_NULLPTR;
    QPushButton *snap,*clear,*connectBtn;
    QNetworkAccessManager *manager;
    QString token;
    QColor white=QColor(255,255,255), black=QColor(0,0,0);

private slots:
    void replyFinished();
    void replyFinished1();
    void findEvents();
    void replyFinished2();
    void cameraErrorProcessing( const QString & );
    void onSnapPress();
    void clearContent();
    void connectToOveApi();
    void replyFinishedCheckIn();
};

#endif // TEMPLATEOBJECT_H

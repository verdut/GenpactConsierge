#include "include/templateobject.h"

TemplateObject::TemplateObject()
{
    Q_INIT_RESOURCE( resources );
}

TemplateObject::~TemplateObject()
{
    stop();
}

void TemplateObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );




    central=new QWidget;

    text_primit=new QLabel(central);
    text_primit->setGeometry(QRect(50,450,1000,100));
    text_primit->setText("Inca nu am primit");

    camera = new Camera( central );
    camera->setGeometry(QRect(50,50,500,300));

    snap = new QPushButton(central);
    snap->setText("SNAP");
    snap->setGeometry(QRect(700,150,150,80));

    clear = new QPushButton(central);
    clear->setText("CLEAR");
    clear->setGeometry(QRect(950,150,150,80));

    connectBtn=new QPushButton(central);
    connectBtn->setText("Press to connect");
    connectBtn->setGeometry(QRect(700,300,300,80));
    connectBtn->adjustSize();

    manager = new QNetworkAccessManager(this);

    //dialog.setModal(true);
    //parent->stackUnder(central);
    //dialog.addEnabledOption(QAbstractPrintDialog::PrintSelect
    //dialog.raise();ion);

    setCentralWidget(central);
    connect( camera, &Camera::error, this, &TemplateObject::cameraErrorProcessing );
    connect(snap,&QPushButton::clicked,this,&TemplateObject::onSnapPress);
    connect(clear,&QPushButton::clicked,this,&TemplateObject::clearContent);
    connect(connectBtn,&QPushButton::clicked,this,&TemplateObject::connectToOveApi);

}



void TemplateObject::start()
{
    PluginInterface::start();

    if( camera->open() ) camera->start();

    QString fileName3 = QFileDialog::getOpenFileName(central,"Open File",QString(),"JPEG File(*.jpeg)");

    QPrintDialog dialog(&printer,central);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPageMargins(10,10,10,10,QPrinter::Millimeter);
    dialog.setWindowTitle(tr("Print Document"));
    if (dialog.exec()==QDialog::Accepted)
       {

        QImage img (fileName3);
        //img = img.scaled(100,,Qt::KeepAspectRatio);
        QPainter painter(&printer);
        painter.drawImage(QPoint(10,10),img);
        painter.end();
    }
}
void TemplateObject::connectToOveApi()
{
    QUrl url;
    url.setUrl("https://l.oveit.com/api/seller/login");
    QUrlQuery query1;
    query1.addQueryItem("email","bucharestrobots@gmail.com");
    query1.addQueryItem("password","AMUITAT2");
    QByteArray data;
    data.append(query1.toString());
    QNetworkAccessManager *manager= new QNetworkAccessManager(this);
    reply = manager->post(QNetworkRequest(url),data);

    connect(reply,&QNetworkReply::finished,this,&TemplateObject::replyFinished);
}

void TemplateObject::onSnapPress(){


   PluginInterface::signalAnimation("black.gif");
   QPixmap snapshot = camera->frame();

   QImage editImage = snapshot.toImage().convertToFormat(QImage::Format_Grayscale8);


   editImage =medfilt2(editImage);

   int width = editImage.size().width();
   int height = editImage.size().height();

   for(int i = 0; i<=width-1;i++)
      for(int j=0;j<=height-1;j++)
      {
          if(editImage.pixelColor(i,j).red()>75)
          {
              editImage.setPixelColor(i,j,white);

          }else {
              editImage.setPixelColor(i,j,black);
          }
      }

   editImage =medfilt2(editImage);

   QPixmap snapshot2 = QPixmap::fromImage(editImage);

   QString version = QString::number(picVersion);

   QString file= path + fileName + version + extension;
   QString file2= path + fileName2+version + extension;

   picVersion++;

   qDebug()<<"Poza s-a salvat:"<<snapshot2.save(file);
   qDebug()<<"Poza 2: "<<snapshot.save(file2);


   makeDecodeRequest(file);


}

QImage TemplateObject::medfilt2(QImage img){

    QImage image(img);

    int width = image.size().width();
    int height = image.size().height();

    for(int i = 0; i<=width-1;i++)
       for(int j=0;j<=height-1;j++)
       {
           if(i==0 || i==width-1 || j==0 || j==height-1)
           {
               image.setPixelColor(i,j,img.pixelColor(i,j));
           }else
           {
               QVector<int> v(9,0);

               v[0]=img.pixelColor(i-1,j-1).red();
               v[1]=img.pixelColor(i-1,j).red();
               v[2]=img.pixelColor(i-1,j+1).red();
               v[3]=img.pixelColor(i,j-1).red();
               v[4]=img.pixelColor(i,j).red();
               v[5]=img.pixelColor(i,j+1).red();
               v[6]=img.pixelColor(i+1,j-1).red();
               v[7]=img.pixelColor(i+1,j).red();
               v[8]=img.pixelColor(i+1,j+1).red();


               qSort(v);
               uint intensity = v[v.size()/2];

               image.setPixelColor(i,j,QColor(intensity,intensity,intensity));


           }
       }

    return image;

}

void TemplateObject::makeDecodeRequest(QString fString){

  QFile *file = new QFile(fString);
  QUrl url;
  url.setUrl(urlName);

  QFileInfo fileInfo(file->fileName());
  QString nume_fisier(fileInfo.fileName());
  QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
  QHttpPart imagePart;

  QString finalData = formData;
  finalData+= nume_fisier+"\"";


  imagePart.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("file"));
  imagePart.setHeader(QNetworkRequest::ContentDispositionHeader,QVariant(finalData));

  file->open(QIODevice::ReadOnly);

  imagePart.setBodyDevice(file);

  file->setParent(multiPart);

  multiPart->append(imagePart);


  QNetworkRequest request(url);
  reply1=manager->post(request,multiPart);
  multiPart->setParent(reply1);

  connect(reply1,&QNetworkReply::finished,this,&TemplateObject::replyFinished1);

}

void TemplateObject::cameraErrorProcessing(const QString &error_text){

   qDebug()<<error_text;
}

void TemplateObject::replyFinished1()
{
    QString data= reply1->readAll();
    qDebug()<<"Am citit deja reply la QR";
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(),&error);

    QJsonValue val = doc.array().first().toObject().value("symbol");
    QJsonArray arr = val.toArray();
    QJsonValue finalValue = arr.first().toObject().value("data");
    qDebug()<<"Urmeaza sa setez textul";

    QString codeData = finalValue.toString();
    if(codeData!="")
     {
        text_primit->setText(codeData);
        text_primit->adjustSize();
        if (!token.isEmpty())
            checkIN(codeData);
    }
    else
      {
        text_primit->setText("Eroare");
        text_primit->adjustSize();
    }



}
void TemplateObject::checkIN(QString QRCode)
{
    QUrl url;
    url.setUrl("https://l.oveit.com/api/tickets/checkin");
    QUrlQuery query;
    query.addQueryItem("token",token);
    query.addQueryItem("ticket_code",QRCode);
    QByteArray data;
    data.append(query.toString());
    replyCheckIn = manager->post(QNetworkRequest(url),data);

    connect(replyCheckIn,&QNetworkReply::finished,this,&TemplateObject::replyFinishedCheckIn);

}
void TemplateObject::replyFinishedCheckIn()
{
    QString data = replyCheckIn->readAll();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(),&error);
    QJsonObject object = doc.object();
    QJsonValue ok = object.value("ok");
    qDebug()<<"Ok value : "<<ok;

    QJsonValue location = object.value("event").toObject().value("location");

    QJsonValue ticketName = object.value("ticket_type").toObject().value("name");

    QJsonArray attendees = object.value("attendee").toObject().value("values").toArray();

    qDebug()<<"Debug: " <<attendees<<' '<<attendees.at(0);

    QJsonValue nume = attendees.at(0);
    QJsonValue prenume = attendees.at(1);

    QString okValue;

    if(ok.toBool()==true)
        okValue = "True";
    else okValue = "False";

    text_primit->setText("Check-in ok:" + okValue + "\n Locatia este:" + location.toString() + "\nTipul biletului este:" + ticketName.toString()
                         + "\nNumele celui ce participa este:" + nume.toString() + " "+ prenume.toString());

    text_primit->adjustSize();
}
void TemplateObject::replyFinished()
{
    QString data = reply->readAll();
    token="";
    int i = 10;
    while (data[i]!=",")
    {
          token=token+data[i];
          i++;
    }
    token=token.left(token.size()-1);
    text_primit->setText(token);
    if (token!="")
    {
        findEvents();
    }
}
void TemplateObject::findEvents()
{
    QUrl url;
    url.setUrl("https://l.oveit.com/api/seller/events");
    QUrlQuery query;
    query.addQueryItem("token",token);
    QByteArray data;
    data.append(query.toString());
    QNetworkAccessManager *manager= new QNetworkAccessManager(this);
    reply2 = manager->post(QNetworkRequest(url),data);
    connect(reply2,&QNetworkReply::finished,this,&TemplateObject::replyFinished2);

}
void TemplateObject::replyFinished2()
{
    QString data = reply2->readAll();
    //data="["+data+"]";
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(),&error);
    QJsonObject object = doc.object();
    QJsonValue val = object.value("events");
    QJsonArray arr = val.toArray();
    QJsonValue finalValueID = arr.first().toObject().value("id");
    QJsonValue finalValueName= arr.first().toObject().value("name");
    qDebug()<<arr;
    text_primit->setText("Numele evenimentului este: " + finalValueName.toString() + " iar id-ul sau este : " + QString::number(finalValueID.toDouble()));
    text_primit->adjustSize();
}
void TemplateObject::clearContent(){

    text_primit->clear();

}
RetData TemplateObject::stop()
{
    return PluginInterface::stop();
}

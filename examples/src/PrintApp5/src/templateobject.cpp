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

    this->centralWidget()->setGeometry(QRect(0,0,1280,800));

    button = new QPushButton(this->centralWidget());
    button->setText("Print");

    button->setGeometry(QRect(600,400,200,100));

    connect(button,&QPushButton::clicked,this,&TemplateObject::handleButton);
}

void TemplateObject::handleButton()
{

 // QImage img(375,150,QImage::Format_RGB32);
 //img.fill(Qt::white);

  QPrinter printer(QPrinter::PrinterResolution);


      printer.setPrinterName("dymo");
      printer.setOutputFormat(QPrinter::NativeFormat);


  QPainter paint;

  //QImage face("/home/costin/Pictures/Barcodes/face.png");


   //QImage scaledFace = face.scaled(70,100,Qt::KeepAspectRatio);
   //scaledFace.save("/home/costin/Pictures/Etichete/face.jpg");

  paint.begin(&printer);

  paint.setFont(QFont("Georgia",12,QFont::Bold));
  paint.drawText(30,20,"Raileanu");
  paint.drawText(30,45,"Costin");
  paint.setFont(QFont("Georgia",10));
  paint.drawText(30,70,"Inginer Sef");
  paint.drawText(30,85,"Front End Developer");
  paint.setFont(QFont("Georgia",8));
  paint.drawText(30,105,"Valid From: 04/15/2019");
  paint.drawText(87,118,"To: 04/15/2019");

  paint.drawImage(30,127,QImage("/home/promobot/Pictures/barcode.png"));

  //paint.drawImage(250,30,scaledFace);

  paint.end();

  //img.save("/home/promobot/Pictures/eticheta1.jpg");

 // PluginInterface::printer(Device::DEV_PRINTER_DYMO)->startPrint("/home/promobot/Pictures/eticheta1.jpg");






}

void TemplateObject::start()
{
    PluginInterface::start();


}

RetData TemplateObject::stop()
{
    return PluginInterface::stop();
}

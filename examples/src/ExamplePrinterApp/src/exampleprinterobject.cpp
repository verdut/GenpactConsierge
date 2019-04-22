#include "exampleprinterobject.h"

ExamplePrinterObject::ExamplePrinterObject()
{
    Q_INIT_RESOURCE( resources );
}

ExamplePrinterObject::~ExamplePrinterObject()
{
    stop();
}

void ExamplePrinterObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );

    // Виджет отображения статуса фотопринтера
    photoStatusLabel = new QLabel( this->centralWidget() );
    photoStatusLabel->setAlignment( Qt::AlignCenter );

    // Виджет отображения статуса фискального принтера
    fiscalStatusLabel = new QLabel( this->centralWidget() );
    fiscalStatusLabel->setAlignment( Qt::AlignCenter );

    buttonPrintFiscal = new QPushButton();
    buttonPrintPicture = new QPushButton();

    // Настройка компоновщика
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing( 0 );
    layout->setContentsMargins(200,100,200,100);
    layout->addWidget( buttonPrintPicture );
    layout->addWidget( buttonPrintFiscal );
    layout->addWidget( photoStatusLabel );
    layout->addWidget( fiscalStatusLabel );

    this->centralWidget()->setLayout( layout );


    // Получаем указатель на объект фотопринтера
    photoPrinter = PluginInterface::printer( DEV_PRINTER_PHOTO  );
    photoPrinter->setObjectName("photoPrinter");

    // Проверяем подключение принтера, вывести ошибку если отсутствует
    if (!photoPrinter->connected()) {
        photoStatusLabel->setText("Ошибка: фотопринтер не подключен");
    }

    // Получаем указатель на объект фискального принтера
    fiscalPrinter = PluginInterface::printer( DEV_PRINTER_RECEIPT  );
    fiscalPrinter->setObjectName("fiscalPrinter");

        // Проверяем подключение принтера, вывести ошибку если отсутствует
    if (!fiscalPrinter->connected()) {
        photoStatusLabel->setText("Ошибка: принтер чеков не подключен");
    }


    // Для отслеживания текущих состояний принтеров
    connect( photoPrinter.data(), &IPrinter::signalState, this, &ExamplePrinterObject::photoPrinterStatusProcessing );
    connect( fiscalPrinter.data(), &IPrinter::signalState, this, &ExamplePrinterObject::fiscalPrinterStatusProcessing );

    // Для запуска печати
    connect( buttonPrintPicture, &QPushButton::clicked, this, &ExamplePrinterObject::printPhoto );
    connect( buttonPrintFiscal, &QPushButton::clicked, this, &ExamplePrinterObject::printFiscalCheck );

}

void ExamplePrinterObject::printPhoto()
{
    // Печатать фотографии или вывод ошибки в случае ошибки
    if (!photoPrinter->startPrint("/home/promobot/.promobot/resources/applications/PrinterApp/kittens.jpg" )) {
        photoStatusLabel->setText("Ошибка печати фотографии: " + photoPrinter->errorString());
    }
}

void ExamplePrinterObject::printFiscalCheck()
{
    // Печать чека на фискальном принтере или вывод ошибки в случае ошибки
    if (!fiscalPrinter->startPrint("/home/promobot/.promobot/resources/applications/PrinterApp/text.txt")) {
        fiscalStatusLabel->setText("Ошибка печати чека");
    }
}


void ExamplePrinterObject::photoPrinterStatusProcessing()
{
    // В случае ошибки принтера выводим соощение ошибки, деактивируем кнопку запуска печати
    if (photoPrinter->state() == PrinterState::HAVE_ERROR) {
        photoStatusLabel->setText("Ошибка принтера: " + photoPrinter->errorString());
        if (buttonPrintPicture->isEnabled())
            buttonPrintPicture->setEnabled(false);
    }
    // В нормальном состоянии выводим сообщение готовности и делаем кнопку активной, если она была деактивирована
    if (photoPrinter->state() == PrinterState::IDLE) {
        photoStatusLabel->setText("Статус: готов к работе");
        if (!buttonPrintPicture->isEnabled())
            buttonPrintPicture->setEnabled(true);
    }
    // Если принтер не подключен вывести сообщение об этом, деактивировать кнопку печати
    if (photoPrinter->state() == PrinterState::NOT_CONNECTED) {
        photoStatusLabel->setText("Принтер не поключен");
        if (buttonPrintPicture->isEnabled())
            buttonPrintPicture->setEnabled(false);
    }
    // В момент печати выводим сообщение об этом, деактивируем кнопку печати для избежания инициализации повторных заданий печати
    if (photoPrinter->state() == PrinterState::PROCESS) {
        photoStatusLabel->setText("Статус: процесс печати фотографии...");
        if (buttonPrintPicture->isEnabled())
            buttonPrintPicture->setEnabled(false);
    }
}

void ExamplePrinterObject::fiscalPrinterStatusProcessing()
{
    // В случае ошибки принтера выводим соощение ошибки, деактивируем кнопку запуска печати
    if (fiscalPrinter->state() == PrinterState::HAVE_ERROR) {
        fiscalStatusLabel->setText("Ошибка принтера: " + photoPrinter->errorString());
        if (buttonPrintFiscal->isEnabled())
            buttonPrintFiscal->setEnabled(false);
    }
    // В нормальном состоянии выводим сообщение готовности и делаем кнопку активной, если она была деактивирована
    if (fiscalPrinter->state() == PrinterState::IDLE) {
        fiscalStatusLabel->setText("Статус: готов к работе");
        if (!buttonPrintFiscal->isEnabled())
            buttonPrintFiscal->setEnabled(true);
    }
    // Если принтер не подключен вывести сообщение об этом, деактивировать кнопку печати
    if (fiscalPrinter->state() == PrinterState::NOT_CONNECTED) {
        fiscalStatusLabel->setText("Принтер не поключен");
        if (buttonPrintFiscal->isEnabled())
            buttonPrintFiscal->setEnabled(false);
    }
    // В момент печати выводим сообщение об этом, деактивируем кнопку печати для избежания инициализации повторных заданий печати
    if (fiscalPrinter->state() == PrinterState::PROCESS) {
        fiscalStatusLabel->setText("Статус: процесс печати фотографии...");
        if (buttonPrintFiscal->isEnabled())
            buttonPrintFiscal->setEnabled(false);
    }
}




void ExamplePrinterObject::start()
{
    PluginInterface::start();


    // Устанавливаем текст виджетов
    photoStatusLabel->setText( tr( "Статус: " ) );
    buttonPrintFiscal->setText( tr("Напечатать текст на \nфискальном принтере") );
    buttonPrintPicture->setText( tr("Напечатать картинку на \nфотопринтере") );
}

RetData ExamplePrinterObject::stop()
{
    return PluginInterface::stop();
}

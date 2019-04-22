#ifndef EXAMPLEPRINTEROBJECT_H
#define EXAMPLEPRINTEROBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtpromobot/iprinter.h>

class ExamplePrinterObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExamplePrinterObject();
    ~ExamplePrinterObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;


public slots:
    // Печать фотографий на фотопринтере
    void printPhoto();
    // Печать чеков на фискальном принтере
    void printFiscalCheck();
    // Отслеживание состояний фотопринтера
    void photoPrinterStatusProcessing();
    // Отслеживание состояний фискального принтера
    void fiscalPrinterStatusProcessing();


private:

    // Виджеты запуска печати
    QPushButton *buttonPrintPicture;
    QPushButton *buttonPrintFiscal;

    // Отображает статус фотопринтера или прогресса печати
    QLabel *photoStatusLabel;

    // Отображает статус фискального принтера или прогресса печати
    QLabel *fiscalStatusLabel;

    // Указатели на объекты принтеров
    IPrinterPtr photoPrinter;
    IPrinterPtr fiscalPrinter;
};

#endif // PRINTEROBJECT_H

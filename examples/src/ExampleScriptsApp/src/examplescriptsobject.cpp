#include "examplescriptsobject.h"
#include "ui_examplescriptsobject.h"

ExampleScriptsObject::ExampleScriptsObject():
    ui( new Ui::ExampleScriptsObject )
{
    Q_INIT_RESOURCE( resources );
}

ExampleScriptsObject::~ExampleScriptsObject()
{
    stop();
    delete ui;
}

void ExampleScriptsObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );
    ui->setupUi( centralWidget() );

    model = new QStringListModel( this );
    // получаем список всех возможных скриптов и отображаем их в QListView
    QDir script_files( Locations::path( Locations::Scripts ) );
    model->setStringList( script_files.entryList( QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst ) );
    ui->listView->setModel( model );
    ui->listView->setEditTriggers( QAbstractItemView::NoEditTriggers );

    // при нажатии на элемент списка вызываем слот itemClicked(QModelIndex)
    connect( ui->listView, SIGNAL(clicked(const QModelIndex)), this, SLOT(itemClicked(QModelIndex)) );
}

void ExampleScriptsObject::callbackScriptEnd()
{
    ui->label->setText( tr( "Выберите скрипт из списка." ) );
}

void ExampleScriptsObject::start()
{
    PluginInterface::start();
    // сбрасываем текст на виджете отображющем статус выполнения скрипта
    ui->label->setText( tr( "Выберите скрипт из списка." ) );
}

RetData ExampleScriptsObject::stop()
{
    return PluginInterface::stop();
}

void ExampleScriptsObject::itemClicked( QModelIndex index )
{
    // вызываем скрипт соответсвующий тексту выбранного элемента
    QStringList parts = emit signalScriptStart( model->stringList().at( index.row() ) );
    if( !parts.isEmpty() ) // если скрипт не пустой
    {
        // обновляем текст на виджете отображющем статус выполнения скрипта
        ui->label->setText( tr( "Выполняю скрипт..." ) );
    }
}

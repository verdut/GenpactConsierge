#include "include/examplecameraobject.h"

ExampleCameraObject::ExampleCameraObject()
{
    Q_INIT_RESOURCE( resources );
}

ExampleCameraObject::~ExampleCameraObject()
{
    stop();
}

void ExampleCameraObject::createWidget( QWidget *parent )
{
    QWidget *wgt_main = new QWidget( parent );
    this->setCentralWidget( wgt_main );

    // Создаем экземпляр камеры
    camera = new Camera( wgt_main );
    // Устанавливаем размер виджета камеры
    camera->setFixedSize( QSize( 720, 404 ) );

    // Виджет извещающий о состоянии камеры
    cameraStatusLabel = new QLabel();

    // Настраиваем компоновщик
    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->setSpacing( 0 );
    layout_main->addWidget( camera );
    layout_main->addWidget( cameraStatusLabel );
    this->centralWidget()->setLayout( layout_main );

    connect( camera, &Camera::error, this, &ExampleCameraObject::cameraErrorProcessing );
}

void ExampleCameraObject::start()
{
    PluginInterface::start();


    if( camera->open() ) camera->start();
    else cameraStatusLabel->setText( "Не могу запустить камеру" );
}

RetData ExampleCameraObject::stop()
{
    return PluginInterface::stop();
}

void ExampleCameraObject::cameraErrorProcessing( const QString &error_text )
{
    cameraStatusLabel->setText( "Ошибка камеры: " + error_text );
}

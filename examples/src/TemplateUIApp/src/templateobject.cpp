#include "include/templateobject.h"
#include "ui_templateobject.h"

TemplateUIObject::TemplateUIObject():
    ui( new Ui::TemplateUIObject )
{
    Q_INIT_RESOURCE( resources );
}

TemplateUIObject::~TemplateUIObject()
{
    stop();
    delete ui;
}

void TemplateUIObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );

    // Настройка UI
    ui->setupUi( centralWidget() );
}

void TemplateUIObject::start()
{
    PluginInterface::start();
}

RetData TemplateUIObject::stop()
{
    return PluginInterface::stop();
}

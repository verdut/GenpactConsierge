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

    lbl_hello = new QLabel( this->centralWidget() );

    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->addWidget( lbl_hello, 0, Qt::AlignCenter );
    this->centralWidget()->setLayout( layout_main );
}

void TemplateObject::start()
{
    PluginInterface::start();

    lbl_hello->setText( tr( "Hello World!" ) );
}

RetData TemplateObject::stop()
{
    return PluginInterface::stop();
}

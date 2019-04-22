#include "include/templateobject.h"

#include <libqtpromobot/promobot.h>

TemplateQMLObject::TemplateQMLObject()
{
    Q_INIT_RESOURCE( resources );
}

TemplateQMLObject::~TemplateQMLObject()
{
    stop();
}

void TemplateQMLObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );

    // Создание виджета для QML
    view = new QQuickWidget( this->centralWidget() );
    view->setAttribute( Qt::WA_AlwaysStackOnTop );
    view->setResizeMode( QQuickWidget::SizeRootObjectToView );
    view->setClearColor( Qt::transparent );
    view->setSource( QUrl::fromLocalFile( ":/TemplateQMLApp/form.qml" ) );

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget( view );
    this->centralWidget()->setLayout( layout );
}

void TemplateQMLObject::start()
{
    PluginInterface::start();
}

RetData TemplateQMLObject::stop()
{
    return PluginInterface::stop();
}

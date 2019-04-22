#ifndef TEMPLATEOBJECT_H
#define TEMPLATEOBJECT_H

#include <QtWidgets>
#include <QtPrintSupport/QPrinter>

#include <libqtapplicationbase/plugininterface.h>

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

private:
   QPushButton *button;

public slots:
   void handleButton();
};

#endif // TEMPLATEOBJECT_H

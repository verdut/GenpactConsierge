#ifndef TEMPLATEOBJECT_H
#define TEMPLATEOBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>

namespace Ui
{
class TemplateUIObject;
}

class TemplateUIObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit TemplateUIObject();
    ~TemplateUIObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;

private:
    Ui::TemplateUIObject *ui;
};

#endif // TEMPLATEOBJECT_H

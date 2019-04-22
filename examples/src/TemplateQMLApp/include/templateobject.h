#ifndef TEMPLATEQMLOBJECT_H
#define TEMPLATEQMLOBJECT_H

#include <QtQuickWidgets>

#include <libqtapplicationbase/plugininterface.h>

class TemplateQMLObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit TemplateQMLObject();
    ~TemplateQMLObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;

private:
    // Виджет для показа QML
    QQuickWidget *view;
};

#endif // TEMPLATEQMLOBJECT_H

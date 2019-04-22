#ifndef EXAMPLECAMERAOBJECT_H
#define EXAMPLECAMERAOBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtapplicationwidgets/camera.h>

class ExampleCameraObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleCameraObject();
    ~ExampleCameraObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;

private slots:
    void cameraErrorProcessing( const QString & );

private:

    // Виджет работы с камерой
    Camera *camera = Q_NULLPTR;
    QLabel *cameraStatusLabel;
};

#endif // EXAMPLECAMERAOBJECT_H

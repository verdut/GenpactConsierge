#ifndef EXAMPLESCRIPTSOBJECT_H
#define EXAMPLESCRIPTSOBJECT_H

#include <QtWidgets>
#include <QtGui>
#include <QStringList>
#include <QStringListModel>
#include <QAbstractItemView>

#include <libqtapplicationbase/plugininterface.h>
#include <libqtpromobot/locations.h>

namespace Ui
{
class ExampleScriptsObject;
}

class ExampleScriptsObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleScriptsObject();
    ~ExampleScriptsObject();
    // создает объект плагина
    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;
    // принимает статус завершения скрипта
    void callbackScriptEnd() Q_DECL_OVERRIDE;
    // вызывается при открытии плагина
    void start() Q_DECL_OVERRIDE;
    // вызывается при закрытии плагина
    RetData stop() Q_DECL_OVERRIDE;

private slots:
    void itemClicked( QModelIndex index );

private:
    Ui::ExampleScriptsObject *ui; // интерфейс плагина
    QStringListModel *model; // модель для отображения списка скриптов в QListView
};

#endif // EXAMPLESCRIPTSOBJECT_H

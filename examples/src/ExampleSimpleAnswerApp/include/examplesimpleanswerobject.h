#ifndef EXAMPLESIMPLEOBJECT_H
#define EXAMPLESIMPLEOBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>

class ExampleSimpleAnswerObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleSimpleAnswerObject();
    ~ExampleSimpleAnswerObject();
    // создает объект плагина
    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;
    // принимает финальный вариант распознанной речи
    void callbackASRResult( const QUuid &, const QString & ) Q_DECL_OVERRIDE;
    // получает ответ от модуля лингвобазы
    void callbackAnswerResult( const AnswerPtr & ) Q_DECL_OVERRIDE;
    // вызывается при открытии плагина
    void start() Q_DECL_OVERRIDE;
    // вызывается при закрытии плагина
    RetData stop() Q_DECL_OVERRIDE;

private:
    QLabel *lbl_question, // виджет для вывода вопроса
           *lbl_answer; // виджет для вывода ответа
};

#endif // EXAMPLESIMPLEOBJECT_H

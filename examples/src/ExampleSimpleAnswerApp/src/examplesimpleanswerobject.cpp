#include "examplesimpleanswerobject.h"

ExampleSimpleAnswerObject::ExampleSimpleAnswerObject()
{
    Q_INIT_RESOURCE( resources );
}

ExampleSimpleAnswerObject::~ExampleSimpleAnswerObject()
{
    stop();
}

void ExampleSimpleAnswerObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );
    // создаем виджет для вывода вопроса и ответа, задаем выравнивание по центру
    lbl_question = new QLabel( this->centralWidget() );
    lbl_question->setAlignment( Qt::AlignCenter );
    lbl_answer = new QLabel( this->centralWidget() );
    lbl_answer->setAlignment( Qt::AlignCenter );

    // распологаем созданные виджеты на виджете плагина
    QGridLayout *layout_main = new QGridLayout;
    layout_main->setSpacing( 0 );
    layout_main->addWidget( lbl_question, 0, 0, 1, 1 );
    layout_main->addWidget( lbl_answer, 1, 0, 1, 1 );
    this->centralWidget()->setLayout( layout_main );
}

void ExampleSimpleAnswerObject::callbackASRResult( const QUuid &, const QString &result )
{
    if( result.isEmpty() ) return;
    // выводим полученный вопрос
    lbl_question->setText( result );

    // формируем запрос в модуль лингвобазы без привязки к полу/возрасту
    Question question;
    question.load( result, *( new FaceInfo ) );
    // отправляем запрос в модуль лингвобазы
    emit signalAnswersQuery( question );
}

void ExampleSimpleAnswerObject::callbackAnswerResult( const AnswerPtr &answer )
{
    // выводим полученный от модуля лингвобазы ответ
    lbl_answer->setText( answer->replica()->text() );
    // отправляем текст ответа на произношение
    emit signalTTSStart( answer->replica()->text() );
    // отправляем эмоцию ответа на лицо
    emit signalAnimation( answer->replica()->animation() );
}

void ExampleSimpleAnswerObject::start()
{
    PluginInterface::start();
    // очищаем виджеты ответа и вопроса
    lbl_question->setText( "" );
    lbl_answer->setText( "" );
}

RetData ExampleSimpleAnswerObject::stop()
{
    return PluginInterface::stop();
}

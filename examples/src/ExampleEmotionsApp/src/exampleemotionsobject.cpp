#include "exampleemotionsobject.h"

ExampleEmotionsObject::ExampleEmotionsObject()
{
    Q_INIT_RESOURCE( resources );
}

ExampleEmotionsObject::~ExampleEmotionsObject()
{
    stop();
}

void ExampleEmotionsObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );

    lbl = new QLabel( this->centralWidget() );
    lbl->setAlignment( Qt::AlignCenter );

    // Создание виджетов для активации эмоций
    buttonEmojiStandart = new QPushButton("Standart");
    buttonEmojiSurprise = new QPushButton("Surpruse");
    buttonEmojiSmile = new QPushButton("Smile");
    buttonEmojiAnger = new QPushButton("Anger");
    buttonEmojiLaugh = new QPushButton("Laugh");
    buttonEmojiLowBattery = new QPushButton("Low Battery");
    buttonEmojiScan = new QPushButton("Scan");
    buttonEmojiSpeak = new QPushButton("Speak");
    buttonEmojiHeart = new QPushButton("Heart");

    buttonEmojiStandart->setObjectName( "btn" );
    buttonEmojiAnger->setObjectName( "btn" );
    buttonEmojiHeart->setObjectName( "btn" );
    buttonEmojiLaugh->setObjectName( "btn" );
    buttonEmojiLowBattery->setObjectName( "btn" );
    buttonEmojiScan->setObjectName( "btn" );
    buttonEmojiSmile->setObjectName( "btn" );
    buttonEmojiSpeak->setObjectName( "btn" );
    buttonEmojiSurprise->setObjectName( "btn" );

    // Настройка компоновщика
    QVBoxLayout *layout_main = new QVBoxLayout;
    layout_main->setContentsMargins(120,0,120,0);
    layout_main->setSpacing( 0 );
    layout_main->addWidget( lbl );
    layout_main->addWidget( buttonEmojiStandart );
    layout_main->addWidget( buttonEmojiSurprise);
    layout_main->addWidget( buttonEmojiLaugh);
    layout_main->addWidget( buttonEmojiScan);
    layout_main->addWidget( buttonEmojiSmile);
    layout_main->addWidget( buttonEmojiSpeak);
    layout_main->addWidget( buttonEmojiAnger );
    layout_main->addWidget( buttonEmojiHeart);
    layout_main->addWidget( buttonEmojiLowBattery);

    this->centralWidget()->setLayout( layout_main );

    // Активация эмоций при клике на кнопки через посылку сигнала с параметром эмоции
    connect(buttonEmojiStandart, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::STANDARD ); });
    connect(buttonEmojiSurprise, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::SURPRISE); });
    connect(buttonEmojiLaugh, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::LAUGH ); });
    connect(buttonEmojiScan, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::SCAN ); });
    connect(buttonEmojiSmile, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::SMILE ); });
    connect(buttonEmojiSpeak, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::SPEAK ); });
    connect(buttonEmojiAnger, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::ANGER ); });
    connect(buttonEmojiHeart, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::HEART ); });
    connect(buttonEmojiLowBattery, &QPushButton::clicked, [=]() { emit signalAnimation( Animation::LOW_BATTERY ); });


}

void ExampleEmotionsObject::start()
{
    PluginInterface::start();

    // Задание текста описывающего действие пользователя
    lbl->setText( tr( "Нажмите на кнопку для выбора эмоции" ) );
}

RetData ExampleEmotionsObject::stop()
{
    return PluginInterface::stop();
}

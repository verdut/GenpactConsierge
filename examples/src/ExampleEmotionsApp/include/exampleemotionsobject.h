#ifndef EMOJIOBJECT_H
#define EMOJIOBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>

class ExampleEmotionsObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleEmotionsObject();
    ~ExampleEmotionsObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;

private:

    // Виджет описывающий действие пользователя
    QLabel *lbl;

    // Виджеты для активации эмоции
    QPushButton *buttonEmojiStandart;
    QPushButton *buttonEmojiSurprise;
    QPushButton *buttonEmojiAnger;
    QPushButton *buttonSorrow;
    QPushButton *buttonEmojiLaugh;
    QPushButton *buttonEmojiSmile;
    QPushButton *buttonEmojiScan;
    QPushButton *buttonEmojiHeart;
    QPushButton *buttonEmojiLowBattery;
    QPushButton *buttonEmojiSpeak;
};

#endif // EmojiObject_H

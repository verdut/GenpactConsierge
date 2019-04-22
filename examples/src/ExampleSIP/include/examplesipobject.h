#ifndef EXAMPLESIPOBJECT_H
#define EXAMPLESIPOBJECT_H

#include <QtWidgets>

#include <libqtapplicationbase/plugininterface.h>

namespace promobot
{
namespace examples
{
class ExampleSIPObject: public PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID PluginInterface_iid FILE "description.json" )
    Q_INTERFACES( PluginInterface )

public:
    explicit ExampleSIPObject();
    ~ExampleSIPObject();

    void createWidget( QWidget *parent = Q_NULLPTR ) Q_DECL_OVERRIDE;

    void start() Q_DECL_OVERRIDE;
    RetData stop() Q_DECL_OVERRIDE;

    void callbackFaceInfo( const QVector<FaceInfoPtr> &list ) Q_DECL_OVERRIDE;

    void callbackSipStatus( const SipStatus &status, const QString &info ) Q_DECL_OVERRIDE;

protected:
    void timerEvent( QTimerEvent *event ) Q_DECL_OVERRIDE;

private:
    int timer_id;

    bool is_sip_active;
    QString phone_number;
    QString text_template;
    QMultiMap<qint64, UserData> cur_info_list, prev_info_list;
    QMap<qint64, QString> map_alert_names;
};
} // namespace examples
} // namespace promobot

#endif // EXAMPLESIPOBJECT_H

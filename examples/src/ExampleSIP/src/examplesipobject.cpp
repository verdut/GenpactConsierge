#include "include/examplesipobject.h"

using namespace promobot::examples;

ExampleSIPObject::ExampleSIPObject():
    timer_id( -1 ),
    is_sip_active( false ),
    phone_number(),
    text_template()
{
}

ExampleSIPObject::~ExampleSIPObject()
{
    stop();
}

void ExampleSIPObject::createWidget( QWidget *parent )
{
    PluginInterface::createWidget( parent );
}

void ExampleSIPObject::start()
{
    PluginInterface::start();

    phone_number    = appParameter( "/alert/phone_number", "911" ).toString();
    text_template   = appParameter( "/alert/text", "I saw some bad guys: %1" ).toString();

    timer_id = startTimer( 60000 );
}

RetData ExampleSIPObject::stop()
{
    if( is_sip_active )
    {
        signalSipHangup();
        is_sip_active = false;
    }

    if( timer_id != -1 )
    {
        killTimer( timer_id );
        timer_id = -1;
    }

    cur_info_list.clear();
    prev_info_list.clear();
    map_alert_names.clear();

    return PluginInterface::stop();
}

void ExampleSIPObject::callbackFaceInfo( const QVector<FaceInfoPtr> &list )
{
    cur_info_list.swap( prev_info_list );
    cur_info_list.clear();

    for( const FaceInfoPtr &item : list )
    {
        UserData prev_data;
        QMultiMap<qint64, UserData>::iterator it;
        if( ( it = prev_info_list.find( item->idTrack() ) ) != prev_info_list.end() )
        {
            prev_data = it.value();
        }

        if( item->type() != FaceInfo::NONE )
        {
            UserData data;
            if( prev_data.info().type() > FaceInfo::UNKNOWN && item->type() == FaceInfo::UNKNOWN )
            {
                // face lost
            }
            else if( prev_data.info().type() == FaceInfo::UNKNOWN
                     || prev_info_list.isEmpty()
                     || item->id() != prev_data.info().id() )
            {
                // face is recognized
                bool status = this->databaseUsers()->getUserData( item->id(), data );
                if( status )
                {
                    if( data.action() == UserData::ALARM )
                    {
                        qDebug() << "ALERT:" << data.info().id() << data.name();
                        map_alert_names.insert( data.info().id(), data.name() );
                    }
                }
            }
            else if( !prev_data.isEmpty() )
            {
                data = prev_data;
            }

            data.setInfo( item );
            cur_info_list.insert( item->idTrack(), data );
        }
    }
}

void ExampleSIPObject::callbackSipStatus( const SipStatus &status, const QString &info )
{
    Q_UNUSED( info );

    is_sip_active = ( status != SipStatus::IDLE );
}

void ExampleSIPObject::timerEvent( QTimerEvent *event )
{
    if( event->timerId() == timer_id
        && !is_sip_active
        && !map_alert_names.isEmpty() )
    {
        QStringList names;
        QMapIterator<qint64, QString> it( map_alert_names );
        while( it.hasNext() )
        {
            it.next();
            names.append( it.value() );
        }
        map_alert_names.clear();

        QString text = text_template.arg( names.join( ", " ) );

        qDebug() << "CALL with text:" << text;
        emit signalSipCallWithText( phone_number, text );
    }
}

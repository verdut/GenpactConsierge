import QtQuick 2.0
import QtQuick.Controls 1.4

Rectangle {
    id: rectangle
    width: 1280
    height: 770
    anchors.margins: 0
    color: "#00000000"

    Flow {
        id: flow
        anchors.margins: 10
        anchors.centerIn: parent
        spacing: 20

        Label {
            id: text
            anchors.fill: parent
            font.family: "Roboto"
            font.pixelSize: 26
            color: "#9A000000"
            text: qsTr("Hello World!")
        }
    }
}

import QtQuick 2.1
import QtQuick.Controls 1.0

Rectangle {
    Rectangle {
        id: rootRectangle
        width: 220
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color.a: 0.5


        Button {
            text: qsTr("Hello World")
            anchors.centerIn: parent
        }
    }
}

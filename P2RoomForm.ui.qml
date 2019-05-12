import QtQuick 2.12
import QtQuick.Controls 2.5

Page {
    width: 720
    height: 1180

    header: Label {
        text: qsTr("Room")
        font.pixelSize: Qt.application.font.pixelSize * 2
        padding: 10
    }

    Label {
        text: qsTr("You are on Page 2.")
        anchors.centerIn: parent
    }
}

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "Constants"

Item {
    property string backgroundColor: "Orange"
    property alias roomsRepeater: roomsRepeater
    property alias scenesRepeater: scenesRepeater
    property alias home: home
    property alias settings: settings
    property alias controllerList: controllerList
    property alias lightList: lightList

    Rectangle {
        color: backgroundColor
        anchors.fill: parent
    }

    Flickable {
        id: flickable
        clip: true
        anchors.fill: parent
        contentHeight: mainColumnLayout.height

        ColumnLayout {
            id: mainColumnLayout
            width: flickable.width
            spacing: 0

            ItemDelegate {
                id: home
                text: qsTr("Home")
                icon.source: ILStyle.homeIconSource
                Layout.fillWidth: true
            }

            MenuSeparator {
                topPadding: 0
                bottomPadding: 0
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Rooms")
                rightPadding: 8
                font.pointSize: 10
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
            }

            Repeater {
                id: roomsRepeater
            }

            MenuSeparator {
                bottomPadding: 0
                topPadding: 0
                Layout.fillWidth: true
            }

            Label {
                text: qsTr("Scenes")
                rightPadding: 8
                font.pointSize: 10
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignRight
            }

            Repeater {
                id: scenesRepeater
            }

            MenuSeparator {
                bottomPadding: 0
                topPadding: 0
                Layout.fillWidth: true
            }

            ItemDelegate {
                id: settings
                text: qsTr("Settings")
                Layout.fillWidth: true
                icon.source: ILStyle.settingsIconSource
            }

            ItemDelegate {
                id: controllerList
                text: qsTr("Controllers")
                Layout.fillWidth: true
                icon.source: ILStyle.controllersIconSource
            }

            ItemDelegate {
                id: lightList
                text: qsTr("Lights")
                Layout.fillWidth: true
                icon.source: ILStyle.lightsIconSource
            }
        }
    }
}




/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;invisible:true}
}
 ##^##*/

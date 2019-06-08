import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

ApplicationWindow {
    id: window
    visible: true
    width: 360
    height: 640
    title: qsTr("BLE demo")

    header: ToolBar {
        id: toolBar
        anchors.right: parent.right
        anchors.left: parent.left
        RowLayout {
            anchors.fill: parent
            Label {
                text: "BLE demo"
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
            ToolButton {
                onClicked: lightsUart.scan()
                enabled: !lightsUart.scanning
                icon.source: "images/material.io--baseline-perm_scan_wifi-24px.svg"
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        visible: lightsUart.scanning
    }

    Label {
        id: messageLabel
        visible: lightsUart.message !== ""
        text: lightsUart.message
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        horizontalAlignment: Text.AlignHCenter

        background: Rectangle {
            color: "black"
            opacity: 0.2
        }
    }
}

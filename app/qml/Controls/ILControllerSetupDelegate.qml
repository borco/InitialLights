import QtQuick 2.13
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.13

import "../Constants"
import InitialLights 1.0

Rectangle {
    id: root

    property string name: "Unnamed"
    property string address: "ACCF24634326FA12"
    property bool isOnline: true
    property bool isEnabled: true
    property var kind: Controller.Unknown

    signal clicked()

    implicitHeight: 60
    implicitWidth: 360

    color: isOnline
           ? (isEnabled && kind != Controller.Unknown
              ? "#FFFFFF"
              : "#F3F4F5"
              )
           : "#f7eded"
    border.color: isOnline
                  ? "#E3E5E8"
                  : "#ffd4d4"
    radius: 4

    RowLayout {
        anchors.fill: parent
        Image {
            Layout.rightMargin: 16
            Layout.leftMargin: 16
            source: isOnline
                    ? (isEnabled && kind != Controller.Unknown
                       ? "../Images/Controller-Enabled.svg"
                       : "../Images/Controller-Disabled.svg" )
                    : "../Images/Controller-Offline.svg"
        }

        ColumnLayout {
            Layout.fillWidth: true
            Text {
                text: name
                Layout.fillWidth: true
                font: Qt.font({ family: "Inter", styleName: "Medium", pointSize: 14 })
                color: isOnline
                       ? "#000"
                       : "#ff2f2f"
            }
            Text {
                text: address
                Layout.fillWidth: true
                font: Qt.font({ family: "Inter", styleName: "Medium", pointSize: 12 })
                color: isOnline
                       ? "#5C6670"
                       : "#ff9090"
            }
        }

        Rectangle {
            Layout.leftMargin: 16
            Layout.rightMargin: 16

            implicitWidth: 100
            implicitHeight: 32
            color: isOnline
                   ? (isEnabled && kind != Controller.Unknown ? "#C2F2C2" : "#C7CCD2")
                   : "#ffafaf"

            border.color: isOnline
                          ? (isEnabled && kind != Controller.Unknown ? "#8AE68A" : "#ABB2BA")
                          : "#ff9090"
            border.width: 1
            radius: 4

            Text {
                anchors.centerIn: parent
                text: isOnline
                      ? (kind === Controller.Unknown
                         ? qsTr("NOT CONFIGURED")
                         : (isEnabled
                            ? qsTr("ENABLED")
                            : qsTr("DISABLED") )
                         )
                      : qsTr("OFFLINE")
                font: Qt.font({ family: "Inter", styleName: "Medium", pointSize: 10 })
                color: isOnline
                       ? (isEnabled && kind != Controller.Unknown ? "#21be2b" : "#fff")
                       : "#ff2f2f"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                elide: Text.ElideRight
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }
}

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../../Constants"
import "../../Controls"

Page {
    id: root

    implicitWidth: 360
    implicitHeight: 640

    signal next()

    property var rooms

    background: Rectangle {
        color: ILStyle.backgroundColor
    }

    header: ILToolBar {
        id: iLToolBar
        ILToolBarTitle {
            text: qsTr("Setup your space")
        }

        ILButton {
            text: qsTr("Next")
            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter
            flat: true
            onClicked: next()
        }
    }

    Flickable {
        anchors.fill: parent
        contentHeight: columnLayout.height
        ScrollIndicator.vertical: ScrollIndicator {}

        ColumnLayout {
            id: columnLayout
            anchors.top: parent.top
            anchors.rightMargin: 20
            anchors.leftMargin: 20
            anchors.right: parent.right
            anchors.left: parent.left
            ILTitleText {
                text: qsTr("Your Rooms")
                Layout.topMargin: 20
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            ILInfoText {
                text: qsTr("Start by creating your rooms.<br>This can be done later.")
                Layout.bottomMargin: 20
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }

            Repeater {
                model: rooms ? rooms.items : []

                RowLayout {
                    id: rowLayout

                    ILToolButton {
                        display: AbstractButton.IconOnly
                        icon.source: "../../Images/material.io-remove-24px.svg"
                        icon.width: 32
                        icon.height: 32
                        onClicked: rooms.removeRoom(index)
                    }

                    // TODO: remove the ColumnLayout and the ID
                    ColumnLayout {
                        ILTextField {
                            id: roomNameTextField
                            placeholderText: qsTr("Room Name")
                            textField.text: name
                            textField.onTextEdited: name = textField.text
                        }
                        Text {
                            text: "ID:" + rid
                            font.pixelSize: 8
                        }
                    }
                }
            }

            ILButton {
                text: qsTr("Add new room")
                Layout.bottomMargin: 20
                Layout.topMargin: 20
                Layout.fillWidth: true
                highlighted: true
                onClicked: rooms.appendNewRoom()
            }

        }
    }
}

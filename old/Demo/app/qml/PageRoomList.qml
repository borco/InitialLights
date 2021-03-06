import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "Images"
import "Controls"
import "Constants"

Item {
    id: root

    property string title: qsTr("Rooms")
    property var extraToolbarItems: [
        addRoomButton,
    ]

    property alias model: listView.model
    property alias addRoomButton: addRoomButton

    signal showRoom(int index)

    ToolButton {
        id: addRoomButton
        icon.source: ILStyle.addIconSource
    }

    ListView {
        id: listView
        anchors.fill: parent

        delegate: SwipeDelegate {
            width: parent.width
            text: model.name
            onClicked: showRoom(index)

            swipe.right: Label {
                id: deleteLabel
                text: qsTr("Delete")
                verticalAlignment: Label.AlignVCenter
                padding: 12
                height: parent.height
                anchors.right: parent.right

                SwipeDelegate.onClicked: listView.model.remove(index)

                background: Rectangle {
                    color: deleteLabel.SwipeDelegate.pressed ? Qt.darker("tomato", 1.1) : "tomato"
                }
            }
        }

        Connections {
            target: listView.model
            onRowsInserted: {
                if (first == last) {
                    showRoom(first)
                } else {
                    console.log("inserted %1 rows".arg(last - first + 1))
                }
            }
        }
    }
}

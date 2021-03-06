import QtQuick 2.12
import QtQuick.Layouts 1.12

import "Constants"
import "Controls"
import QtQuick.Controls 2.12

Item {
    id: element
    width: 400
    height: 400

    ColumnLayout {
        id: columnLayout
        anchors.bottom: switchColumnLayout.top
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        ILColorWheel {
            id: canvasColorWheel
            useShaderWheel: false
            visible: canvasWheelSwitch.checked
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        ILColorWheel {
            id: shaderColorWheel
            color: "#00ffff"
            useShaderWheel: true
            visible: shaderWheelSwitch.checked
            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }

    ColumnLayout {
        id: switchColumnLayout
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        Switch {
            id: canvasWheelSwitch
            text: qsTr("Enable Canvas Wheel")
            checked: true
            Layout.fillWidth: true
        }

        Switch {
            id: shaderWheelSwitch
            text: qsTr("Enable Shader Wheel")
            checked: false
            Layout.fillWidth: true
        }
    }

}










/*##^## Designer {
    D{i:1;anchors_height:100;anchors_width:100;anchors_x:118;anchors_y:181}D{i:4;anchors_width:100}
}
 ##^##*/

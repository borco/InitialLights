import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../Constants"

Item {
    id: control
    width: 50
    height: 50

    property int size: Math.min(width, height)
    property color color: "blue"
    property bool isOn: false

    property alias checked: button.checked

    property real buttonOpacity: 0.5

    property color buttonBorderColor: ILStyle.circularToggleLedButton.borderColor
    property int buttonBorderWidth: 2

    property real lightIntensity: 0.3
    property real lightTemperature: 0.7
    property int lightIndicatorWidth: 3

    signal toggled()

    AbstractButton {
        id: button
        checked: false
        anchors.fill: parent
        checkable: true

        background: Item {
            Rectangle {
                width: control.size
                height: width
                radius: width / 2
                color: control.color
                opacity: control.isOn ? 1.0 : control.buttonOpacity
            }

            Rectangle {
                width: control.size
                height: width
                radius: width / 2
                border.color: control.buttonBorderColor
                border.width: control.buttonBorderWidth
                color: "transparent"
            }

            Rectangle {
                id: intensityBackground
                x: (button.width + control.size + control.lightIndicatorWidth) / 2
                height: control.size
                width: control.lightIndicatorWidth
                radius: width / 2
                color: "black"
                opacity: 0.5
            }

            Rectangle {
                id: itensity
                x: intensityBackground.x
                y: control.size - height
                height: control.size * control.lightIntensity
                width: control.lightIndicatorWidth
                radius: width / 2
                color: control.buttonBorderColor
            }

            Rectangle {
                id: temperatureBackground
                x: intensityBackground.x + intensityBackground.width + control.lightIndicatorWidth / 2
                height: control.size
                width: control.lightIndicatorWidth
                radius: width / 2
                color: "black"
                opacity: 0.5
            }

            Rectangle {
                id: temperature
                x: temperatureBackground.x
                y: control.size - height
                height: control.size * control.lightTemperature
                width: control.lightIndicatorWidth
                radius: width / 2
                color: control.color
            }
        }
    }

    Component.onCompleted: {
        button.toggled.connect(toggled)
    }
}

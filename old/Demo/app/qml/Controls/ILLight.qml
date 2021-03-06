import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.12

import "../Constants"

import InitialLights 1.0

RowLayout {
    id: root

    property int margins: 8
    property int valueLabelMinimumWidth: 50
    property int colorSwatchSize: 40
    property int colorSwatchRadius: 5

    property bool isControllerNameVisible: true
    property bool rgbChannelsVisible: true

    property string controllerName: "Controller"

    property var light: null

    property string label: qsTr("%1%2 Light").arg(isControllerNameVisible ? (controllerName + ": ") : "").arg(light !== null ? light.lightTypeName : "")

    signal colorSwatchClicked(var color)

    ColumnLayout {
        id: columnLayout
        width: parent.width

        Label {
            text: root.label
            font.italic: true
            font.pointSize: 8
            Layout.fillWidth: true
            Layout.topMargin: root.margins
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            color: Material.accent
        }

        TextField {
            id: nameTextField
            placeholderText: qsTr("Light Name")
            Layout.fillWidth: true
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            text: root.light !== null ? root.light.name : "Light"
            onTextChanged: if (root.light !== null) { root.light.name = nameTextField.text }
        }

        Rectangle {
            visible: root.light !== null && root.light.lightType === Light.RGB
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            Layout.fillWidth: true
//            width: root.colorSwatchSize
            height: root.colorSwatchSize
            radius: root.colorSwatchRadius
            color:  root.light !== null ? root.light.color : "white"

            MouseArea {
                anchors.fill: parent
                onClicked: root.colorSwatchClicked(parent.color)
            }
        }

        ILLightChannel {
            id: analogicChannel
            visible: root.light !== null && root.light.lightType === Light.Analogic
            Layout.fillWidth: true
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            labelMinimumWidth: root.valueLabelMinimumWidth
            property double displayValue: root.light !== null
                                          ? 10.0 * (value - minValue) / (maxValue - minValue)
                                          : 0
            text: displayValue.toFixed(2) + "V"

            minValue: root.light !== null ? root.light.minValue : 0
            maxValue: root.light !== null ? root.light.maxValue : 100
            valueIncrement: root.light !== null ? root.light.valueIncrement : 1
            value: root.light !== null ? root.light.value : 0
            onValueChanged: if (root.light !== null) root.light.value = analogicChannel.value
            onBlink: if (root.light !== null) root.light.blink(0)
        }

        ILLightChannel {
            id: pwmChannel
            visible: root.light !== null && root.light.lightType === Light.PWM
            Layout.fillWidth: true
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            labelMinimumWidth: root.valueLabelMinimumWidth
            property int displayValue: root.light !== null
                                       ? 100 * (value - minValue) / (maxValue - minValue)
                                       : 0
            text: displayValue + "%"

            minValue: root.light !== null ? root.light.minValue : 0
            maxValue: root.light !== null ? root.light.maxValue : 100
            valueIncrement: root.light !== null ? root.light.valueIncrement : 1
            value: root.light !== null ? root.light.value : 0
            onValueChanged: if (root.light !== null) root.light.value = pwmChannel.value
//            onBlink: if (root.light !== null) root.light.blink(0)
            onBlink: if (root.light !== null) root.light.blink(1) // FIXME: fix for demo
        }

        ILLightChannel {
            id: redChannel
            visible: root.light !== null && root.rgbChannelsVisible && root.light.lightType === Light.RGB
            Layout.fillWidth: true
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            labelMinimumWidth: root.valueLabelMinimumWidth
            property int displayValue: root.light !== null
                                       ? 100 * (value - minValue) / (maxValue - minValue)
                                       : 0
            text: "%1% R".arg(displayValue)

            minValue: root.light !== null ? root.light.minValue : 0
            maxValue: root.light !== null ? root.light.maxValue : 100
            valueIncrement: root.light !== null ? root.light.valueIncrement : 1
            value: root.light !== null ? root.light.redValue : 0
            onValueChanged: if (root.light !== null) root.light.redValue = redChannel.value
//            onBlink: if (root.light !== null) root.light.blink(0)
            onBlink: if (root.light !== null) root.light.blink(-1) // FIXME: fix for demo
        }

        ILLightChannel {
            id: greenChannel
            visible: root.light !== null && root.rgbChannelsVisible && root.light.lightType === Light.RGB
            Layout.fillWidth: true
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            labelMinimumWidth: root.valueLabelMinimumWidth
            property int displayValue: root.light !== undefined
                                       ? 100 * (value - minValue) / (maxValue - minValue)
                                       : 0
            text: "%1% G".arg(displayValue)

            minValue: root.light !== null ? root.light.minValue : 0
            maxValue: root.light !== null ? root.light.maxValue : 100
            valueIncrement: root.light !== null ? root.light.valueIncrement : 1
            value: root.light !== null ? root.light.greenValue : 0
            onValueChanged: if (root.light !== null) root.light.greenValue = greenChannel.value
//            onBlink: if (root.light !== null) root.light.blink(1)
            onBlink: if (root.light !== null) root.light.blink(2) // FIXME: fix for demo
        }

        ILLightChannel {
            id: blueChannel
            visible: root.light !== null && root.rgbChannelsVisible && root.light.lightType === Light.RGB
            Layout.fillWidth: true
            Layout.leftMargin: root.margins
            Layout.rightMargin: root.margins
            labelMinimumWidth: root.valueLabelMinimumWidth
            property int displayValue: root.light !== undefined
                                       ? 100 * (value - minValue) / (maxValue - minValue)
                                       : 0
            text: "%1% B".arg(displayValue)

            minValue: root.light !== null ? root.light.minValue : 0
            maxValue: root.light !== null ? root.light.maxValue : 100
            valueIncrement: root.light !== null ? root.light.valueIncrement : 1
            value: root.light !== null ? root.light.blueValue : 0
            onValueChanged: if (root.light !== null) root.light.blueValue = blueChannel.value
//            onBlink: if (root.light !== null) root.light.blink(2)
            onBlink: if (root.light !== null) root.light.blink(1) // FIXME: fix for demo
        }
    }

    Switch {
        id: isOnSwitch
//        Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        visible: true
        checked: (root.light !== null && root.light.isOn)
        onClicked: if (root.light !== null) root.light.isOn = checked
    }
}

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "../../Constants"
import "../../Controls"

Page {
    id: root

    implicitWidth: 360
    implicitHeight: 640

    signal done()

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: roomsView
    }

    Component {
        id: roomsView
        PageInitialSetupRooms {
            rooms: backend ? backend.rooms : null
            onNext: stackView.push(controllersView)
        }
    }

    Component {
        id: controllersView
        PageInitialSetupControllers {
            controllers: backend ? backend.controllers : null
            onBack: stackView.pop()
            onScan: {
                backend.bluetoothExplorer.search()
                controllersScanDialog.open()
            }
            onClickedController: {
                stackView.push(controllerConfigurationView,
                               { "controller" : backend.controllers.items.get(index)},
                               StackView.Immediate)
            }
            onNext: done()
        }
    }

    Component {
        id: controllerConfigurationView
        PageInitialSetupControllerConfiguration {
            onCanceled: stackView.pop(StackView.Immediate)
        }
    }

    PageInitialSetupControllersScanDialog {
        id: controllersScanDialog
        modal: true

        property int border: 20

        width: root.width - 2 * border
        height: root.height - border

        x: border
        y: root.height

        enter: Transition {
            NumberAnimation { properties: "y"; to: controllersScanDialog.border; duration: 500 }
        }

        exit: Transition {
            NumberAnimation { properties: "y"; to: root.height; duration: 500 }
        }
    }

    Connections {
        target: backend ? backend.bluetoothExplorer : null

        onDiscoverFinished: {
            controllersScanDialog.close()
        }
    }
}

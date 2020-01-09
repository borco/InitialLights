import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import "Constants"
import "Pages"
import "Pages/Developer"
import "Pages/Login"
import "Pages/InitialSetup"

ApplicationWindow {
    id: window
    visible: true
    width: 360
//    width: 480
    height: 640
    title: qsTr("Initial Lights")

    function showOnboarding() {
        console.log("Showing Onboarding")
        mainStackView.replace(null, onboardingView, StackView.Immediate)
    }

    function showLoginLobbyOrStartView() {
        if (!backend.user.isLogged) {
            showLoginLobbyView()
        } else {
            showStartView()
        }
    }

    function showLoginLobbyView() {
        console.log("Showing Login Lobby")
        mainStackView.replace(null, loginLobbyView, StackView.Immediate)
    }

    function showStartView() {
        if (backend.showInitialSetup) {
            console.log("Showing Initial Setup")
            mainStackView.replace(null, initialSetupView, StackView.Immediate)
        } else {
            console.log("Showing Main/Developer")
//            mainStackView.replace(null, mainView, StackView.Immediate)
            mainStackView.replace(null, developerView, StackView.Immediate)
        }
    }

    StackView {
        id: mainStackView
        anchors.fill: parent
    }

    Component {
        id: developerView
        PageDeveloper {
        }
    }

    Component {
        id: onboardingView
        PageOnboarding {
            onDone: backend.showOnboarding = false
        }
    }

    Component {
        id: loginLobbyView
        PageLoginLobby {
            stackView: mainStackView
            user: backend ? backend.user : null
        }
    }

    Component {
        id: initialSetupView
        PageInitialSetup {
            onDone: backend.showInitialSetup = false
        }
    }

//    Component {
//        id: mainView
//        PageMain {
//            stackView: mainStackView
//        }
//    }

    Component.onCompleted: {
        if (backend.showOnboarding) {
            showOnboarding()
        } else {
            showLoginLobbyOrStartView()
        }
    }

    Connections {
        target: backend

        onShowOnboardingChanged: {
            if (backend.showOnboarding) {
                showOnboarding()
            } else {
                showLoginLobbyOrStartView()
            }
        }

        onShowInitialSetupChanged: showLoginLobbyOrStartView()
    }

    Connections {
        target: backend ? backend.user : null
        onIsLoggedChanged: showLoginLobbyOrStartView()
    }

    Connections {
        target: Qt.application
        onStateChanged: {
            if (Qt.application.state === Qt.ApplicationSuspended) {
                console.log("application suspended")
                backend.saveLocalData()
            }
        }
        onAboutToQuit: {
            console.log("application about to quit")
            backend.saveLocalData()
        }
    }
}

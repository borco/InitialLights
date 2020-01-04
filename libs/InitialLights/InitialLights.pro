TEMPLATE = lib

QT += qml bluetooth

TARGET = InitialLights
CONFIG += c++17

# we're using static libs for easy deploy on mobile platforms
CONFIG += static

DEFINES += QTQMLTRICKS_NO_PREFIX_ON_GETTERS

DEFINES += INITIALLIGHTS_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        il/backend.cpp \
        il/bluetooth/bluetoothcontroller.cpp \
        il/bluetooth/bluetoothexplorer.cpp \
        il/bluetooth/ibluetoothcontroller.cpp \
        il/bluetooth/ibluetoothexplorer.cpp \
        il/controllers/controller.cpp \
        il/controllers/controllercollection.cpp \
        il/gui/config.cpp \
        il/iindexed.cpp \
        il/iindexer.cpp \
        il/jsonhelper.cpp \
        il/lights/analogiclight.cpp \
        il/lights/ilight.cpp \
        il/lights/lightbase.cpp \
        il/lights/lightcollection.cpp \
        il/lights/lightkind.cpp \
        il/lights/rgbwlight.cpp \
        il/room.cpp \
        il/roomcollection.cpp \
        il/simpleindexer.cpp \
        il/user.cpp

HEADERS += \
        il/backend.h \
        il/bluetooth/bluetoothcontroller.h \
        il/bluetooth/bluetoothexplorer.h \
        il/bluetooth/ibluetoothcontroller.h \
        il/bluetooth/ibluetoothexplorer.h \
        il/controllers/controller.h \
        il/controllers/controllercollection.h \
        il/gui/config.h \
        il/iindexed.h \
        il/iindexer.h \
        il/jsonhelper.h \
        il/lights/analogiclight.h \
        il/lights/ilight.h \
        il/lights/lightbase.h \
        il/lights/lightcollection.h \
        il/lights/lightkind.h \
        il/lights/rgbwlight.h \
        il/room.h \
        il/roomcollection.h \
        il/simpleindexer.h \
        il/user.h \
        initiallights_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}


# QtQmlModels

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../3rdparty/QtQmlModels/release/ -lQtQmlModels
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../3rdparty/QtQmlModels/debug/ -lQtQmlModels
else:unix: LIBS += -L$$OUT_PWD/../../3rdparty/QtQmlModels/ -lQtQmlModels

INCLUDEPATH += $$PWD/../../3rdparty/QtQmlModels
DEPENDPATH += $$PWD/../../3rdparty/QtQmlModels

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtQmlModels/release/libQtQmlModels.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtQmlModels/debug/libQtQmlModels.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtQmlModels/release/QtQmlModels.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtQmlModels/debug/QtQmlModels.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtQmlModels/libQtQmlModels.a

# QtSuperMacros

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../3rdparty/QtSuperMacros/release/ -lQtSuperMacros
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../3rdparty/QtSuperMacros/debug/ -lQtSuperMacros
else:unix: LIBS += -L$$OUT_PWD/../../3rdparty/QtSuperMacros/ -lQtSuperMacros

INCLUDEPATH += $$PWD/../../3rdparty/QtSuperMacros
DEPENDPATH += $$PWD/../../3rdparty/QtSuperMacros

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtSuperMacros/release/libQtSuperMacros.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtSuperMacros/debug/libQtSuperMacros.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtSuperMacros/release/QtSuperMacros.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtSuperMacros/debug/QtSuperMacros.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../../3rdparty/QtSuperMacros/libQtSuperMacros.a

DISTFILES += \
    README.md

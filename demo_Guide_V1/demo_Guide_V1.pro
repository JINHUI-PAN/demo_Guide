QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    QRoundProgressBar.cpp \
    configwindow.cpp \
    diarywindow.cpp \
    hell_window.cpp \
    main.cpp \
    demo_guide.cpp \
    mqtt_window.cpp \
    network_detect.cpp \
    onewindow.cpp \
    parameter_window.cpp \
    passwork.cpp \
    project_information.cpp \
    remotecontrol.cpp \
    remoteset.cpp \
    serialport.cpp \
    syetem_information.cpp \
    user_instructions.cpp

HEADERS += \
    Data.h \
    QRoundProgressBar.h \
    configwindow.h \
    demo_guide.h \
    diarywindow.h \
    hell_window.h \
    mqtt_window.h \
    network_detect.h \
    onewindow.h \
    parameter_window.h \
    passwork.h \
    project_information.h \
    remotecontrol.h \
    remoteset.h \
    serialport.h \
    syetem_information.h \
    user_instructions.h

FORMS += \
    configwindow.ui \
    demo_guide.ui \
    diarywindow.ui \
    hell_window.ui \
    mqtt_window.ui \
    onewindow.ui \
    parameter_window.ui \
    passwork.ui \
    project_information.ui \
    remotecontrol.ui \
    remoteset.ui \
    serialport.ui \
    syetem_information.ui \
    user_instructions.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES +=



win32:CONFIG(release, debug|release): LIBS += -L'F:/Goolge Download/qtmqtt-5.12.12/build-qtmqtt-Desktop_Qt_5_12_12_MinGW_64_bit-Release/lib/' -lQt5Mqtt
else:win32:CONFIG(debug, debug|release): LIBS += -L'F:/Goolge Download/qtmqtt-5.12.12/build-qtmqtt-Desktop_Qt_5_12_12_MinGW_64_bit-Release/lib/' -lQt5Mqttd
else:unix: LIBS += -L'F:/Goolge Download/qtmqtt-5.12.12/build-qtmqtt-Desktop_Qt_5_12_12_MinGW_64_bit-Release/lib/' -lQt5Mqtt

INCLUDEPATH += 'F:/Goolge Download/qtmqtt-5.12.12/build-qtmqtt-Desktop_Qt_5_12_12_MinGW_64_bit-Release/include'
DEPENDPATH += 'F:/Goolge Download/qtmqtt-5.12.12/build-qtmqtt-Desktop_Qt_5_12_12_MinGW_64_bit-Release/include'

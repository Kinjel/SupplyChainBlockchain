QT       += widgets charts

CONFIG += c++17

SOURCES += \
    block.cpp \
    blockchain.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    block.h \
    blockchain.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

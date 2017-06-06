QT += qml quick

CONFIG += c++11
HEADERS += \
    beziqueDeck.h \
    player.h \
    card.h \
    test.h \
    beziquehand.h \
    gamedata.h \
    gamestate.h \
    unseencards.h \
    aievaluate.h \
    aiendplay.h \
    scores.h \
    beziquematch.h
SOURCES += main.cpp \
    beziqueDeck.cpp \
    player.cpp \
    card.cpp \
    test.cpp \
    beziquehand.cpp \
    gamedata.cpp \
    gamestate.cpp \
    unseencards.cpp \
    aievaluate.cpp \
    aiendplay.cpp \
    scores.cpp \
    beziquematch.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=



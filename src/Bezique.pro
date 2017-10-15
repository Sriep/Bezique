QT += qml quick
QT += widgets
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

DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    deploy/buildLinux_bezique.sh \
    deploy/buildLinux_bezique.sh~ \
    deploy/BeziqueIcon.png \
    deploy/BeziqueIcon57x57.png \
    deploy/BeziqueIcon180x120.png \
    deploy/BeziqueIcon500x1024.png \
    deploy/BeziqueIcon512.png \
    deploy/BeziqueIcon512x512.png \
    deploy/BeziqueIcon1280x720.png \
    deploy/screen_endgame.png \
    deploy/screen1.png \
    deploy/screen3.png \
    deploy/screen4.png \
    deploy/BeziqueIcon.svg \
    deploy/BeziqueIcon57x57.svg \
    deploy/BeziqueIcon180x120.svg \
    deploy/BeziqueIcon500x1024.svg \
    deploy/BeziqueIcon512.svg \
    deploy/BeziqueIcon512x512.svg \
    deploy/BeziqueIcon1280x720.svg \
    deploy/bezique.iss \
    deploy/buildWin_bezique.bat \
    deploy/InfoAfterWin.txt \
    deploy/InfoBeforeWin.txt \
    deploy/LicenseFileWin.txt \
    release/bezique.png \
    release/bezique.desktop \
    ios/BeziqueIcon57x57.png \
    ios/BeziqueIcon67.png \
    ios/BeziqueIcon120.png \
    ios/BeziqueIcon152.png \
    ios/BeziqueIcon167.png \
    ios/BeziqueIcon180x120.png \
    ios/BeziqueIcon500x1024.png \
    ios/BeziqueIcon512.png \
    ios/BeziqueIcon512x512.png \
    ios/BeziqueIcon1024.png \
    ios/BeziqueIcon1280x720.png \
    ios/Info.plist

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ios {
    ios_icon.files = $$files($$PWD/ios/BeziqueIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
    QMAKE_INFO_PLIST = ios/Info.plist
}

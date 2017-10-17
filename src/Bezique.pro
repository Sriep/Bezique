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
    beziquematch.h \
    aiendplay.h \
    aievaluate.h \
    beziqueDeck.h \
    beziquehand.h \
    beziquematch.h \
    card.h \
    gamedata.h \
    gamestate.h \
    player.h \
    scores.h \
    test.h \
    unseencards.h
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
    beziquematch.cpp \
    aiendplay.cpp \
    aievaluate.cpp \
    aiplayer_copy.cpp \
    beziqueDeck.cpp \
    beziquehand.cpp \
    beziquematch.cpp \
    card.cpp \
    gamedata.cpp \
    gamedataBad.cpp \
    gamestate.cpp \
    main.cpp \
    player.cpp \
    scores.cpp \
    test.cpp \
    unseencards.cpp

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
    ios/BeziqueIcon67.png \
    ios/Info.plist \
    ios/Images.xcassets/AppIcon.appiconset/Contents.json \
    ios/Images.xcassets/AppIcon.appiconset/Icon-24@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-27.5@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-29@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-29@3x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-40@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-44@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-86@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-98@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-20x20@1x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-20x20@2x-1.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-20x20@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-20x20@3x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-29x29@1x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-29x29@2x-1.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-29x29@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-29x29@3x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-40x40@1x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-40x40@2x-1.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-40x40@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-40x40@3x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-60x60@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-60x60@3x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-76x76@1x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-76x76@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/Icon-App-83.5x83.5@2x.png \
    ios/Images.xcassets/AppIcon.appiconset/ItunesArtwork@2x.png

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

ios {
    #ios_icon.files = $$files($$PWD/ios/BeziqueIcon*.png)
    #QMAKE_BUNDLE_DATA += ios_icon
    #ios_icon2.files = $$files($$PWD/ios/Icon-*.png)
    #QMAKE_BUNDLE_DATA += ios_icon2
    #QMAKE_INFO_PLIST = ios/Info.plist
    QMAKE_ASSET_CATALOGS = $$PWD/ios/Images.xcassets
    QMAKE_ASSET_CATALOGS_APP_ICON = "AppIcon"
    PRODUCT_BUNDLE_IDENTIFIER = com.sriep.bezique
    #QMAKE_ASSET_CATALOGS = $$PWD/ios
    #QMAKE_ASSET_CATALOGS_APP_ICON = "BeziqueIcon1024.png"
}

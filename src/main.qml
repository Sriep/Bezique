import QtQuick 2.2
import "content"
import Qt.labs.settings 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
//import Qt.labs.platform 1.0
ApplicationWindow {
    id: appwin
    visible: true
    width: 800
    height: 1280
    property string playerName: "player"//userSettings.playerName
    property string aiName: "AI"//userSettings.aiName
    property int playerScore: 0//userSettings.playerScore
    property int aiScore : 0//userSettings.aiScore
    property int gameTarget : 1000
    property int toolbarHeight : toolbar.height
/*
    UseInfoBox {
        id: useInfoBox
        //buttons: MessageDialog.Ok
    }
*/
    ToolBar {
        id: toolbar
        width: parent.width
        RowLayout {
            spacing: 20
            anchors.fill: parent
            Label {
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
                text: getTitleBar()
            }

            ToolButton {
                id: drawerButton
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source:  "qrc:///content/gfx/menu@4x.png"
                }
                onClicked: optionsMenu.open()
                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight
                    /*
                    MenuItem {
                        text: qsTr("Main menu")
                        onTriggered: {
                            mainDisplay.currentIndex = 0;
                            console.log("New Game");
                        }
                    }
                    */
                    //MenuItem {
                    //    text: "About"
                    //    onTriggered: console.log("About");
                    //}

                    MenuItem {
                        text: qsTr("Quit")
                        onTriggered: Qt.quit();
                    }
                } //Menu
            } //ToolButton
        } //RowLayour
    } //ToolBar

    StackLayout {
        id: mainDisplay
        height: parent.height
        width: parent.width
        y: toolbar.height
        //property int ttoolbarHeight : toolbar.height
        //Flickable {
            //id: flickOptions
            //property int changedGameTarget: appwin.gameTarget
            ColumnLayout {
                id: selectButtons
                property int changedGameTarget: appwin.gameTarget
                RowLayout {
                    Label {
                        text: "Player name"
                    }
                    TextField {
                        id: userNameTextField
                        text: userSettings.playerName
                        cursorVisible: true
                        selectByMouse: true
                        onEditingFinished: {
                            appwin.playerName = userNameTextField.text;
                            console.log("new player name 1 ", appwin.playerName);
                            console.log("new player name 2 ", userSettings.playerName);
                        }
                    }
                }
                RowLayout {
                    Label {
                        text: "Opponent name"
                    }
                    TextField {
                        id: aiNameTextField
                        text: userSettings.aiName
                        cursorVisible: true
                        selectByMouse: true
                        onEditingFinished: {
                            appwin.aiName = aiNameTextField.text;
                            console.log("new player name a ", appwin.aiName);
                            console.log("new player name b ", userSettings.aiName);
                        }
                    }
                }
                RowLayout {
                    Button {
                        text: "Continue"
                        onClicked: {
                            gameBoard.continueGame();
                            mainDisplay.currentIndex = 1;
                            console.log("Contnue");
      /*
                            if (appwin.gameTarget === selectButtons.changedGameTarget) {
                                gameBoard.continueGame();
                                mainDisplay.currentIndex = 1;
                                console.log("Continue");
                            } else {
                                useInfoBox.text = qsTr("Continue disable")
                                useInfoBox.informativeText  = qsTr("You can only "
                                    + "start a new game after changing the winnint threshold.")
                                useInfoBox.detailedText  = qsTr("To continue "
                                    + "change the winning thresold back to "
                                    + "its original value of ") + appwin.gameTarget
                                useInfoBox.visible = true;
                            }*/
                        }
                    }
                    Button {
                        x:20
                        text: "New"
                        onClicked: {
                            appwin.gameTarget = selectButtons.changedGameTarget
                            console.log("new game target", appwin.gameTarget)
                            appwin.playerScore = 0
                            appwin.aiScore = 0
                            gameBoard.newGame();
                            mainDisplay.currentIndex = 1;
                            console.log("New");
                        }
                    }
                    Button {
                        x:20
                        text: qsTr("Exit")
                        onClicked: Qt.quit();
                    }
                }
                /*
                RowLayout {
                    id: rowSetWinningThreshold
                    //visible: false
                    Label {
                        text: qsTr("Winning Threshold")
                    }
                    TextField {
                        id: setWinningThreshold
                        text: appwin.gameTarget
                        validator: IntValidator{bottom: 10; top: 2000;}
                        cursorVisible: true
                        selectByMouse: true
                        //visible: false
                        onEditingFinished: {
                            if (acceptableInput) {
                                flickOptions.changedGameTarget = parseInt(text);
                                console.log("new win threshold", flickOptions.changedGameTarget);
                                //appwin.gameTarget = parseInt(text);
                            }
                        }
                    }
                }
            }*/
        }
        BeziqueView {
            id: gameBoard
            property string bottomName: bottomName
            property int topName: bottomName
            property string bottomGamesWon: bottomGamesWon
            property int topGamesWon: topGamesWon
            property bool restartGame: restartGame = false
            //property int toolbarHeight : mainDisplay.ttoolbarHeight
         }

        MessageDialog {
            id: finishedGame
            icon: StandardIcon.Question
            title: qsTr("Game over")
            informativeText: qsTr("Select yes to start another game and 'no' to quit")
            visible: false
            standardButtons: StandardButton.Yes | StandardButton.No
            onYes: {
                gameBoard.newGame();
                visible = false;
                mainDisplay.currentIndex = 1;
            }
            onNo: {
               Qt.quit();;
            }
        }

        Rectangle {
            color: "blue"
        }

    } //StackLayout

    Settings {
        category: "display"
        property alias width: appwin.width
        property alias height: appwin.height
    }

    Settings {
        id: userSettings
        category: "user"
        property alias playerName: appwin.playerName
        property alias aiName: appwin.aiName
        property alias playerScore: appwin.playerScore
        property alias aiScore : appwin.aiScore
        property alias gameTarget : appwin.gameTarget
    }
/*
    Settings {
        id: gameData
        category: "data"
        property string gameData: ""
    }
*/
    function getTitleBar () {
        var titleBar = qsTr("Bezique. ")
                      + appwin.playerName
                      + ": " + appwin.playerScore
                      + "  " + appwin.aiName
                      + ": " + appwin.aiScore;
        return titleBar;
    }

}

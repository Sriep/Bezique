import QtQuick 2.2
import "content"
import Qt.labs.settings 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1

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

    ToolBar {
        id: toolbar
        //Material.foreground: "white"
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

                    MenuItem {
                        text: "Main menu"
                        onTriggered: {
                            mainDisplay.currentIndex = 0;
                            console.log("New Game");
                        }
                    }
                    //MenuItem {
                    //    text: "About"
                    //    onTriggered: console.log("About");
                    //}
                    MenuItem {
                        text: "Quit"
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
        property int ttoolbarHeight : toolbar.height
        ColumnLayout {
            id: selectButtons

            RowLayout {
                Label {
                    text: "Player name" + "toobar height" + ttoolbarHeight
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
                    }
                }
                Button {
                    x:20
                    text: "New"
                    onClicked: {
                        appwin.playerScore = 0
                        appwin.aiScore = 0
                        gameBoard.newGame();
                        mainDisplay.currentIndex = 1;
                        console.log("New");
                    }
                }
                Button {
                    x:20
                    text: "Exit"
                    onClicked: Qt.quit();
                }
            }


        }

        BeziqueView {
            id: gameBoard
            property string bottomName: bottomName
            property int topName: bottomName
            property string bottomGamesWon: bottomGamesWon
            property int topGamesWon: topGamesWon
            property bool restartGame: restartGame = false
            property int toolbarHeight : mainDisplay.ttoolbarHeight
         }

        MessageDialog {
            id: finishedGame
            icon: StandardIcon.Question
            title: "Game over"
            informativeText: "Select yes to start another game and 'no' to quit"
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
        var titleBar = "Bezique. "
                      + appwin.playerName
                      + ": " + appwin.playerScore
                      + "  " + appwin.aiName
                      + ": " + appwin.aiScore;
        return titleBar;
    }

}

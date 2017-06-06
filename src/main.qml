import QtQuick 2.2
import QtQuick.Controls 1.2
import "content"
import Qt.labs.settings 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: appwin
    visible: true
    width: 800
    height: 1280
    title: qsTr("Bezique " + stackView.depth);
/*
    ColumnLayout {
        id: selectButtons
        //anchors.centerIn: horizontalCenter
        Button {
            anchors.centerIn: horizontalCenter
            text: "Contnue"
            onClicked: stackView.push(Page2)
        }
        Button {
            text: "New"
            onClicked: model.revert()
        }
        Button {
            text: "Options"
            //onClicked: model.revert()
        }
        Button {
            text: "Exit"
            onClicked: Qt.quit();
        }
    }
*/
    Page2 {
        id: gameBoard
         property string bottomName: bottomName
         property int topName: bottomName
         property string bottomGamesWon: bottomGamesWon
         property int topGamesWon: topGamesWon
         property bool restartGame: restartGame = false
     }



    StackView {
        id: stackView
        anchors.fill: parent
        // Implements back key navigation
        focus: true
        Keys.onReleased: if (event.key === Qt.Key_Back && stackView.depth > 1) {
                             stackView.pop();
                             event.accepted = true;
                         }

        initialItem :  gameBoard
    }

    Settings {
        property alias width: appwin.width
        property alias height: appwin.height
    }

}

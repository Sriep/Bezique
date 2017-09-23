import QtQuick 2.4
import QtQml 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Bezique 1.0

RowLayout {
    height: root.cardHeight
    spacing: root.hRowSpacing
    Rectangle {
        color: root.backColor;
        width: root.cardWidth; height: root.cardHeight;
        Image {
            width: root.cardWidth * 0.95;
            height: root.cardHeight * 0.95;
            source: gameData.stockImage
            visible: gameData.stockImage !== root.emptyImage
        }
    }
    Rectangle {
        color: root.backColor;
        width: root.cardWidth; height: root.cardHeight;
        Image {
            width: root.cardWidth * 0.95;
            height: root.cardHeight * 0.95;
            source: gameData.faceCard.image
            visible: gameData.faceCard.image !== root.emptyImage
        }
    }

    StatsColumn {
        Layout.alignment: Qt.AlignTop
        width: root.cardWidth; height: root.cardHeight;
    }

    Rectangle {
        id: statusText
        color: root.backColor;
        width: root.cardWidth*2.8; height: root.cardHeight;

        Text {
            horizontalAlignment:  Text.AlignHCenter
            font.family: "Helvetica"
            font.pointSize: 20
            color: "white"
            text: gameData.statusMessage
                  //+ "\n H: " + root.cardHeight + " W: " + root.cardWidth
                  //  + " TBH: " + toolbar.height
        }
    }

    TrickCards {
        height: root.cardHeight;
    } //Row
}

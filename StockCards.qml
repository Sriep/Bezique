import QtQuick 2.4
import QtQml 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Bezique 1.0

RowLayout {
    spacing: root.vRowSpacing
    Rectangle {
        color: root.backColor;
        width: root.cardWidth; height: root.cardHeight;
        Image {  source: gameData.stockImage }
    }
    Rectangle {
        color: root.backColor;
        width: root.cardWidth; height: root.cardHeight;
        Image { source: gameData.faceCard.image }
    }

    StatsColumn {
        Layout.alignment: Qt.AlignTop
        //color: root.backColor;
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
        }
    }

    TrickCards {
       // anchors.centerIn: parent
    } //Row
}

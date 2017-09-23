import QtQuick 2.0
import Bezique 1.0

Rectangle {
    id: cardImage
    color: root.backColor;
    width: root.cardWidth; height: root.cardHeight;
    property Card thisCard
    property string image: "content/gfx/b1fv.bmp"
    property bool canMeld: false;
    //property alias canMeld: thisCard.canMeld
    property bool canPlay: true;
    property int rowPos: 0
    property bool melded: false
    border.width: {
        if (thisCard.image === root.emptyImage)
            return 0;
        if (gameData.waitingForCard
            || (gameData.humanMelding && cardImage.canMeld) )
            return 1;
        return 0;
    }
    border.color: {
        if (cardImage.image !== root.emptyImage) {
            if (gameData.humanMelding)  {
                if (cardImage.canMeld)
                    return "red";
                else
                    return "green";
            } else if (gameData.waitingForCard) {
                if (cardImage.canPlay)
                    return "blue";
                else
                    return "green";
            }
        }
        return "green"
    }

    Image {
        width: root.cardWidth * 0.95;
        height: root.cardHeight * 0.95;
        //source: cardImage.image
        visible: thisCard.image !== root.emptyImage//thisCard.isVisible
        source: thisCard.image
    }
    MouseArea {
        id: cimageMouseArea
        anchors.fill: parent;
        signal playedCardIndex(int rowPos);
        onClicked: {
            console.log("gameData.waitingForCard",gameData.waitingForCard);
            console.log("gameData.humanMelding",gameData.humanMelding);
            if (gameData.waitingForCard && cardImage.canPlay) {
                gameData.waitingForCard = false;
                gameData.cardPlayed(rowPos, melded);                
            } else if (gameData.humanMelding && cardImage.canMeld) {
                gameData.humanMeld(canMeld, rowPos, melded);
                gameData.humanMelding = false;
            }
        }
    }    
}


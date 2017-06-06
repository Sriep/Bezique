import QtQuick 2.0

Rectangle {
    id: cardImage
    color: root.backColor;
    width: root.cardWidth; height: root.cardHeight;
    property string image: "content/gfx/b1fv.bmp"
    property bool canMeld: false;
    property bool canPlay: true;
    property int rowPos: 0
    property bool melded: false
    border.width: {
        if (image === root.emptyImage)
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

    Image { source: cardImage.image }
    MouseArea {
        id: cimageMouseArea
        anchors.fill: parent;
        signal playedCardIndex(int rowPos);
        onClicked: {
            console.log("gameData.waitingForCard",gameData.waitingForCard);
            console.log("gameData.humanMelding",gameData.humanMelding);
            if (gameData.waitingForCard) {
                gameData.waitingForCard = false;
                gameData.cardPlayed(rowPos, melded);                
            } else if (gameData.humanMelding) {
                gameData.humanMeld(canMeld, rowPos, melded);
                gameData.humanMelding = false;
            }
        }
    }
}

/*
Flipable {
    property alias source: frontImage.source
    property bool flipped: true
    width: front.width; height: front.height
    front: Image { id: frontImage }
    back: Image { source: "content/gfx/b1fv.bmp" }
    state: "back"
    MouseArea {
        anchors.fill: parent;
        onClicked: {
            container.flipped = !container.flipped
        }
    }
}
*/

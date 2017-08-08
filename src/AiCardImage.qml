import QtQuick 2.0
import Bezique 1.0

Rectangle {
    id: aiCardImage
    color: root.backColor;
    width: root.cardWidth; height: root.cardHeight;
    property Card thisCard
    //visible: thisCard.isVisible
    Image {
        width: root.cardWidth * 0.95;
        height: root.cardHeight * 0.95;
        visible: thisCard.image !== root.emptyImage
        source: thisCard.image
    }
}

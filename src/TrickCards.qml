import QtQuick 2.4
import QtQml 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Bezique 1.0

RowLayout {
  spacing: root.hRowSpacing
  Rectangle {
    color: root.backColor;
    width: root.cardWidth; height: root.cardHeight;
    //Image {  source: "content/gfx/up.png" }
    //Image {  source: aiHand.aiPlayedCardImage }
    Image {
        width: root.cardWidth * 0.95;
        height: root.cardHeight * 0.95;
        source: gameData.aisCard.image
        visible: gameData.aisCard.image !== root.emptyImage
    }
  }
  Rectangle {
    color: root.backColor;
    width: root.cardWidth; height: root.cardHeight;
    //Image { source: "content/gfx/down.png" }
    //Image {  source: humanHand.humanPlayedCardImage }
    Image {
        width: root.cardWidth * 0.95;
        height: root.cardHeight * 0.95;
        source: gameData.humansCard.image
        visible: gameData.humansCard.image !== root.emptyImage
    }
  }
}

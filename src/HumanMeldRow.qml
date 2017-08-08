import QtQuick 2.4
import QtQml 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Bezique 1.0

RowLayout {
  spacing: root.hRowSpacing
  //anchors.bottom: humanHidden.top
  width: root.cardWidth; height: root.cardHeight;
  CardImage { thisCard: melded1 ;image: melded1.image; rowPos: 0; melded: true;
      canMeld: melded1.canMeld; canPlay: hidden1.canPlay  }
  CardImage { thisCard: melded2 ;image: melded2.image; rowPos: 1; melded: true;
      canMeld: melded2.canMeld; canPlay: hidden2.canPlay }
  CardImage { thisCard: melded3 ;image: melded3.image; rowPos: 2; melded: true;
      canMeld: melded3.canMeld; canPlay: hidden3.canPlay }
  CardImage { thisCard: melded4 ;image: melded4.image; rowPos: 3; melded: true;
      canMeld: melded4.canMeld; canPlay: hidden4.canPlay }
  CardImage { thisCard: melded5 ;image: melded5.image; rowPos: 4; melded: true;
      canMeld: melded5.canMeld; canPlay: hidden5.canPlay }
  CardImage { thisCard: melded6 ;image: melded6.image; rowPos: 5; melded: true;
      canMeld: melded6.canMeld; canPlay: hidden6.canPlay }
  CardImage { thisCard: melded7 ;image: melded7.image; rowPos: 6; melded: true;
      canMeld: melded7.canMeld; canPlay: hidden7.canPlay }
  CardImage { thisCard: melded8 ;image: melded8.image; rowPos: 7; melded: true;
      canMeld: melded8.canMeld; canPlay: hidden8.canPlay }
}

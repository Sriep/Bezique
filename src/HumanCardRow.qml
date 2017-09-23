import QtQuick 2.1
import QtQuick.Layouts 1.3

RowLayout {
    //id: humanHidden
    spacing: root.hRowSpacing
    //anchors.bottom: parent.bottom
    width: root.cardWidth; height: root.cardHeight;
    CardImage {  thisCard: hidden1 ;image: hidden1.image; rowPos: 0;
        canMeld: hidden1.canMeld; canPlay: hidden1.canPlay }
    CardImage { thisCard: hidden2 ;image: hidden2.image; rowPos: 1;
        canMeld: hidden2.canMeld; canPlay: hidden2.canPlay }
    CardImage { thisCard: hidden3 ;image: hidden3.image; rowPos: 2;
        canMeld: hidden3.canMeld; canPlay: hidden3.canPlay }
    CardImage { thisCard: hidden4 ;image: hidden4.image; rowPos: 3;
        canMeld: hidden4.canMeld; canPlay: hidden4.canPlay }
    CardImage { thisCard: hidden5 ;image: hidden5.image; rowPos: 4;
        canMeld: hidden5.canMeld; canPlay: hidden5.canPlay }
    CardImage { thisCard: hidden6 ;image: hidden6.image; rowPos: 5;
        canMeld: hidden6.canMeld; canPlay: hidden6.canPlay }
    CardImage { thisCard: hidden7 ;image: hidden7.image; rowPos: 6;
        canMeld: hidden7.canMeld; canPlay: hidden7.canPlay }
    CardImage { thisCard: hidden8 ;image: hidden8.image; rowPos: 7;
        canMeld: hidden8.canMeld; canPlay: hidden8.canPlay }
} //Row

import QtQuick 2.1
import QtQuick.Layouts 1.3

RowLayout {
    //id: humanHidden
    spacing: root.vRowSpacing
    //anchors.bottom: parent.bottom
    width: root.cardWidth; height: root.cardHeight;
    CardImage { image: hidden1.image; rowPos: 0;
        canMeld: hidden1.canMeld; canPlay: hidden1.canPlay }
    CardImage { image: hidden2.image; rowPos: 1;
        canMeld: hidden2.canMeld; canPlay: hidden2.canPlay }
    CardImage { image: hidden3.image; rowPos: 2;
        canMeld: hidden3.canMeld; canPlay: hidden3.canPlay }
    CardImage { image: hidden4.image; rowPos: 3;
        canMeld: hidden4.canMeld; canPlay: hidden4.canPlay }
    CardImage { image: hidden5.image; rowPos: 4;
        canMeld: hidden5.canMeld; canPlay: hidden5.canPlay }
    CardImage { image: hidden6.image; rowPos: 5;
        canMeld: hidden6.canMeld; canPlay: hidden6.canPlay }
    CardImage { image: hidden7.image; rowPos: 6;
        canMeld: hidden7.canMeld; canPlay: hidden7.canPlay }
    CardImage { image: hidden8.image; rowPos: 7;
        canMeld: hidden8.canMeld; canPlay: hidden8.canPlay }
} //Row

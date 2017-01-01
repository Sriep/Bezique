import QtQuick 2.4
import QtQml 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Bezique 1.0

ColumnLayout {

  Rectangle {
      //anchors.top: parent.top
      Layout.alignment: Qt.AlignTop
      color: root.backColor;
      width: parent.width; height: root.cardHeight/3;
      Text {
         color: "white"
         text :  gameData.aiPlayer.score.toString()
      }
  }

  RowLayout {
      Layout.alignment: Qt.AlignVCenter
      width: parent.width; height: root.cardHeight/3;
      Rectangle {
          color: root.backColor;
          width: 10; height: root.cardHeight/3;
          Text{
            color: "white"
            text : gameData.cardsInStock.toString()
          }
      }

      Rectangle {
          color: root.backColor;
          width: 10; height: root.cardHeight/3;
          Text {
             color: (gameData.trumps === 0 || gameData.trumps === 2)
                    ? "red" : "black";
             text : {
                 switch (gameData.trumps) {
                     case 0:
                         return "\u2666";
                     case 1:
                         return "\u2663";
                     case 2:
                         return "\u2665";
                     case 3:
                         return "\u2660";
                     default:
                         return "";
                 } // switch
             } // text
          } // Text
      } // Rectangle
  }

  Rectangle {
      Layout.alignment: Qt.AlignBottom
      color: root.backColor;
      width: parent.width; height: root.cardHeight/3;
      Text {
          color: "white"
          text : gameData.humanPlayer.score.toString()
      }
  }
}

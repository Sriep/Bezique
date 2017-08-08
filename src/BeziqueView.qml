import QtQuick 2.4
import QtQml 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.0
import Qt.labs.settings 1.0
import Bezique 1.0
import QtQuick.Window 2.2


Item {
    id: root
    visible: true
    width: parent.width; height: parent.height;

    property int vRowSpacing: 0
    property int hRowSpacing: 0
    property int cardPicWidth: 46
    property int cardPicHeight: 66
    function calCardPixelSize(cardWidth, cardHeight) {
        var maxWidth = (Screen.desktopAvailableWidth - 7*hRowSpacing)/8;
        maxWidth = cardWidth < maxWidth ? cardWidth : maxWidth;

        var maxHeight = (Screen.desktopAvailableHeight
                         - 4*vRowSpacing - toolbar.height)/5;
        maxHeight = cardHeight < maxHeight ? cardHeight : maxHeight;

        var pixelSizeWidth = maxWidth / cardWidth;
        var pixelSizeHeight = maxHeight / cardHeight;
        var rtv = pixelSizeWidth < pixelSizeHeight ? pixelSizeWidth : pixelSizeHeight;
        console.log("calCardPixelSize",rtv);
        console.log("cardWidth",rtv * cardWidth);
        console.log("cardHeight",rtv *  cardHeight);
        return pixelSizeWidth < pixelSizeHeight ? pixelSizeWidth : pixelSizeHeight;
    }
    property int cardWidth: Math.floor(calCardPixelSize(cardPicWidth, cardPicHeight) * cardPicWidth)
    // Multiplying cardHeight by 0.95 seems to work on my android.
    // Need to check this out on ohter resolutions.
    property int cardHeight: Math.floor(calCardPixelSize(cardPicWidth, cardPicHeight) * cardPicHeight * 0.95)


    property string emptyImage: "content/gfx/onePixelGreen.png"
    property string backImage: "content/gfx/tinydeck/back111.gif"
    property string backColor: "green"

    property string bottomName: match.bottomName
    property string topName: match.bottomName
    property int bottomGamesWon: match.bottomGamesWon
    property int topGamesWon: match.topGamesWon
    property bool saveAvaliable: match.saveAvaliable
    property bool restartGame: restartGame = false
    property int toolbarHeight :0

    signal continueGame()
    signal newGame()

    Rectangle {
        width: parent.width; height: parent.height;
        color: root.backColor;
        MouseArea {
            id: gameArea
            anchors.fill: parent;
            onClicked: {
                console.log("gameData.drawCard",gameData.drawCard);
                console.log("gameData.isEndgame",gameData.isEndgame);
                console.log("gameData.statusMessage",gameData.statusMessage);
                console.log("gameData.waitNextTrick",gameData.waitNextTrick);
                console.log("Screen.desktopAvailableHeight",Screen.desktopAvailableHeight);
                console.log("Screen.desktopAvailableWidth",Screen.desktopAvailableWidth);
                console.log("aiMelded1",aiMelded1);
                var temp = aiMelded1;
                if (!gameData.isGameInProgress())
                    return;

                if (gameData.drawCard) {
                    gameData.drawCard = false;
                    gameData.statusMessage = ""
                    gameData.finishTrick();
                } else if (gameData.waitNextTrick
                           && gameData.isEndgame) {
                    gameData.waitNextTrick = false;
                    gameData.statusMessage = ""
                    gameData.scoreEndTrick();
                }
            }
        }
    }

    BeziqueMatch {
        id: match
        bottomName: "human"
        topName: "ai"
        bottomGamesWon: 0
        topGamesWon: 0
        saveAvaliable: false

        gameData: GameData {
            id: gameData
            faceCard: Card { id: faceCard; }
            onHumansCardChanged: {
            }

            aisCard: Card {}
            humansCard: Card {}

            property bool aiPlayedCard: aisCard.image !== root.emptyImage
            property string statusMessage: "Play"
            property bool waitingForCard: false
            property bool humanMelding: false
            property bool drawCard: false
            property string stockImage: root.backImage
            property bool isEndgame: false;
            property bool waitNextTrick: false

            onWaitingForCard: {
                waitingForCard = true;
                statusMessage = "Play\n" + Screen.desktopAvailableHeight  + " " + Screen.desktopAvailableWidth;
            }

            onFollowedToTrick: {
                if (isEndgame) {
                    waitNextTrick = true;
                    statusMessage = "Next trick";
                }
            }

            onGameOver : {
                console.log("GameOver", playerWon);
                if (playerWon) {
                    appwin.playerScore++;
                    finishedGame.text = "Congratulations you won the game. Play again?"
                } else {
                    finishedGame.text = "Bad luck you lost the game. Play again?"
                    appwin.aiScore++;
                }
                finishedGame.visible = true;
                mainDisplay.currentIndex = 2;
                //gameData.startNewGame();
            }

            Connections {
                target: root
                onPlayingChanged: {
                    console.log("onPlayingChanged");
                    if (playing)
                        gameData.startNewGame();
                }
            }

            Connections {
                target: root
                onContinueGame: {
                    console.log("onPlayingChanged");
                    gameData.continueGame();
                }
            }

            Connections {
                target: root
                onNewGame: {
                    gameData.startNewGame();
                    console.log("onNewGame");
                }
            }

            onStartEndgame: {
               stockImage =  root.emptyImage;
               isEndgame = true;
            }

            onHandsDealt: {
                stockImage = root.backImage;
                isEndgame = false;
            }

            onWaitingForMeld: {
                humanMelding = true;
                waitingForCard = false;
                statusMessage = "Meld";
            }
            onMelded: {
                humanMelding = false;
                statusMessage = ""
            }
            onDrawing: {
                drawCard = true;
                statusMessage = "Draw";
            }

            humanPlayer: Player {
                id: humanPlayer
                ai: false
                hand: BeziqueHand {
                    id: humanHand
                    //property string humanPlayedCardImage: "content/gfx/down.png"
                    cards: [
                        Card { id: card1 },
                        Card { id: card2 },
                        Card { id: card3 },
                        Card { id: card4 },
                        Card { id: card5 },
                        Card { id: card6 },
                        Card { id: card7 },
                        Card { id: card8 }
                    ] //card
                    onEnginPlayedCard: {
                        //humanHand.humanPlayedCardImage = cards[index].image;
                        //data[index].image =  "content/gfx/onePixel.png"
                    }
                    hiddedCards: [
                        Card { id: hidden1 },
                        Card { id: hidden2 },
                        Card { id: hidden3 },
                        Card { id: hidden4 },
                        Card { id: hidden5 },
                        Card { id: hidden6 },
                        Card { id: hidden7 },
                        Card { id: hidden8 }
                    ] //card
                    meldedCards: [
                        Card { id: melded1 },
                        Card { id: melded2 },
                        Card { id: melded3 },
                        Card { id: melded4 },
                        Card { id: melded5 },
                        Card { id: melded6 },
                        Card { id: melded7 },
                        Card { id: melded8 }
                    ] //card

                } // playerHand: BeziqueHand
            } // humanPlayer: Player

           aiPlayer: Player {
               ai: true
               //score: 0
               hand: BeziqueHand {
                    id: aiHand
                    //property string aiPlayedCardImage: "content/gfx/up.png"
                    cards: [
                        Card { id: aiCard1 },
                        Card { id: aiCard2 },
                        Card { id: aiCard3 },
                        Card { id: aiCard4 },
                        Card { id: aiCard5 },
                        Card { id: aiCard6 },
                        Card { id: aiCard7 },
                        Card { id: aiCard8 }
                    ] //cards
                    onEnginPlayedCard: {
                        //aiHand.aiPlayedCardImage = data[index].image;
                        //console.log("index", index);
                        //console.log("image", cards[index].imag);
                        //data[index].image =  "content/gfx/onePixel.png"
                    }
                    hiddedCards: [
                        Card { id: aiHidden1 },
                        Card { id: aiHidden2 },
                        Card { id: aiHidden3 },
                        Card { id: aiHidden4 },
                        Card { id: aiHidden5 },
                        Card { id: aiHidden6 },
                        Card { id: aiHidden7 },
                        Card { id: aiHidden8 }
                    ] //card
                   meldedCards: [
                        Card { id: aiMelded1 },
                        Card { id: aiMelded2 },
                        Card { id: aiMelded3 },
                        Card { id: aiMelded4 },
                        Card { id: aiMelded5 },
                        Card { id: aiMelded6 },
                        Card { id: aiMelded7 },
                        Card { id: aiMelded8 }
                    ] //card

                } // aiHand: BeziqueHand

           } // aiPlayer: Player


           onTrickFinished: {
               gameData.humansCard.image =  "content/gfx/down.png";
               gameData.aisCard.image =  "content/gfx/up.png";
           }
        } //GameData

        //Column {
        ColumnLayout    {
            spacing: root.vRowSpacing
            AiCardRow {
                id: aiHidden
                width: root.cardWidth; height: root.cardHeight;
            }

            AiMeldRow {
                width: root.cardWidth; height: root.cardHeight;
            } //Row

            StockCards {
                id: stock;
                width: root.cardWidth; height: root.cardHeight;
            } //Row

            HumanMeldRow {
                width: root.cardWidth; height: root.cardHeight;
            } //Row

            HumanCardRow {
                width: root.cardWidth; height: root.cardHeight;
                id: humanHidden
            }
        } //Column
    } // BeziqueMatch

} // Item



































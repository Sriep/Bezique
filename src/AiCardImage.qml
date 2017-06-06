import QtQuick 2.0

Rectangle {
    id: aiCardImage
    color: root.backColor;
    width: root.cardWidth; height: root.cardHeight;
    property string image: "content/gfx/b1fv.bmp"
    Image { source: aiCardImage.image }
}

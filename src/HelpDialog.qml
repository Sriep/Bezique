import QtQuick 2.2
import "content"
import Qt.labs.settings 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.1
import QtQuick.Controls.Universal 2.1
//import Qt.labs.platform 1.0
Dialog {
    id: helpDialog
    title:  "Bezique"
    contentItem : Item {
        Text {
            //width: aboutDialog.availableWidth
            width: parent.width
            text: qsTr("<html><h2>Easy Event App</h2><br>")
                  + qsTr("Easy event app displayes  custom event and conference information. Design your own free Event App. ")
                  + "<a href=\"http://www.easyeventapps.com\">www.easyeventapps.com</a><br>"
                  + qsTr("Version ") + eaContainer.appVersion() + "</html>\n"
            wrapMode: Text.WordWrap
            font.pixelSize: 12
        }
    }   
    
}

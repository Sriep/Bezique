import QtQuick 2.2
import "content"
import Qt.labs.settings 1.0
import QtQuick.Window 2.2
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

//import Qt.labs.platform 1.0
import QtQuick.Dialogs 1.2
MessageDialog {
    id: useInfoBox
    //onOkClicked: {
        //visible = false;
    //}
    icon: 	StandardIcon.Information
    standardButtons: StandardButton.Ok
}

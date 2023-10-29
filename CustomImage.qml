import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    property alias source: image.source
    property alias text: text.text
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.alignment: Qt.AlignCenter
    Image {
        id: image
        anchors.centerIn: parent
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        Text {
            id: text
            anchors.left: parent.left
            anchors.top: parent.top
        }
        Rectangle {
            color: Qt.rgba(1, 1, 1, 0.7)
            radius: 10
            border.width: 1
            border.color: "white"
            anchors.fill: parent
            anchors.margins: -10
            z: -1
        }
        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (image2.source !== "") {
                    fileDialog.fileUrl = image.source;
                    fileDialog.open();
                }
            }
        }
    }
    FileDialog {
        id: fileDialog
        fileMode: FileDialog.SaveFile
        defaultSuffix: "png"
        nameFilters: ["PNG (*.png)", "JPG (*.jpg)"]
        property string fileUrl: ""
        onAccepted: {
            //delete prefix "image://imageProvider/"
            var fileName = fileUrl.substring(22);
            imageController.saveImage(fileName, selectedFile);
        }
    }
}

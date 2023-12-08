import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
    property string nowSelectedFile: ""
    ColumnLayout {
        Layout.alignment: Qt.AlignCenter
        width: parent.width
        height: parent.height
        spacing: 40
        Rectangle {
            width: 0
            color: "transparent"
        }
        RowLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.preferredHeight: parent.width / 2.5
            spacing: 40
            Rectangle {
                width: 0
                color: "transparent"
            }
            CustomImage {
                id: image1
                text: "原图"
            }
            CustomImage {
                id: image2
                text: "Canny 边缘检测"
            }
            Rectangle {
                width: 0
                color: "transparent"
            }
        }
        RowLayout {
            Layout.alignment: Qt.AlignCenter
            spacing: 10
            Button {
                Layout.alignment: Qt.AlignCenter
                text: "打开图片并转换"
                onClicked: fileDialog.open()
            }
        }
    }
    FileDialog {
        id: fileDialog
        onAccepted: {
            image1.source = selectedFile;
            imageController.loadImage(selectedFile);
            imageController.cannyEdgeDetection(selectedFile);
            image2.source = "image://imageProvider/" + selectedFile + "cannyEdgeDetection";
        }
    }
}

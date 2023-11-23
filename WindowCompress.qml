import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs

Item {
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
                text: "JPEG压缩"
            }
            CustomImage {
                id: image3
                text: "JPEG2000压缩"
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
            Button {
                Layout.alignment: Qt.AlignCenter
                text: "重置"
                onClicked: {
                    image1.source = "";
                    image2.source = "";
                    image3.source = "";
                }
            }
        }
    }
    FileDialog {
        id: fileDialog
        onAccepted: {
            image1.source = selectedFile;
            imageController.loadImage(selectedFile);
            imageController.compressImageJPEG(selectedFile);
            image2.source = "image://imageProvider/" + selectedFile + "compressImageJPEG";
            imageController.compressImageJPEG2000(selectedFile);
            image3.source = "image://imageProvider/" + selectedFile + "compressImageJPEG2000";
        }
    }
}

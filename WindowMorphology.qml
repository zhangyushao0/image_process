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
                text: "处理后"
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
                text: "腐蚀"
                onClicked: {
                    imageController.erode(nowSelectedFile);
                    nowSelectedFile += "erode";
                    image2.source = "image://imageProvider/" + nowSelectedFile;
                }
            }
            Button {
                Layout.alignment: Qt.AlignCenter
                text: "膨胀"
                onClicked: {
                    imageController.dilate(nowSelectedFile);
                    nowSelectedFile += "dilate";
                    image2.source = "image://imageProvider/" + nowSelectedFile;
                }
            }
        }
    }
    FileDialog {
        id: fileDialog
        onAccepted: {
            image1.source = selectedFile;
            imageController.loadImage(selectedFile);
            imageController.removeSmallComponents(selectedFile);
            image2.source = "image://imageProvider/" + selectedFile + "removeSmallComponents";
            nowSelectedFile = selectedFile + "removeSmallComponents";
        }
    }
}

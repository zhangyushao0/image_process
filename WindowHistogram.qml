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
                text: qsTr("原图")
            }
            CustomImage {
                id: image2
                text: qsTr("直方图")
            }
            Rectangle {
                width: 0
                color: "transparent"
            }
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
                id: image3
                text: qsTr("直方图均衡化图像")
            }
            CustomImage {
                id: image4
                text: qsTr("CLACHE算法图像")
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
                    image4.source = "";
                }
            }
        }
    }
    FileDialog {
        id: fileDialog
        onAccepted: {
            image1.source = selectedFile;
            imageController.loadImage(selectedFile);
            imageController.displayHistogram(selectedFile);
            imageController.histogramEqualization(selectedFile);
            imageController.applyCLAHE(selectedFile);
            image2.source = "image://imageProvider/" + selectedFile + "displayHistogram";
            image3.source = "image://imageProvider/" + selectedFile + "histogramEqualization";
            image4.source = "image://imageProvider/" + selectedFile + "applyCLAHE";
        }
    }
}

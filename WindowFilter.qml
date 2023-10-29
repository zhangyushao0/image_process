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
                text: "高斯噪声"
            }
            CustomImage {
                id: image3
                text: "中值处理"
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
                id: image4
                text: "均值处理"
            }
            CustomImage {
                id: image5
                text: "自适应中值处理"
            }
            CustomImage {
                id: image6
                text: "Nonlocal means处理"
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
                    image5.source = "";
                    image6.source = "";
                }
            }
        }
    }
    FileDialog {
        id: fileDialog
        onAccepted: {
            image1.source = selectedFile;
            imageController.loadImage(selectedFile);
            imageController.addGaussianNoise(selectedFile);
            image2.source = "image://imageProvider/" + selectedFile + "addGaussianNoise";
            imageController.medianBlurFilter(selectedFile + "addGaussianNoise");
            image3.source = "image://imageProvider/" + selectedFile + "addGaussianNoise" + "medianBlurFilter";
            imageController.meanBlurFilter(selectedFile + "addGaussianNoise");
            image4.source = "image://imageProvider/" + selectedFile + "addGaussianNoise" + "meanBlurFilter";
            // imageController.adaptiveMedianFilter(selectedFile + "addGaussianNoise");
            // image5.source = "image://imageProvider/" + selectedFile + "addGaussianNoise" + "adaptiveMedianFilter";
            imageController.nonLocalMeanFilter(selectedFile + "addGaussianNoise");
            image6.source = "image://imageProvider/" + selectedFile + "addGaussianNoise" + "nonLocalMeanFilter";
        }
    }
}

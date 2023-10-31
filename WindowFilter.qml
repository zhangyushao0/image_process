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
                text: "噪声"
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
                text: "添加高斯噪声"
                onClicked: fileDialog.open()
            }
            Button {
                Layout.alignment: Qt.AlignCenter
                text: "添加椒盐噪声"
                onClicked: fileDialog2.open()
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
            let readyImageURL = selectedFile + "addGaussianNoise";
            imageController.medianBlurFilter(readyImageURL);
            image3.source = "image://imageProvider/" + readyImageURL + "medianBlurFilter";
            imageController.meanBlurFilter(readyImageURL);
            image4.source = "image://imageProvider/" + readyImageURL + "meanBlurFilter";
            imageController.adaptiveMedianFilter(readyImageURL);
            image5.source = "image://imageProvider/" + readyImageURL + "adaptiveMedianFilter";
            imageController.nonLocalMeanFilter(readyImageURL);
            image6.source = "image://imageProvider/" + readyImageURL + "nonLocalMeanFilter";
        }
    }
    FileDialog {
        id: fileDialog2
        onAccepted: {
            image1.source = selectedFile;
            imageController.loadImage(selectedFile);
            imageController.addSaltAndPepperNoise(selectedFile);
            image2.source = "image://imageProvider/" + selectedFile + "addSaltAndPepperNoise";
            let readyImageURL = selectedFile + "addSaltAndPepperNoise";
            imageController.medianBlurFilter(readyImageURL);
            image3.source = "image://imageProvider/" + readyImageURL + "medianBlurFilter";
            imageController.meanBlurFilter(readyImageURL);
            image4.source = "image://imageProvider/" + readyImageURL + "meanBlurFilter";
            imageController.adaptiveMedianFilter(readyImageURL);
            image5.source = "image://imageProvider/" + readyImageURL + "adaptiveMedianFilter";
            imageController.nonLocalMeanFilter(readyImageURL);
            image6.source = "image://imageProvider/" + readyImageURL + "nonLocalMeanFilter";
        }
    }
}

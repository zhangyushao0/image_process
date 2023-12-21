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
                text: "直方图均衡后"
            }
            CustomImage {
                id: image3
                text: "二阶导数增强后"
            }
            CustomImage {
                id: image4
                text: "自适应中值滤波后"
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
            imageController.loadImage(selectedFile);
            image1.source = "image://imageProvider/" + selectedFile;
            imageController.histogramEqualization(selectedFile);
            image2.source = "image://imageProvider/" + selectedFile + "histogramEqualization";
            imageController.enhanceImageUsingSecondDerivative(selectedFile + "histogramEqualization");
            image3.source = "image://imageProvider/" + selectedFile + "histogramEqualization" + "enhanceImageUsingSecondDerivative";
            imageController.adaptiveMedianFilter(selectedFile + "histogramEqualization" + "enhanceImageUsingSecondDerivative");
            imageController.adaptiveMedianFilter(selectedFile + "histogramEqualization" + "enhanceImageUsingSecondDerivative" + "adaptiveMedianFilter");
            image4.source = "image://imageProvider/" + selectedFile + "histogramEqualization" + "enhanceImageUsingSecondDerivative" + "adaptiveMedianFilter" + "adaptiveMedianFilter";
        }
    }
}

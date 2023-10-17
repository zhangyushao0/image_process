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
            Image {
                id: image1
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                Text {
                    text: qsTr("原始图像")
                    anchors.left: parent.left
                    anchors.top: parent.top
                }
                fillMode: Image.PreserveAspectFit
                Rectangle {
                    color: Qt.rgba(1, 1, 1, 0.7)
                    radius: 10
                    border.width: 1
                    border.color: "white"
                    anchors.fill: parent
                    anchors.margins: -10
                    z: -1
                }
            }
            Image {
                id: image2
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                Text {
                    text: qsTr("直方图")
                    anchors.left: parent.left
                    anchors.top: parent.top
                }
                fillMode: Image.PreserveAspectFit
                Rectangle {
                    color: Qt.rgba(1, 1, 1, 0.7)
                    radius: 10
                    border.width: 1
                    border.color: "white"
                    anchors.fill: parent
                    anchors.margins: -10
                    z: -1
                }
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
            Image {
                id: image3
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                Text {
                    text: qsTr("直方均衡化后图像")
                    anchors.left: parent.left
                    anchors.top: parent.top
                }
                fillMode: Image.PreserveAspectFit
                Rectangle {
                    color: Qt.rgba(1, 1, 1, 0.7)
                    radius: 10
                    border.width: 1
                    border.color: "white"
                    anchors.fill: parent
                    anchors.margins: -10
                    z: -1
                }
            }
            Image {
                id: image4
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.alignment: Qt.AlignCenter
                Text {
                    text: qsTr("CLAHE算法图像")
                    anchors.left: parent.left
                    anchors.top: parent.top
                }
                fillMode: Image.PreserveAspectFit
                Rectangle {
                    color: Qt.rgba(1, 1, 1, 0.7)
                    radius: 10
                    border.width: 1
                    border.color: "white"
                    anchors.fill: parent
                    anchors.margins: -10
                    z: -1
                }
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
            imageController.displayHistogram(selectedFile);
            imageController.histogramEqualization(selectedFile);
            imageController.applyCLAHE(selectedFile);
            console.log("success");
            image2.source = "image://imageProvider/" + selectedFile + "displayHistogram";
            image3.source = "image://imageProvider/" + selectedFile + "histogramEqualization";
            image4.source = "image://imageProvider/" + selectedFile + "applyCLAHE";
        }
    }
}

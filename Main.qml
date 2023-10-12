import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts
ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title:stackView.currentItem.objectName
    menuBar: MenuBar {
           Menu {
               title: "图像变换"
               MenuItem {
                   text: "傅里叶变换"
                   onTriggered: stackView.replace(window_ft)
               }
               MenuItem {
                   text: "Window 2"
                   onTriggered: stackView.replace(window2)
               }
           }
       }
    StackView {
        id: stackView
        initialItem: window_ft
        anchors.fill: parent
    }
    WindowFT{
        id:window_ft
    }
}


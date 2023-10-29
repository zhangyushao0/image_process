import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("图像处理")
    menuBar: TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("傅里叶变换")
            width: implicitWidth
        }
        TabButton {
            text: qsTr("直方图均衡化")
            width: implicitWidth
        }
        TabButton {
            text: qsTr("滤波变换")
            width: implicitWidth
        }
    }
    SwipeView {
        id: view
        anchors.fill: parent
        currentIndex: bar.currentIndex
        Item {
            id: windowFT
            WindowFT {
                anchors.fill: parent
            }
        }
        Item {
            id: windowHistogram
            WindowHistogram {
                anchors.fill: parent
            }
        }
        Item {
            id: windowFilter
            WindowFilter {
                anchors.fill: parent
            }
        }
    }
}

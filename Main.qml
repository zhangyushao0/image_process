import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: stackView.currentItem.objectName
    menuBar: MenuBar {
        Menu {
            title: "图像变换"
            MenuItem {
                text: "傅里叶变换"
                onTriggered: stackView.replace(windowFtComponent)
            }
            MenuItem {
                text: "直方图"
                onTriggered: stackView.replace(windowHistogramComponent)
            }
        }
    }
    StackView {
        id: stackView
        initialItem: windowFtComponent
        anchors.fill: parent
    }
    Component {
        id: windowFtComponent
        WindowFT {
        }
    }

    Component {
        id: windowHistogramComponent
        WindowHistogram {
        }
    }
}

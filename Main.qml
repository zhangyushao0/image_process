import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("傅里叶变换")
    menuBar: MenuBar {
        Menu {
            title: "图像变换"
            MenuItem {
                text: "傅里叶变换"
                onTriggered: {
                    stackView.replace(windowFtComponent);
                    window.title = "傅里叶变换";
                }
            }
            MenuItem {
                text: "直方图"
                onTriggered: {
                    stackView.replace(windowHistogramComponent);
                    window.title = "直方图";
                }
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

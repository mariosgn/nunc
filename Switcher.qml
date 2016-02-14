import QtQuick 2.0

Rectangle {
    width: 40
    color: "#b52121"
    property alias mouseArea: mouseArea
    Image {
        id: image1
        y: 0
        height: 100
        anchors.right: parent.right
        anchors.rightMargin: -30
        anchors.left: parent.left
        anchors.leftMargin: -30
        source: "qrc:/images/lock.svg"

        MouseArea {
            id: mouseArea
            anchors.fill: parent
        }
    }

}

import QtQuick 2.5

Rectangle {
    id: rectangle2

    width: 360
    height: 360
    property alias mouseAreaWrite: mouseAreaWrite

    Rectangle {
        id: leftBar
        width: 20
        color: "#1f1f2e"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottomMargin: 0
        anchors.topMargin: 0

        MouseArea {
            id: mouseAreaWrite
            anchors.fill: parent
        }
    }

    Rectangle {
        id: topHeader
        height: 50
        color: "#5d5d5d"
        anchors.left: leftBar.right
        anchors.leftMargin: 0
        anchors.top: parent.top
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
    }

    TextEdit {
        id: textEdit1
        text: qsTr("Text Edit")
        anchors.rightMargin: 5
        antialiasing: true
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        anchors.left: leftBar.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: topHeader.bottom
        anchors.leftMargin: 5
        font.pixelSize: 14
        font.family: "Shadows Into Light Two"
    }


}

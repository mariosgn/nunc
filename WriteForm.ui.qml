import QtQuick 2.5
import QtQuick.Controls 1.3

Rectangle {
    id: baseWrite

    width: 360
    height: 360
    color: "#f2f2f2"
    property string currentText: ""
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
        x: 26
        height: 63
        color: "#838383"
        radius: 6
        anchors.left: leftBar.right
        anchors.leftMargin: 5
        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

        Text {
            id: text1
            y: 15
            color: "#ffffff"
            text: qsTr("SUN")
            anchors.left: parent.left
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            rotation: -90
            font.pixelSize: 17
        }

        Text {
            id: text2
            y: 18
            color: "#ffffff"
            text: "20"
            styleColor: "#000000"
            anchors.left: parent.left
            anchors.leftMargin: 37
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: 50
        }

        Text {
            id: text3
            y: 11
            color: "#ffffff"
            text: qsTr("September")
            anchors.left: text2.right
            anchors.leftMargin: 10
            font.pixelSize: 24
        }

        Text {
            id: text4
            y: 37
            color: "#ffffff"
            text: qsTr("2016")
            anchors.left: text2.right
            anchors.leftMargin: 10
            font.pixelSize: 12
        }
    }

    Rectangle {
        radius: 3
        border.color: "#00000000"
        border.width: 0
        anchors.rightMargin: 20
        antialiasing: true
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.left: leftBar.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: topHeader.bottom
        anchors.leftMargin: 20



        TextEdit {
            id: diarypage
            cursorVisible: true
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8
            anchors.fill: parent
            focus: true
            font.pixelSize: 18
            font.family: "Shadows Into Light Two"

            onTextChanged: {
            baseWrite.currentText = text
            }

        }
    }

}

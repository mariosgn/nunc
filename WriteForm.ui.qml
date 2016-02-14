import QtQuick 2.5
import QtQuick.Controls 1.3

FocusScope
{

    id: baseWrite
    focus: true
    property string currentText: ""
    property alias mouseAreaWrite: mouseAreaWrite

    Rectangle {
        anchors.fill: parent
        color: "#f2f2f2"

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

        DateHeader
        {
            id: topHeader
            x: 26
            height: 63
            anchors.left: leftBar.right
            anchors.leftMargin: 5
            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.right: parent.right
            anchors.rightMargin: 5
        }

        Rectangle {
            radius: 3

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
                focus: true
                cursorVisible: true
                anchors.rightMargin: 8
                anchors.leftMargin: 8
                anchors.bottomMargin: 8
                anchors.topMargin: 8
                anchors.fill: parent

                font.pixelSize: 18
                font.family: "Shadows Into Light Two"

                onTextChanged: {
                    baseWrite.currentText = text
                }

            }
        }
    }
}

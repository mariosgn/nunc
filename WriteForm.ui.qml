import QtQuick 2.5
import QtQuick.Controls 1.3

FocusScope
{
    id: baseWrite
    focus: true
    property alias currentText: diarypage.text

    Rectangle {
        anchors.fill: parent
        color: "#f2f2f2"

        DateHeader
        {
            id: topHeader
            height: 63
            color: "#00000000"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.left: parent.left
        }

        Rectangle {
            radius: 3
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8
            anchors.top: topHeader.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            antialiasing: true

            TextEdit {
                id: diarypage
                focus: true
                color: "#000000"
                antialiasing: true
                cursorVisible: true

                wrapMode: TextEdit.WordWrap

                anchors.rightMargin: 8
                anchors.leftMargin: 8
                anchors.bottomMargin: 8
                anchors.topMargin: 8

                anchors.fill: parent

                font.pixelSize: 18
                font.family: "Shadows Into Light Two"

            }
        }
    }
}

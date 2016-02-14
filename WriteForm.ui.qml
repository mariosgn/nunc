import QtQuick 2.5
import QtQuick.Controls 1.3

FocusScope
{

    id: baseWrite
    focus: true
    property string currentText: ""
    property alias mouseAreaWrite: mouseAreaWrite


   /* function writeData( text )
    {
        var time = Qt.now
        year.text = Qt.formatDateTime( time , "yyyy")
    }*/

    /*Component.onCompleted:
    {
        writeData();
    }*/

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
                id: day
                y: 15
                color: "#ffffff"
                text: new Date().toLocaleString( Qt.locale(), "ddd").toUpperCase()
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.verticalCenter: parent.verticalCenter
                rotation: -90
                font.pixelSize: 17
            }

            Text {
                id: dayN
                y: 18
                color: "#ffffff"
                text: new Date().toLocaleString( Qt.locale(), "dd")
                styleColor: "#000000"
                anchors.left: parent.left
                anchors.leftMargin: 37
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 50
            }

            Text {
                id: month
                y: 11
                color: "#ffffff"
                text: new Date().toLocaleString( Qt.locale(), "MMMM")
                anchors.left: dayN.right
                anchors.leftMargin: 10
                font.pixelSize: 24
            }

            Text {
                id: year
                y: 37
                color: "#ffffff"
                text: new Date().toLocaleString( Qt.locale(), "yyyy")
                anchors.left: dayN.right
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

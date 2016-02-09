import QtQuick 2.0

Rectangle {

    property int textMargin: 10

    width: parent.width
    height: headerDate.height + entryTxt.paintedHeight + textMargin

    Rectangle {
        id: headerDate
        height: 30
        color: "#ffffff"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: textMargin

        Text {
            text: Qt.formatDateTime( time , "hh:mm")
            anchors.fill: parent
            font.bold: true
            font.family: "Shadows Into Light Two"
            wrapMode: Text.WordWrap
            font.pixelSize: 13
        }
    }

    Rectangle {
        id: content
        color: "#ffffff"
        anchors.margins: textMargin
        anchors.top: headerDate.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left        

        Text {
            id: entryTxt
            text: entryText
            font.family: "Shadows Into Light Two"
            wrapMode: Text.WordWrap
            font.pixelSize: 14
            anchors.fill: parent
        }
    }

}

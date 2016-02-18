import QtQuick 2.0

Rectangle {
    width: parent.width
    height: 70 //sectionText.paintedHeight + 10
    color: "#f2f2f2"

    DateHeader
    {
        id: sectionText
        date : section
        anchors.fill: parent
        anchors.margins: 10
        color: "#f2f2f2"
    }
/*
    Text {
        id: sectionText
        text: Qt.formatDateTime( section , "yy MM dd ")
        font.pointSize: 18
        font.family: "Shadows Into Light Two"
        verticalAlignment: Text.AlignVCenter
        anchors.fill: parent
        font.bold: true
        anchors.margins: 10
    }*/
}

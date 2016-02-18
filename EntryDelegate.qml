import QtQuick 2.0

Rectangle {

    property int textMargin: 10
    color: "#f2f2f2"

    width: parent.width
    height: headerDate.height + entryTxt.paintedHeight + textMargin +10 + entryImg.height +8

    Component.onCompleted:
    {
//        console.log( date + " --- " + entryHasImage )
        if ( entryHasImage )
        {
            entryImg.visible = true;
            entryImg.source = entryImage
        }
    }

    Rectangle {
        id: headerDate
        color: "#f2f2f2"
        height: headerDateText.paintedHeight
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.leftMargin: 20
        anchors.top: parent.top
        anchors.margins: 0 //textMargin

        Text {
            id: headerDateText
            text: Qt.formatDateTime( time , "hh:mm")
            anchors.fill: parent
            //            font.bold: true
            font.family: "Shadows Into Light Two"
            font.pixelSize: 12
        }
    }

    Rectangle {
        id: content
        color: "#ffffff"
        radius: 3
        height: entryTxt.paintedHeight + entryImg.height + 16 + 8
        anchors.top: headerDate.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        Image {
            id: entryImg
            asynchronous: true
            visible: false
            fillMode: Image.PreserveAspectFit
            anchors.top: parent.top
            anchors.topMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.left: parent.left
            anchors.leftMargin: 8
        }

        Text {
            id: entryTxt
            text: entryText
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: entryImg.bottom
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8
            font.family: "Shadows Into Light Two"
            wrapMode: Text.WordWrap
            font.pixelSize: 16
        }
    }

}

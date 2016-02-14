import QtQuick 2.0

Rectangle {
    property date headerDate: new Date()

    id: topHeader

    //color: "#838383"
    radius: 6

    Text {
        id: day
        y: 15
        color: "#000000"
        text: headerDate.toLocaleString( Qt.locale(), "ddd").toUpperCase()
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        rotation: -90
        font.pixelSize: 17
    }

    Text {
        id: dayN
        y: 18
        color: "#000000"
        text: headerDate.toLocaleString( Qt.locale(), "dd")
        styleColor: "#000000"
        anchors.left: parent.left
        anchors.leftMargin: 37
        anchors.verticalCenter: parent.verticalCenter
        font.pixelSize: 50
    }

    Text {
        id: month
        y: 11
        color: "#000000"
        text: headerDate.toLocaleString( Qt.locale(), "MMMM")
        anchors.left: dayN.right
        anchors.leftMargin: 10
        font.pixelSize: 24
    }

    Text {
        id: year
        y: 37
        color: "#000000"
        text: headerDate.toLocaleString( Qt.locale(), "yyyy")
        anchors.left: dayN.right
        anchors.leftMargin: 10
        font.pixelSize: 12
    }
}

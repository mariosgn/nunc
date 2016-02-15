import QtQuick 2.4

Rectangle {
    property date headerDate: new Date()
    width: 400
    height: 200

    id: topHeader

    Component.onCompleted:
    {
        month.text = headerDate.toLocaleString( Qt.locale(), "MMMM")
        dayN.text = headerDate.toLocaleString( Qt.locale(), "dd")
        day.text = headerDate.toLocaleString( Qt.locale(), "ddd").toUpperCase()
        year.text = headerDate.toLocaleString( Qt.locale(), "yyyy")
    }
    radius: 1

    Text {
        id: day
        y: 15
        color: "#000000"
        text: "MON"
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        rotation: -90
        font.pixelSize: 17
    }

    Text {
        id: dayN
        height: paintedHeight
        color: "#000000"
        text: "30"
        anchors.verticalCenter: parent.verticalCenter
        styleColor: "#000000"
        anchors.left: parent.left
        anchors.leftMargin: 37
        font.pixelSize: 50
    }

    TextMetrics {
        id: textMetrics
        font: dayN.font
        text: dayN.text
    }


    Text {
        id: month
        x: 0
        color: "#000000"
        text: "September"
        anchors.top: dayN.top
        anchors.topMargin: (textMetrics.height - textMetrics.tightBoundingRect.height )/4
        anchors.left: dayN.right
        anchors.leftMargin: 0
        font.pixelSize: 24
    }

    Text {
        id: year
        x: 0
        y: 0
        color: "#000000"
        text: "2016"
        anchors.top: month.bottom
        anchors.bottomMargin: 0
        anchors.left: dayN.right
        anchors.leftMargin: 0
        font.pixelSize: 12
    }
}

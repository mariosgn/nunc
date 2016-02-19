import QtQuick 2.4

Rectangle {
    property date date: new Date()
    width: 400
    height: 200

    id: topHeader

    radius: 1

    Text {
        id: day
        y: 15
        color: "#000000"
        text: date.toLocaleString( Qt.locale(), "ddd").toUpperCase()
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.verticalCenter: parent.verticalCenter
        font.family: "Shadows Into Light Two"
        rotation: -90
        font.pixelSize: 17
    }

    Text {
        id: dayN
        height: paintedHeight
        color: "#000000"
        text: date.toLocaleString( Qt.locale(), "dd")
        anchors.verticalCenter: parent.verticalCenter
        styleColor: "#000000"
        anchors.left: parent.left
        font.family: "Shadows Into Light Two"
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
        text: date.toLocaleString( Qt.locale(), "MMMM")
        anchors.top: dayN.top
        anchors.topMargin: (textMetrics.height - textMetrics.tightBoundingRect.height )/4
        anchors.left: dayN.right
        font.family: "Shadows Into Light Two"
        anchors.leftMargin: 0
        font.pixelSize: 24
    }

    Text {
        id: year
        x: 0
        y: 0
        color: "#000000"
        text: date.toLocaleString( Qt.locale(), "yyyy")
        anchors.top: month.bottom
        anchors.bottomMargin: 0
        anchors.left: dayN.right
        anchors.leftMargin: 0
        font.family: "Shadows Into Light Two"
        font.pixelSize: 12
    }
}

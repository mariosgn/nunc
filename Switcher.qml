import QtQuick 2.0

Rectangle {
    width: 36
    color: "#3b3b3b"
    border.color: "#00000000"
    property alias mouseArea: mouseArea

    Rectangle {
        id: rectangle1
        height: 50
        color: "#00000000"
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top

        Image {
            id: imageList
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 1
            sourceSize.width:parent.width*0.8
            sourceSize.height: parent.width*0.8
            source: "qrc:/images/list.svg"
        }

        Image {
            id: imageWrite
            anchors.top: parent.top
            anchors.topMargin: 10
            anchors.horizontalCenter: parent.horizontalCenter
            opacity: 0
            sourceSize.width:parent.width*0.8
            sourceSize.height: parent.width*0.8
            source: "qrc:/images/write.svg"
        }

        MouseArea {
            id: mouseArea
            anchors.fill: parent
        }
    }

    states: [
        State {
            name: "StateWriting"

            PropertyChanges {
                target: imageList
                opacity: 0
            }
            PropertyChanges {
                target: imageWrite
                opacity: 1
            }
        }
    ]

}

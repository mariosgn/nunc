import QtQuick 2.0

Rectangle {
    id: base
    property alias entriesList: entriesList
    property alias writeForm: writeForm

    EntriesList {
        id: entriesList
        height: 0
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.rightMargin: base.width
        mouseAreaEntries.onClicked: {
            base.state = ""
        }
    }

    WriteForm {
        id: writeForm
        anchors.left: entriesList.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.leftMargin: 0
        mouseAreaWrite.onClicked: {
            base.state = "StateWriting"
        }
    }
    states: [
        State {
            name: "StateWriting"

            PropertyChanges {
                target: entriesList
                anchors.rightMargin: 0
            }
        }
    ]

    transitions: Transition {
        PropertyAnimation { properties: "anchors.rightMargin"; easing.type: Easing.OutCubic }
    }
}

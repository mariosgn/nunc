import QtQuick 2.0
//import org.nunc 1.0

Rectangle {

    property alias mainView: mainView
    property alias rightPanel: rightPanel
    property alias mouseAreaEntries: mouseAreaEntries
    property alias listView: listView

    Rectangle {
        id: rightPanel
        x: 523
        width: 20
        color: "#852c2c"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: parent.top

        MouseArea {
            id: mouseAreaEntries
            anchors.fill: parent
        }
    }

    Rectangle {
        id: mainView

        anchors.right: rightPanel.left
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.rightMargin: 0

        color: "#f2f2f2"

        ListView {
            id: listView
            anchors.fill: parent
            delegate: EntryDelegate {}

            model: modelData

            section.property: "date"
            section.delegate: EntrySectionDelegate {}
        }
    }



}



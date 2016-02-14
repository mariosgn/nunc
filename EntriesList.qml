import QtQuick 2.0

Rectangle {
    property alias listView: listView

    anchors.fill: parent
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





import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.4

CalendarStyle {
    dayDelegate: Item {
        readonly property color sameMonthDateTextColor: "#444"
        readonly property color selectedDateColor: "#3778d0"
        readonly property color selectedDateTextColor: "white"
        readonly property color differentMonthDateTextColor: "#bbb"
        readonly property color invalidDatecolor: "#dddddd"

        enabled: modelData.entriesAtDate(styleData.date)  > 0

        Rectangle {
            anchors.fill: parent
            border.color: "transparent"
            color: styleData.date !== undefined && styleData.selected ? selectedDateColor : "transparent"
            anchors.margins: styleData.selected ? -1 : 0
            enabled: modelData.entriesAtDate(styleData.date)  > 0
        }

        Image {
            visible: modelData.entriesAtDate(styleData.date)  > 0
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: -1
            width: 12
            height: width
            source: "qrc:/images/eventindicator.png"
        }

        Label {
            enabled: modelData.entriesAtDate(styleData.date)  > 0
            id: dayDelegateText
            text: styleData.date.getDate()
            anchors.centerIn: parent
            color: {
                var color = invalidDatecolor;
                if (styleData.valid) {
                    // Date is within the valid range.
                    color = styleData.visibleMonth ? sameMonthDateTextColor : differentMonthDateTextColor;
                    if (styleData.selected) {
                        color = selectedDateTextColor;
                    }
                }
                color;
            }
        }
    }
}

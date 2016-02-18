import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: calView
    color: "#00000000"
    property alias selectedDate: calendar.selectedDate
    signal showEntry(int modelIdx)

    Rectangle {
        id: rectangle
        x: 82
        y: 69
        width: 400
        height: 400
        color: "#ffffff"
        radius: 6
        border.width: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Calendar {
            id: calendar
            selectedDate: new Date(0,0,0)
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            anchors.fill: parent
//            style: CalendarStyle {
//                gridVisible: false
//                dayDelegate: CalendarDayDelegate { }
//            }
            style: CalendarDayDelegate { }
            onClicked:  {
                var idx = diary.getIndexForDate(selectedDate)
                if (idx>0)
                    calView.showEntry(idx)
            }
        }
    }

}

import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: rectangle3
    width: 500
    height: 316
    color: "#f2f2f2"
    property alias busyIndicator: busyIndicator
    property alias passwordField: passwordField

    function xxx(){
        fadeOut.start();
    }

    states: [
        State {
            name: "StateLogged"
            PropertyChanges {
                target: login
                opacity: 0
                focus: false
            }
        }
    ]

    onStateChanged: {
        busyIndicator.visible = false
    }

    transitions: [
        Transition {
            from: ""; to: "StateLogged"
            PropertyAnimation { duration: 800; properties: "opacity"; easing.type: Easing.InQuad; }
        },
        Transition {
            from: "StateLogged"; to: ""
            PropertyAnimation { duration: 200; properties: "opacity"; easing.type: Easing.InQuad;  }
        }
    ]

    Connections {
        target: diary
        onError: {
            console.error( err )
            errorText.opacity = 1
            errorText.text = err
            xxx();

         }
    }

    Rectangle {
        id: rectangle1
        x: 341
        y: 190
        width: 127
        height: 73
        color: "#00000000"
        anchors.horizontalCenterOffset: 0
        anchors.bottom: rectangle2.top
        anchors.bottomMargin: 18
        anchors.horizontalCenter: parent.horizontalCenter

        Image {
            id: image1
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            sourceSize.width:parent.width
            sourceSize.height: parent.height
            source: "qrc:/images/lock.svg"
        }
    }

    Rectangle {
        id: rectangle2
        x: 327
        y: 100
        width: 200
        height: 48
        color: "#ffffff"
        radius: 4
        border.width: 0
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        TextField {
            focus: true
            id: passwordField
            echoMode: 2
            antialiasing: false
            anchors.rightMargin: 10
            anchors.leftMargin: 10
            anchors.bottomMargin: 10
            anchors.topMargin: 10
            placeholderText: qsTr("Password")
            anchors.fill: parent
            style: TextFieldStyle {
                   textColor: "grey"
                   background: Rectangle {
                       radius: 4
                       implicitWidth: 100
                       implicitHeight: 24
                       border.color: "white"
                       border.width: 1

                   }
            }
            onAccepted: {
                busyIndicator.visible = true
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        visible: false
        anchors.top: rectangle2.bottom
        anchors.topMargin: 23
        anchors.horizontalCenter: parent.horizontalCenter
        running: true
    }

    Text {
        id: errorText
        anchors.top: rectangle2.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: rectangle2.horizontalCenter
        font.pixelSize: 12
        color: "grey"
        opacity: 0
        NumberAnimation on opacity {
            id: fadeOut;
            easing.type: Easing.InQuad
            to: 0;
            duration: 2000
        }
        onTextChanged: {
            busyIndicator.visible = false
        }
    }

}

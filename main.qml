import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: mainw
    visible: true
    //    flags: Qt.Dialog|Qt.WA_OpaquePaintEvent|Qt.WA_NoSystemBackground

    function checkLogin( passwd )
    {
        if ( !diary.open() )
        {
            return;
        }

        if ( diary.checkPassword( passwd ) )
        {
            var res = diary.load( passwd );
            console.log( res )
            if ( res )
            {
                login.state = "StateLogged"
            }
        }
    }

    Rectangle {
        id: base
        anchors.fill: parent
        property alias entriesList: entriesList
        property alias writeForm: writeForm
        property alias login: login

        LoginForm {

            id: login
            z: 1
            anchors.fill: parent
            passwordField.onAccepted: {
                checkLogin( passwordField.text, mainw.diary )
            }
            states: [
                State {
                    name: "StateLogged"

                    PropertyChanges {
                        target: login
                        opacity: 0
                        //visible: false
                    }
                }
            ]

            transitions: Transition {
                PropertyAnimation { properties: "opacity"; easing.type: Easing.InQuad; duration: 1001 }
            }
        }

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
}

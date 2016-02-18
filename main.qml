import QtQuick 2.5
import QtQuick.Window 2.2

Window {
    id: mainw
    visible: true

    property bool diaryOpened: false

    function updateCurrentPage( text )
    {
        diary.setCurrentText( text );
    }

    function checkLogin( passwd )
    {
        if ( !mainw.diaryOpened )
        {
            if ( !diary.open() )
            {
                return;
            }

            if ( diary.checkPassword( passwd ) )
            {
                if ( diary.load( passwd ) )
                {
                    login.state = "StateLogged"
                    writeForm.forceActiveFocus()
                    entriesList.listView.positionViewAtEnd()
                }
            }
            mainw.diaryOpened = true;
        }
        else
        {
            if ( diary.checkPassword( passwd ) )
            {
                login.state = "StateLogged"
                writeForm.forceActiveFocus()
                entriesList.listView.positionViewAtEnd()
            }
        }



    }

    Rectangle {
        id: base
        anchors.fill: parent
        property alias entriesList: entriesList
        property alias writeForm: writeForm
        property alias login: login

        CalendarView
        {
            id: calView
            anchors.fill: parent
            z:1
            visible: false
            onShowEntry: {
                console.log(modelIdx)
                //entriesList.listView.positionViewAtIndex(  modelIdx, ListView.Center )

                var pos = entriesList.listView.contentY;
                var destPos;
                entriesList.listView.positionViewAtIndex(modelIdx, ListView.Beginning);
                destPos = entriesList.listView.contentY;
                anim.from = pos;
                anim.to = destPos;
                anim.running = true;

            }

        }

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
                        focus: false

                    }
                }
            ]

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
        }

        EntriesList {
            id: entriesList
            anchors.rightMargin: switcher.width
            anchors.right: switcher.left
            anchors.left: base.left
            anchors.top: base.top
            anchors.bottom: base.bottom
            NumberAnimation { id: anim; target: entriesList.listView; property: "contentY"; duration: 1000;  easing.type: Easing.InOutQuad; }
        }

        Switcher {
            id: switcher
            anchors.bottom: parent.bottom
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 0
            mouseArea.onClicked: {
                if (base.state == "")
                {
                    base.state = "StateWriting"
                    state = "StateWriting"
                }
                else
                {
                    state = ""
                    base.state = ""
                }
            }
        }

        WriteForm {
            id: writeForm
            anchors.left: switcher.right
            anchors.right: base.right
            anchors.bottom: base.bottom
            anchors.top: base.top
            onCurrentTextChanged: {
                updateCurrentPage( currentText );
            }

        }

        Keys.onPressed: {
            if (event.key == Qt.Key_Escape) {
                login.state = ""
                login.passwordField.text = ""
                login.passwordField.forceActiveFocus()
            }
            else if ((event.key == Qt.Key_E) && (event.modifiers & Qt.ControlModifier))
            {
                if (base.state == "")
                {
                    base.state = "StateWriting"
                    switcher.state = "StateWriting"
                    calView.selectedDate = diary.getLastDate()
                    console.log( diary.getLastDate() )
                }
                else
                {
                    switcher.state = ""
                    base.state = ""

                }
            }
            else if ((event.key == Qt.Key_C) && (event.modifiers & Qt.ControlModifier))
            {
                if (base.state == "StateWriting")
                {
                    calView.visible = !calView.visible
                }
            }
        }

        states: [
            State {
                name: "StateWriting"

                PropertyChanges {
                    target: switcher
                    anchors.leftMargin: base.width - switcher.width
                }
            }
        ]

        transitions: Transition {
            PropertyAnimation { properties: "anchors.leftMargin"; easing.type: Easing.OutCubic }
        }
    }
}

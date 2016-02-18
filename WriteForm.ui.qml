import QtQuick 2.5
import QtQuick.Controls 1.3
import QtQuick.Dialogs 1.2

FocusScope
{
    id: baseWrite
    focus: true
    property alias currentText: diarypage.text
    property string currentImage: ""
    property alias diaryImage: diaryImage

    Rectangle {
        anchors.fill: parent
        color: "#f2f2f2"

        DateHeader
        {
            id: topHeader
            height: 63
            color: "#00000000"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.left: parent.left

            Rectangle {
                id: rectangle1
                height: 30
                width: parent.height
                color: "#00000000"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 10

                Image {
                    id: image1
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/image.svg"
                    sourceSize.width:parent.height*0.8
                    sourceSize.height: parent.height*0.8

                     FileDialog {
                        id: fileDialog
                        title: "Please choose a file"
                        selectMultiple: false
                        visible : false
                        nameFilters: [ "Image files (*.jpg *.png *.jpeg)"]
                        onAccepted: {
                            console.log("You chose: " + fileDialog.fileUrl )
                            baseWrite.currentImage = fileDialog.fileUrl
                            diaryImage.source = fileDialog.fileUrl
                            visible : false
                            diaryImageRect.visible = true
                            diaryImageRect.height = 150
                        }
                        onRejected: {
                            console.log("Canceled")
                            visible : false
                        }
                    }

                    MouseArea {
                        id: mouseArea1
                        anchors.rightMargin: -10
                        anchors.leftMargin: -10
                        anchors.bottomMargin: -10
                        anchors.topMargin: -10
                        anchors.fill: parent
                        onClicked: {
                            fileDialog.visible = true
                        }
                    }
                }
            }
        }

        Rectangle {
            id: rectangle2
            radius: 3
            anchors.rightMargin: 8
            anchors.leftMargin: 8
            anchors.bottomMargin: 8
            anchors.topMargin: 8
            anchors.top: topHeader.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.left: parent.left

            antialiasing: true

            Rectangle {
                id: diaryImageRect
                height: 0
                visible: false
                color: "#ffffff"
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                width: 50
                Image {
                    id: diaryImage
                    fillMode: Image.PreserveAspectCrop
                    anchors.fill: parent
                    sourceSize.width:parent.width

                }
            }

            TextEdit {
                id: diarypage
                focus: true
                color: "#000000"
                anchors.top: diaryImageRect.bottom
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                antialiasing: true
                cursorVisible: true

                wrapMode: TextEdit.WordWrap

                anchors.rightMargin: 8
                anchors.leftMargin: 8
                anchors.bottomMargin: 8
                anchors.topMargin: 8


                font.pixelSize: 18
                font.family: "Shadows Into Light Two"

            }

        }
    }
}

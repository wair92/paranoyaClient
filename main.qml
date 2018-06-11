import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11



Window {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Paranoya client")

    property string message: "Hello"
    signal connectionClicked


    Rectangle{
        property string indicatorColor: "gray"
        id: indicator
        objectName: "indicator"
        anchors.right: parent.right
        width: 20
        height: 20
        color: indicatorColor
        anchors.top: parent.top
    }

    GridLayout{
        rows: 1
        columns: 2
        Rectangle{
            id: connectToServer
            objectName: "connectToServer"
            signal connectionClicked
            color: "red"
            height: 100
            width: 100

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    parent.color = "blue"
                    connectToServer.connectionClicked()
                }
            }

            Text{
                text: "Connect"
                color: "white"
                anchors.centerIn: parent
                height: 20
                width:100
            }
        }

        Rectangle{
            id: disconnectToServer
            objectName: "disconnectToServer"
            signal disconnectionClicked
            color: "green"
            height: 100
            width: 100

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    parent.color = "yellow"
                    disText.color = "black"
                    disconnectToServer.disconnectionClicked()
                }
            }

            Text{
                id: disText
                text: "Disconnect"
                color: "white"
                anchors.centerIn: parent
                height: 20
                width:100
            }
        }

        TextInput{
            signal messageChangedd(string message)
            id: messageInput
            objectName: "messageInput"
            text: "XXXX"
            onAccepted:{
                console.log("Inout accepted")
                messageInput.messageChangedd(messageInput.text)
            }
        }

        Rectangle{
            id: sendText
            objectName: "sendText"
            signal sendMessageClicked
            color: "green"
            height: 100
            width: 100

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    sendText.sendMessageClicked()
                }
            }

            Text{
                id: sendTextText
                text: "Send message"
                color: "white"
                anchors.centerIn: parent
                height: 20
                width:100
            }
        }
    }
}

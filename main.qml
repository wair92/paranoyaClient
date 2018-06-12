import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4


Window {
    id: window
    visible: true
    width: 320
    height: 530
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
        id: loginContainer
        width: 100
        height: 330
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
        rows: 6
        columns: 1
        visible: true
    Text{
        text: "Username:"
    }

    TextInput{
        id: username
        objectName: "username"
        text: "YourUserName"
    }

    Rectangle{
        id: connectToServer
        objectName: "connectToServer"
        signal connectionClicked(string username)
        color: "red"
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        height: 100
        width: 100

        MouseArea{
            anchors.fill: parent
            onClicked: {
                parent.color = "blue"
                connectToServer.connectionClicked(username.text)
                loginContainer.visible = false
                afterLoginContainer.visible = true
            }
        }

        Text{
            text: "Login"
            elide: Text.ElideNone
            color: "white"
            anchors.centerIn: parent
            height: 20
            width:100
        }
    }
}
    GridLayout{
        property bool logged: false
        width: 100
        height: 330
        //anchors.horizontalCenter: parent.horizontalCenter
        //anchors.verticalCenter: parent.verticalCenter
        //anchors.top: parent.top
        //anchors.fill: parent
        rows: 6
        columns: 1
        id: afterLoginContainer
        visible: logged

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
                    afterLoginContainer.visible = false
                    loginContainer.visible = true
                }
            }

            Text{
                id: disText
                text: "Logout"
                color: "white"
                anchors.centerIn: parent
                height: 20
                width:100
            }
        }

        TextArea{
            id: receiver
            text: "Receiver"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 35
        }

        TextArea{
            id: history
            property string historyText: ""
            objectName: "history"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            readOnly: true
            text: historyText
        }

        TextArea{
            signal messageChangedd(string message)
            id: messageInput
            objectName: "messageInput"
            text: "Message"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            onTextChanged: {
                console.log("Input accepted")
                messageInput.messageChangedd(messageInput.text)
            }
            Layout.preferredHeight: 120
        }

        Rectangle{
            id: sendText
            objectName: "sendText"
            signal sendMessageClicked(string receiver)
            color: "green"
            height: 100
            width: 100

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    sendText.sendMessageClicked(receiver.text)
                    history.historyText = history.historyText.concat(messageInput.text)
                    history.historyText = history.historyText.concat("\n")
                    //historyText = history.text
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

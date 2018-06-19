import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4


Item{
    GridLayout{
            property bool logged: true
            width: parent.width
            height: 330
            rows: 6
            columns: 1
            id: afterLoginContainer
            visible: logged

            Label{
                id: username
                text: "uname"
            }

            Rectangle{
                id: disconnectToServer
                objectName: "disconnectToServer"
                signal disconnectionClicked()
                color: "green"
                Layout.preferredHeight: 25
                Layout.preferredWidth: parent.width

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        parent.color = "yellow"
                        disText.color = "black"
                        stackView.pop(null);
                        disconnectToServer.disconnectionClicked()

                    }
                }

                Text{
                    id: disText
                    text: "Logout"
                    color: "white"
                    anchors.centerIn: parent
                    height: 20
                    width: parent.width
                }
            }

            TextArea{
                id: receiver
                text: "Receiver"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.preferredHeight: 35
                Layout.preferredWidth: parent.width
            }

            TextArea{
                id: history
                property string historyText: ""
                objectName: "history"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                readOnly: true
                text: historyText
                Layout.preferredWidth: parent.width
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
                Layout.preferredWidth: parent.width
            }

            Rectangle{
                id: sendText
                objectName: "sendText"
                signal sendMessageClicked(string receiver)
                color: "green"
                Layout.preferredHeight: 25
                Layout.preferredWidth: parent.width

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        sendText.sendMessageClicked(receiver.text)
                        history.historyText = history.historyText.concat(messageInput.text)
                        history.historyText = history.historyText.concat("\n")
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

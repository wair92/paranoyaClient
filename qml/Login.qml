import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4


Item {
    anchors.leftMargin: 10
    GridLayout{
        id: loginContainer
        anchors.centerIn: parent
        width: parent.width
        Layout.preferredHeight:  parent.height
        rows: 6
        columns: 1
        visible: true

        Text{
            text: "Username:"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 25
            horizontalAlignment: Text.AlignHCenter
        }

        TextEdit{
            id: username
            objectName: "username"
            //text: "YourUserName"
            property string placeholderText: "Enter your username "
            Text {
                text: username.placeholderText
                color: "#aaa"
                visible: !username.text
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 25
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle{
            id: connectToServer
            objectName: "connectToServer"
            signal connectionClicked(string username)
            color: "#343537"
            Layout.fillWidth: true
            Layout.preferredHeight: 25
            Layout.preferredWidth: parent.width

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    stackView.push("qrc:/qml/Connecting.qml")
                    connectToServer.connectionClicked(username.text)
                }
            }

            Text{
                text: "Login"
                color: "white"
                height: parent.height
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
            }
        }
    }
}


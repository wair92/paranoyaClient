import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4


Item {

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
        width: parent.width
        Layout.preferredHeight:  parent.height
        rows: 6
        columns: 1
        visible: true
        Text{
            text: "Username:"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 25
        }

        TextInput{
            id: username
            objectName: "username"
            text: "YourUserName"
            Layout.preferredWidth: parent.width
            Layout.preferredHeight: 25
        }

        Rectangle{
            id: connectToServer
            objectName: "connectToServer"
            signal connectionClicked(string username)
            color: "red"
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.preferredHeight: 25
            Layout.preferredWidth: parent.width

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    parent.color = "blue"
                    stackView.pop();
                    stackView.push("qrc:/Chat.qml")
                    connectToServer.connectionClicked(username.text)
                }
            }

            Text{
                text: "Login"
                color: "white"
                height: parent.height
                width: parent.width
            }
        }
    }
}


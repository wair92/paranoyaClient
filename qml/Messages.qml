import QtQuick 2.0


Item {
    id: itemm
    signal chatOpened()
    signal messageListCompleted()
    Rectangle {
        width: parent.width
        height: parent.height
        Component.onCompleted: {
            itemm.messageListCompleted()
        }

        Component {
            id: contactDelegate
            Item {
                width: parent.width; height: 40
                Rectangle{
                    width: parent.width
                    height: parent.height
                    color: index % 2 == 0 ? "white" : "#343537"
                    Column {
                        Text {
                            text: '<b>User:</b> ' + modelData
                            color: index % 2 == 0 ? "#343537" : "white"
                        }
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            itemm.chatOpened()
                            stackView.push("qrc:/qml/Chat.qml")
                        }
                    }
                }
            }
        }

        ListView {
            id: messagesListView
            anchors.fill: parent
            model: myMessages
            delegate: contactDelegate
            focus: true

        }
    }
}

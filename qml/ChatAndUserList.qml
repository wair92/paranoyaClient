import QtQuick 2.11
import QtQuick.Controls 2.4
Item {
    id: list
    objectName: "list"
    signal chatOpened();
    onChatOpened: {
        console.log("Emitting signal!!!")
    }
    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Messages {
            onChatOpened: {
                console.log("Chat opened")
                list.chatOpened()
            }
        }

        Users {
        }
    }

    TabBar {
        id: tabBar
        anchors.bottom: parent.bottom
        width: parent.width
        currentIndex: swipeView.currentIndex

        TabButton {
            text: qsTr("Messages")
            onClicked: {

            }
        }
        TabButton {
            id: userList
            objectName: "userList"
            signal getUserList()
            text: qsTr("Users")
            onClicked: {
                userList.getUserList()
            }
        }
    }
}

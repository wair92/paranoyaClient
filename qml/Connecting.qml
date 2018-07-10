import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick.Controls 1.4

Item {
    id: mainItem
    function connectionConfirmed() {
        stackView.push("qrc:/qml/ChatAndUserList.qml")
    }
    ColumnLayout{
        Layout.preferredHeight: 50
        Layout.preferredWidth: parent.width
        Text{
            width: parent.width
            height: parent.height
            text: "Connecting"
        }
        Button{
            text: "Cancel"
            onClicked: {
                stackView.pop();
            }
        }
        Connections{
            target: helper
            onConnectionConfirmed: mainItem.connectionConfirmed()
        }
    }

}

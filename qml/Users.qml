import QtQuick 2.0

Item {
    Rectangle {
        width: parent.width
        height: parent.height

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
                            text: '<b>Name:</b> ' + modelData
                            color: index % 2 == 0 ? "#343537" : "white"
                        }
                    }
                    MouseArea{
                        id: setReceiver
                        anchors.fill: parent
                        signal setReceiver(string receiver)
                        onClicked:{
                            setReceiver.setReceiver( modelData )
                        }
                    }
                }
            }
        }

        ListView {
            id: listView
            anchors.fill: parent
            model: myModel
            delegate: contactDelegate
            focus: true

        }
    }
}

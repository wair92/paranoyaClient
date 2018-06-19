import QtQuick.Window 2.2
import QtQuick.Layouts 1.11
import QtQuick 2.11
import QtQuick.Controls 2.4


ApplicationWindow {
    id: root
    visible: true
    width: 320
    height: 530
    title: qsTr("Paranoya client")

    StackView {
        id: stackView
        initialItem: "qrc:/Login.qml"
        anchors.fill: parent
    }
}

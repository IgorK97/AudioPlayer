import QtQuick
import QtQuick.Controls

Component {
    Rectangle {
            width: parent.width
            height: 30
            color: playlistMouse.containsMouse ? "#504d4a" : "transparent"

            Text {
                text: model.name
                color: "white"
                font.pixelSize: 12
                leftPadding: 30
                anchors.verticalCenter: parent.verticalCenter
            }

            MouseArea {
                id: playlistMouse
                anchors.fill: parent
                hoverEnabled: true
                cursorShape: Qt.PointingHandCursor
                onClicked: console.log("Playlist selected:", model.name)
            }
        }

}

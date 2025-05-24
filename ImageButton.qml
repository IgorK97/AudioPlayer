import QtQuick

Image {
  id: root

  signal clicked

  opacity: buttonMouseArea.containsMouse ? 0.75 : 1
  mipmap: true
  fillMode: Image.Stretch

  MouseArea {
    id: buttonMouseArea

    anchors.fill: parent

    hoverEnabled: true
    cursorShape: Qt.PointingHandCursor
    onClicked: {
      root.clicked()
    }
  }
}

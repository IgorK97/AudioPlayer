import QtQuick
import QtQuick.Controls

Rectangle{
    id:myMenuItem
    width:350
    height:40
    color:mouseArea.containsMouse ? "white": "#ff8232"
    required property string text
    signal clicked
    Text{
        text:parent.text
        anchors.verticalCenter:parent.verticalCenter
        anchors.left:parent.left
        anchors.leftMargin:15
        font.pixelSize:16
    }
    MouseArea{
        id:mouseArea
        width:  parent.width
        anchors.fill:parent
        hoverEnabled:true
        onClicked:myMenuItem.clicked()
    }



    Rectangle{
        width:parent.width
        height:1
        color:"#cccccc"
        anchors.bottom:parent.bottom
    }
}

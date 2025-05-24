import QtQuick
import QtQuick.Controls

ComboBox {
    id: control
    property string label: ""
    height: 30
    textRole:"display"
    signal selectionChanged(string value)
    onCurrentIndexChanged:{

        selectionChanged(currentText);
    }

    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: model.display
            color: "#ffffff"
            font.pixelSize: 12
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        background: Rectangle {
            color: highlighted ? "#504d4a" : "transparent"
        }
    }

    indicator: Canvas {
        x: control.width - width - 5
        y: control.topPadding + (control.availableHeight - height) / 2
        width: 10
        height: 5
        contextType: "2d"

        onPaint: {
            context.reset()
            context.moveTo(0, 0)
            context.lineTo(width, 0)
            context.lineTo(width / 2, height)
            context.closePath()
            context.fillStyle = "#ffffff"
            context.fill()
        }
    }

    contentItem: Text {
        leftPadding: 5
        rightPadding: control.indicator.width + 10
        text: control.label + ": " + control.currentText
        color: "#ffffff"
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
        font.pixelSize: 12
    }

    background: Rectangle {
        color: "transparent"
        border.color: control.hovered ? "#666666" : "#444444"
        border.width: 1
        radius: 3
    }

    popup: Popup {
        y: control.height
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
        }

        background: Rectangle {
            color: "#3a3735"
            border.color: "#444444"
            radius: 3
        }
    }
}

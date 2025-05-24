import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import com.company.MyAudioPlayer

Item {
    id: seekController

    implicitHeight: 15

    function formatToMinutes(milliseconds) {
        const min = Math.floor(milliseconds / 60000)
        const sec = ((milliseconds - min * 60000) / 1000).toFixed(1)
        return `${min}:${sec.padStart(4, 0)}`
    }

    RowLayout {
        anchors.fill: parent
        spacing: 22


        Text {
            id: currentTime
            Layout.preferredWidth: 45
            text: seekController.formatToMinutes(MyAudioPlayer.position)
            horizontalAlignment: Text.AlignLeft
            font.pixelSize: 11
        }


        Slider {
            id: slider
            Layout.fillWidth: true

            enabled: MyAudioPlayer.seekable
            value: MyAudioPlayer.position / MyAudioPlayer.duration

            onMoved: {

                MyAudioPlayer.setPosition(value * MyAudioPlayer.duration);
            }
        }


        Text {
            id: remainingTime
            Layout.preferredWidth: 45
            text: seekController.formatToMinutes(MyAudioPlayer.duration - MyAudioPlayer.position)
            horizontalAlignment: Text.AlignRight
            font.pixelSize: 11
        }

    }
}


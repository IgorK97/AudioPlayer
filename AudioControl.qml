import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.company.MyAudioPlayer

Item {
    id: audioController

    property alias busy: slider.pressed

    property real volume: slider.value


    width:40
    height:40



    RowLayout {
        id: mainLayout
        spacing: 10
        anchors.verticalCenter: parent.verticalCenter


            RoundButton {
            id: muteButton
            implicitWidth: 40
            implicitHeight: 40
            radius: 4
            icon.width: 24
            icon.height: 24
            flat: true

            icon.source: MyAudioPlayer.muted ? "assets/images/volume_mute.svg" : "assets/images/volume.svg"

            checkable: true

            checked: MyAudioPlayer.muted

                        onClicked: {
                            MyAudioPlayer.setMuted(checked)
                        }
        }


        Slider {
                    id: slider
                    visible: true
                    implicitWidth: 136
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter


                    from:0
                    to:1
                    stepSize:0.01
                    value: MyAudioPlayer.volume
                    enabled: !MyAudioPlayer.muted

                    onMoved: {
                        MyAudioPlayer.setVolume(value)
                    }


                    live: true
                }

    }
}

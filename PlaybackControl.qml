import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Dialogs
import com.company.PlayerController
import com.company.MyAudioPlayer


Item {
    id: playbackController



    implicitHeight: 168/*landscapePlaybackControls ? 168 : 208*/

    Behavior on opacity { NumberAnimation { duration: 300 } }





    Image{
        id:coverArt
        width:80
        height:80

        fillMode:Image.preserveAspectFit

        source: PlayerController.coverImage
        MouseArea {
            anchors.fill: parent
            onClicked: mainWindow.isExpanded = !mainWindow.isExpanded
        }

    }



    component CustomButton: RoundButton {
        implicitWidth: 40
        implicitHeight: 40
        radius: 4
        icon.width: 24
        icon.height: 24
        flat: true
    }

    component CustomRoundButton: RoundButton {
        property int diameter: 40
        Layout.preferredWidth: diameter
        Layout.preferredHeight: diameter
        radius: diameter / 2
        icon.width: 24
        icon.height: 24
    }



    CustomButton {
        id: fullScreenButton
        icon.source: root.fullScreen ? "assets/images/zoom_minimize.svg"
                                     : "assets/images/zoom_maximize.svg"
        onClicked: {
            root.fullScreen ?  root.showNormal() : root.showFullScreen()
            root.fullScreen = !root.fullScreen
        }
    }

    RowLayout {
        id: controlButtons
        spacing: 16
        // anchors.horizontalCenter: parent.horizontalCenter

        CustomButton {
            id: shuffleButton
            icon.source: PlayerController.shuffle ? "assets/icons/Shuffle_Active.svg" : "assets/icons/Shuffle_Icon.svg"
            // icon.color: PlayerController.loops === MediaPlayer.Once ? palette.buttonText : palette.accent
            onClicked:{
                PlayerController.setShuffle(!PlayerController.shuffle);
            }


        }

        ImageButton{
            id: prevButton
            width:40
            height:40
            Layout.maximumHeight: 40
            Layout.maximumWidth: 40
            source: "assets/icons/previous_icon.png"
            onClicked:{
                PlayerController.switchToPreviousTrack();
            }
        }



        CustomRoundButton {
            id: backward10Button
            icon.source: "assets/images/backward10.svg"
            onClicked: {
                MyAudioPlayer.stepBack();

            }
        }


        CustomRoundButton {
            id: playButton
            visible: true
            icon.source: MyAudioPlayer.playing ? "assets/images/pause_symbol.svg" : "assets/images/play_symbol.svg"
            onClicked: {
                MyAudioPlayer.playPause();
                // playbackController.mediaPlayer.play()
            }
        }




        CustomRoundButton {
            id: forward10Button
            icon.source: "assets/images/forward10.svg"
            onClicked:{
                MyAudioPlayer.stepForward();
            }

        }

        ImageButton{
            id: nextButton
            width:40
            height:40
            Layout.maximumHeight: 40
            Layout.maximumWidth: 40
            source: "assets/icons/next_icon.png"
            onClicked:{
                PlayerController.switchToNextTrack();
            }
        }


        CustomButton {
            id: loopButton
            icon.source: "assets/images/loop.svg"
            icon.color: MyAudioPlayer.loops === MediaPlayer.Once ? palette.buttonText : palette.accent
            onClicked:{
                MyAudioPlayer.setLoops(MyAudioPlayer.loops === MediaPlayer.Once
                                          ? MediaPlayer.Infinite
                                          : MediaPlayer.Once);
            }


        }
        AudioControl {
            id: audioControl
        }

    }



    PlaybackSeekControl {
        id: playbackSeekControl
        Layout.fillWidth: true

    }

    Frame {
        id: landscapeLayout
        anchors.fill: parent
        padding: 32
        topPadding: 28
        visible: true
        background: Rectangle {
            color:"#4d4e4e"
        }

        ColumnLayout {
            anchors.fill: parent
            spacing:16
            // anchors.fill: parent
            // spacing: 16


            LayoutItemProxy {
                target: playbackSeekControl

                Layout.topMargin: 3
                Layout.bottomMargin: 3
            }

            RowLayout {
                Layout.fillWidth: true
                implicitHeight: 80
                spacing:10


                RowLayout{
                    spacing:10
                    // spacing:10


                    LayoutItemProxy{
                        id:coverArtProxy
                        target:coverArt
                        Layout.preferredHeight: 80
                        Layout.preferredWidth:80
                    }



                    ColumnLayout{
                        id:trackInfoProxy
                        spacing:4
                        Layout.alignment:Qt.AlignLeft

                        Layout.preferredWidth: 300


                        Item{
                            height:24

                            Layout.preferredWidth: 300

                            Rectangle {
                                id: textClip
                                anchors.fill: parent
                                color: "transparent"
                                clip: true

                                Text {
                                    id: scrollingText
                                    text: PlayerController.title
                                    font.pixelSize: 18
                                    font.bold: true
                                    color: "white"
                                    width: implicitWidth
                                    height: parent.height
                                    x: needsScroll ? 0 : (textClip.width-implicitWidth)/2

                                    property bool needsScroll: implicitWidth > textClip.width
                                    property real scrollTo:implicitWidth-textClip.width

                                    SequentialAnimation on x {
                                        id:scrollAnim
                                        running: scrollingText.needsScroll
                                        loops: Animation.Infinite
                                        PauseAnimation{duration:2000}
                                        NumberAnimation {
                                            from: 0
                                            to: -scrollingText.scrollTo
                                            duration: 3000
                                            easing.type: Easing.Linear
                                        }

                                        PauseAnimation { duration: 3000 }
                                        ScriptAction{script:x=0}
                                    }
                                }
                            }
                        }
                        Item{
                            height:24

                            Layout.preferredWidth: 300

                            Rectangle {
                                id: textClip2
                                anchors.fill: parent
                                color: "transparent"
                                clip: true


                                Text {
                                    id: scrollingText2
                                    text: PlayerController.artist+" - " +PlayerController.album
                                    font.pixelSize: 14
                                    // font.bold: true
                                    color: "#DDDDDD"
                                    width: implicitWidth
                                    height: parent.height
                                    x: needsScroll ? 0 : (textClip2.width-implicitWidth)/2

                                    property bool needsScroll: implicitWidth > textClip2.width
                                    property real scrollTo:implicitWidth-textClip2.width

                                    SequentialAnimation on x {
                                        id:scrollAnim2
                                        running: scrollingText2.needsScroll
                                        loops: Animation.Infinite
                                        PauseAnimation{duration:2000}
                                        NumberAnimation {
                                            from: 0
                                            to: -scrollingText2.scrollTo
                                            duration: 3000
                                            easing.type: Easing.Linear
                                        }

                                        PauseAnimation { duration: 3000 }
                                        ScriptAction{script:x=0}
                                        PauseAnimation{duration:1000}
                                    }
                                }
                            }
                        }

                    }
                }

                // Item{
                //     Layout.fillWidth:true
                // }



                    RowLayout{


                        Layout.alignment:Qt.AlignHCenter
                        LayoutItemProxy {
                        id:controlButtonsProxy
                        target: controlButtons

                            // anchors.horizontalCenter: parent.horizontalCenter
                        // Layout.alignment: Qt.AlignCenter
                    }
                    }

                Item{
                    Layout.fillWidth:true
                }

                RowLayout{

                    Layout.alignment: Qt.AlignRight
                    Layout.fillWidth:true
                    LayoutItemProxy {
                        id: fbProxy
                        target: fullScreenButton
                            // anchors.right: parent.right
                        // Layout.alignment: Qt.AlignCenter
                    }
                }
            }


        }
    }
}

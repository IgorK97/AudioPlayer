import QtQuick
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Dialogs
import QtMultimedia
import QtQuick.Effects
import QtQuick.Layouts
import com.company.PlayerController





ApplicationWindow {

    id: root
    title: qsTr("My Audio Player")
    width: 1280
    height: 720
    minimumWidth: 960
    minimumHeight: 540
    visible: true
    color: "#2a2723"


    property bool fullScreen: false


    Item{
        id:mainWindow
        width: parent.width
        height:  parent.height
        property bool isExpanded:false

        FileDialog {
            id: fileDialog
            title: "Выберите файл"

            onAccepted: {
                PlayerController.openAudio(fileDialog.selectedFile)

            }
        }




        Rectangle {

            id: topbar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 50
            color:"#ff8232"
            visible:!mainWindow.isExpanded
            RowLayout{
                anchors.fill: parent
                anchors.margins: 10
                spacing: 15

                ColumnLayout{


                    RoundButton {
                        id: fileDialogButt
                        width:40
                        height:40
                        Layout.maximumHeight: 40
                        Layout.maximumWidth: 40


                        icon.source: "assets/images/open_new.svg"


                        onClicked: {

                            fileDialog.open()
                        }
                    }
                }
                ColumnLayout{
                    RoundButton {
                        id:but1
                        Layout.alignment: Qt.AlignCenter

                        width: 32
                        height: 32
                        icon.source: "assets/icons/my_menu_icon.png"
                        onClicked: {
                            if(!musicPanel.visible){

                                musicPanel.visible=true;

                            }
                            else {

                                musicPanel.visible=false;
                            }
                        }
                    }
                }

                RowLayout {


                    spacing: 15

                    CustomComboBox {
                        id: genreComboBox
                        model: dbHandler.genresModel
                        currentIndex: 0
                        width: Math.min(300, Math.max(120, contentItem.implicitWidth+30))
                        label: "Жанр"
                        onSelectionChanged: {

                            Qt.callLater(function(){
                                const genre = genreComboBox.currentText.replace("Жанр: ","");
                                const artist = artistComboBox.currentText.replace("Исполнитель: ","");
                                const album = albumComboBox.currentText.replace("Альбом: ","");

                                PlayerController.filterTracks(artist, album, genre);
                            });
                        }

                    }
                    CustomComboBox {
                        id:artistComboBox
                        model: dbHandler.artistsModel
                        currentIndex: 0
                        width:  Math.min(300, Math.max(180, contentItem.implicitWidth+30))
                        label: "Исполнитель"
                        onSelectionChanged: {
                            Qt.callLater(function(){const genre = genreComboBox.currentText.replace("Жанр: ","");
                            const artist = artistComboBox.currentText.replace("Исполнитель: ","");
                            const album = albumComboBox.currentText.replace("Альбом: ","");

                            PlayerController.filterTracks(artist, album, genre);
                            });
                        }
                    }
                    CustomComboBox {
                        id:albumComboBox
                        model: dbHandler.albumsModel
                        currentIndex: 0
                        width:  Math.min(300, Math.max(180, contentItem.implicitWidth+30))
                        label: "Альбом"
                        onSelectionChanged:{
                            Qt.callLater(function(){const genre = genreComboBox.currentText.replace("Жанр: ","");
                            const artist = artistComboBox.currentText.replace("Исполнитель: ","");
                            const album = albumComboBox.currentText.replace("Альбом: ","");

                            PlayerController.filterTracks(artist, album, genre);
                            });
                        }
                    }



                }


            }


        }

        EditPanel {
            id: musicPanel
            anchors.top: topbar.bottom
            anchors.left:parent.left

            x:0
            width: visible ? 350 : 0
            visible:false
        }
        TrackEditPanel {
            id: trackEditPanel
            anchors.top: topbar.bottom
            // anchors.bottom:playbackController.top
            hidden:true
            x: hidden ? parent.width : parent.width - width
        }

        ListView {
                id: mainTrackList
                visible: !mainWindow.isExpanded
                anchors {
                    top: topbar.bottom
                    bottom: playbackController.top
                    left: musicPanel.right
                    right: parent.right
                }
                model: PlayerController
                delegate:TrackDelegate{}
                spacing:5
        }

        Item{
            anchors.fill:parent
            Image{
            id:bluredBackground
            fillMode:Image.Stretch
            anchors.fill:parent
            source:PlayerController.coverImage
            // scale:2.5
            opacity:mainWindow.isExpanded ? 0.5 : 0
            visible:opacity>0

            Behavior on opacity {
                NumberAnimation {
                    duration:400
                    easing.type: Easing.InOutQuad
                }
            }
        }
            // MultiEffect{
            //     opacity:mainWindow.isExpanded ? 1 : 0
            //     visible:opacity>0
            //     anchors.fill:parent
            //     source:bluredBackground
            //     blurEnabled:true
            //     blurMax:64
            //     blur:0.5
            //     blurMultiplier:2.0
            //     Behavior on opacity {
            //         NumberAnimation {
            //             duration:400
            //             easing.type: Easing.InOutQuad
            //         }
            //     }
            // }
        }
        // Image{
        //     id: coverArt
        //     // visible:mainWindow.isExpanded
        //             width: mainWindow.isExpanded ? 160 : 80
        //             height: mainWindow.isExpanded ? 160 : 80
        //             source: PlayerController.coverImage
        //             anchors {
        //                 horizontalCenter: parent.horizontalCenter
        //                 bottom: mainWindow.isExpanded ? playbackController.top : playbackController.top
        //                 verticalCenter: mainWindow.isExpanded ? undefined : playbackController.verticalCenter
        //             }
        //             fillMode: Image.PreserveAspectFit

        //             MouseArea {
        //                 anchors.fill: parent
        //                 onClicked: mainWindow.isExpanded = !mainWindow.isExpanded
        //             }

        //             Behavior on width { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
        //             Behavior on height { NumberAnimation { duration: 300; easing.type: Easing.InOutQuad } }
        //             Behavior on anchors.verticalCenterOffset { NumberAnimation { duration: 300 } }
        // }
        PlaybackControl {
            id: playbackController

            property bool showControls: true
            opacity: showControls

            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right

        }

    }

}


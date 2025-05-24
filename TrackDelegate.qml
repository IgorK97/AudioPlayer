import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import com.company.PlayerController

Rectangle {
    id: trackDelegate
    required property int audioIndex
    required property int audioModelIndex
    required property string audioTitle
    required property string audioArtist
    required property string audioAlbum
    required property string audioGenre
    required property string audioDuration
    required property bool audioInFavorite

    width: mainWindow.width/*ListView.view.width*/
    height: 60
    color: mouseArea.containsMouse ? "#504d4a" : "transparent"

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: {
            PlayerController.setCurrentTrack(audioModelIndex)
        }
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 15

        ColumnLayout{

        ImageButton{
            id: playButton
            width:40
            height:40
            Layout.maximumHeight: 40
            Layout.maximumWidth: 40
            source:"assets/icons/play.png"
            onClicked:{
                PlayerController.setCurrentTrack(audioModelIndex);
            }
        }
        }



        ColumnLayout {
Layout.fillWidth: true
Layout.maximumWidth: 300

            Text {
Layout.maximumWidth: 300
                text: audioTitle
                color: "white"
                font {
                    pixelSize: 14
                    bold: true
                }
                elide: Text.ElideRight
            }
        }
        ColumnLayout{
Layout.fillWidth: true
Layout.maximumWidth: 300

            Text {
Layout.maximumWidth: 300
                text: `${audioArtist} • ${audioAlbum}`
                color: "#aaaaaa"
                font.pixelSize: 14
                elide: Text.ElideRight
            }
        }
        ColumnLayout{
            Layout.fillWidth: true
            Layout.maximumWidth: 300
            Text {

                text: audioGenre
                color: "#777777"
                font.pixelSize: 14
                elide: Text.ElideRight
            }
        }
        ColumnLayout{

            Text {

                text: audioDuration
                color: "white"
                font.pixelSize: 14
                elide:Text.ElideRight

            }
        }
        RowLayout{
            spacing: 10
            width:300
            ImageButton{
                id:minusButton
                width:40
                height:40
                Layout.maximumHeight: 40
                Layout.maximumWidth: 40
                source:"assets/icons/cross_icon.png"
                visible: PlayerController.currentPlaylist!==""
                onClicked:{
                    PlayerController.removeFromPlaylist(audioIndex);
                }
            }

            ImageButton{
                id: plusButton
                width:40
                height:40
                Layout.maximumHeight: 40
                Layout.maximumWidth: 40
                source:"assets/icons/plus_icon.png"
                onClicked:{
                    playlistPopup.opened ? playlistPopup.close() : playlistPopup.open();
                }
                Popup{
                    id:playlistPopup
                    y:plusButton.height+5
                    x:(plusButton.width-width)/2
                    width:200
                    height:Math.min(5*40,listView.contentHeight)
                    background:Rectangle{
                        color:"#ffffff"
                        border.color:"#2c2c2c"
                        radius:4
                    }
                    contentItem:ListView{
                        id:listView
                        clip:true
                        model:dbHandler.playlistsModel

                        delegate:Rectangle{
                            width:listView.width
                            height:40
                            color:myMouseArea.containsMouse ? "#383838" : "#2c2c2c"
                            property string playlistName: model.display
                            Text{
                                anchors.verticalCenter:parent.verticalCenter
                                x:10
                                text:playlistName
                                color:"#FFFFFF"
                                font.pixelSize: 14
                            }
                            MouseArea{
                                id:myMouseArea
                                anchors.fill:parent
                                hoverEnabled:true
                                cursorShape:Qt.PointingHandCursor
                                onClicked:{
                                    PlayerController.addToPlaylist(audioIndex, playlistName)
                                    playlistPopup.close();
                                }
                            }
                        }

                        // delegate:ItemDelegate{
                        //     width:listView.width
                        //     height:40
                        //     text:model.display

                        //     onClicked:{
                        //         PlayerController.addToPlaylist(text);
                        //         playlistPopup.close();
                        //     }
                        // }
                        ScrollBar.vertical:ScrollBar{
                            policy:listView.contentHeight>listView.height ?
                                       ScrollBar.AlwaysOn:ScrollBar.AsNeeded
                            background:Rectangle{
                                color:"#2c2c2c"
                            }
                            contentItem: Rectangle {
                                    color: "#c0c0c0"
                                    implicitWidth: 6
                                    radius: 3
                                }
                        }
                    }
                }

            }
            ImageButton{
                id: favButton
                width:40
                height:40
                Layout.maximumHeight: 40
                Layout.maximumWidth: 40
                source:audioInFavorite ? "assets/icons/heartFav.png" : "assets/icons/heartNoFav.png"
                onClicked:{
                    if(audioInFavorite){
                        PlayerController.removeFromFavorites(audioIndex);
                    }
                    else{
                        PlayerController.addToFavorites(audioIndex);
                    }
                }
            }
            ImageButton{
                id: editButton
                width:40
                height:40
                Layout.maximumHeight: 40
                Layout.maximumWidth: 40
                source: "assets/icons/edit.png"
                onClicked:{
                    trackEditPanel.loadTrackData(audioIndex)
                    trackEditPanel.hidden=false;
                }
            }
            ImageButton{
                id: delButton
                width:40
                height:40
                Layout.maximumHeight: 40
                Layout.maximumWidth: 40
                source:"assets/icons/delete.png"
                onClicked:{
                    PlayerController.removeAudio(audioIndex);
                }
            }

        }
    }

}

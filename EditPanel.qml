import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import com.company.PlayerController;

Rectangle {
    id: root
    property int editIndex: -1
    property bool playlistsExpanded:false
    property string selectedPlaylist:""
    anchors.top:topbar.bottom
    anchors.bottom:playbackController.top
    color: "#241f1a"


       ScrollView{
           width:parent.width
           height:  parent.height
           anchors.top:parent.top
           Column {
                width:parent.width
                anchors.fill: parent
                spacing:10
                anchors.rightMargin: 5
                anchors.leftMargin: 5
            Text {
                text: "Главная"
                color: "white"
                font.bold: true
                font.pixelSize: 20

            }
            MenuButton{
                text:"Библиотека музыки"
                onClicked:{
                    PlayerController.loadTracks();
                }
            }
            MenuButton{
                text:"Избранное"
                onClicked:{
                    PlayerController.loadFavorites();
                }
            }
            MenuButton{
                text:"Плейлисты"
                onClicked:playlistsExpanded = !playlistsExpanded
            }

            // Column{
            //     visible: playlistsExpanded
            //     spacing:5
                ListView{
                    id:playlistView
                    model:dbHandler.playlistsModel
                    visible:playlistsExpanded
                    width:parent.width
                    height:200
                    delegate: Rectangle{
                        width: parent.width
                        property bool hovered:false
                        property bool editing :false
                        height:50
                        color:hovered? "#3a3730":"transparent"
                        border.color:"#444444"

                        Text{
                            anchors.centerIn:  parent
                            //anchors.left:parent.left
                            //anchors.verticalCenter:parent.verticalCenter
                            text: model.display
                            color:"white"
                            font.pixelSize:14
                            visible:!parent.editing
                        }
                        TextInput {
                            id:playlistNameEdit
                            anchors.centerIn:parent
                            width:parent.width-20
                            text:model.display
                            color:"white"
                            font.pixelSize: 14
                            visible: parent.editing
                            selectByMouse: true
                            enabled: true
                            onAccepted:{
                                parent.editing=false
                                if(text!==model.display && text!==""){
                                    dbHandler.renamePlaylist(model.display, text);
                                }
                            }
                            Keys.onEscapePressed: {
                                parent.editing=false;
                            }
                        }

                        MouseArea{
                            id:mouseArea
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            anchors.fill:parent
                            hoverEnabled: true
                            enabled:!parent.editing
                            onEntered:parent.hovered=true
                            onExited:parent.hovered=false
                            onClicked:{
                                if(mouse.button===Qt.LeftButton){
                                    PlayerController.setCurrentPlaylist(model.display);
                                    root.selectedPlaylist=model.display;
                                }
                                else /*if(mouse.button===Qt.RightButton)*/{

                                    playlistNameEdit.focus=true;
                                    playlistNameEdit.selectAll();
                                    parent.editing=true;
                                }


                            }

                        }
                    }
                }

                Row{
                    spacing:10
                    Button{
                        text:"Добавить"
                        onClicked:playlistDialog.open()
                    }
                    Button{
                        text:"Удалить"
                        onClicked:deleteConfirmDialog.open()
                    }
                }
            // }

        }
    }
       Dialog{
           id:playlistDialog
           title:"Название нового плейлиста"
           modal:true
           width:250
           standardButtons:Dialog.Ok|Dialog.Cancel
           Column{
               spacing:10
               padding:10
               width:parent.width-20
               TextField{
                   id:playlistNameInput
                   placeholderText:"Введите имя плейлиста"

               }
           }
           onAccepted: {
               if(playlistNameInput.text.length>0){
                   dbHandler.addPlaylist(playlistNameInput.text);
                   PlayerController.setCurrentPlaylist(playlistNameInput.text);
               }
               playlistNameInput.text="";
           }
           onRejected:playlistNameInput.text=""
       }

       Dialog{
           id:deleteConfirmDialog
           title:"Удалить плейлист?"
           Column{
               spacing:10
               padding:10
               // Text{

               //     text:"Аудиотреки при удалении плейлиста удалены не будут"
               //     width:parent.width-20
               // }
           }

           // icon:StandardIcon.Warning
           standardButtons:Dialog.Ok|Dialog.Cancel
           onAccepted:{

               dbHandler.deletePlaylist(root.selectedPlaylist);
               root.selectedPlaylist="";
               PlayerController.setCurrentPlaylist("");
           }
       }


}

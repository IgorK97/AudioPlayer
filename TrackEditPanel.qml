import QtQuick
import QtQuick.Controls
import com.company.PlayerController


Rectangle {
    id: root
    property bool hidden: true
    property int editIndex: -1

    width:350
    // height: parent.height
    // y:topbar.height
    anchors.top:topbar.bottom
    anchors.bottom:playbackController.top
    color: "#333333"
    z:1000
    MouseArea{
        anchors.fill:parent
        hoverEnabled:true
    }


    function loadTrackData(index) {

        editIndex = index
        var apt = PlayerController.getTrack(index)

        artistCombo.currentIndex = dbHandler.getIndexFromNameForArtist(apt["artist"])
        albumCombo.currentIndex = dbHandler.getIndexFromNameForAlbum(apt["album"])
        genreCombo.currentIndex = dbHandler.getIndexFromNameForGenre(apt["genre"])

        titleField.text = apt["title"]


    }


       ScrollView{
            width:parent.width
            height: parent.height
            anchors.top:parent.top
            Column {
                width:parent.width
                anchors.fill: parent
                spacing:10
                anchors.rightMargin: 5
                anchors.leftMargin: 5
                Text {
                    text: "Редактирование трека"
                    color: "white"
                    font.bold: true
                    font.pixelSize: 20

                }
                Text{
                    text:"Название"
                    color:"white"
                    font.bold:true
                    font.pixelSize:12
                }
                TextField{
                    id:titleField
                    placeholderText:"Введите название трека"
                    width:parent.width
                }



                Text{
                    text:"Исполнитель/Композитор"
                    color:"white"
                    font.bold:true
                    font.pixelSize:12
                }


                ComboBox {
                    id: artistCombo
                    width: parent.width
                    editable:true

                    model: dbHandler.artistsModel
                    textRole:"display"

                    onFocusChanged:{
                        if(!focus){
                            const foundIndex=find(editText);
                            if(foundIndex===-1){
                                currentIndex=-1;
                            }
                        }
                    }

                    popup: Popup{
                        y:artistCombo.height
                        width:artistCombo.width
                        height:Math.min(contentItem.implicitHeight,5*40)
                        implicitWidth:artistCombo.implicitWidth
                        contentItem:ListView{
                            ScrollBar.vertical: ScrollBar{
                                id:myScrollForStreet
                                policy:ScrollBar.AlwaysOn
                            }

                            clip:true
                            model:artistCombo.delegateModel
                            delegate: artistCombo.delegate
                            implicitHeight: contentHeight
                            boundsBehavior: Flickable.StopAtBounds
                        }
                    }


                }
               Row{
                   spacing:10
                   width:parent.width
                   Button{
                        text:"+ Добавить нового исполнителя"
                        onClicked:artistDialog.open()
                    }
                    Button{
                        text:"- Удалить"
                        onClicked:{
                            if(artistCombo.currentIndex>=0&&artistCombo.currentIndex>=0){

                                dbHandler.deleteArtist(artistCombo.currentIndex);
                                artistCombo.currentIndex=-1;
                            }
                        }
                    }
               }

                Text{
                    text:"Альбом"
                    color:"white"
                    font.bold:true
                    font.pixelSize:12
                }

                ComboBox{
                    id: albumCombo
                    width: parent.width
                    editable:true
                    model: dbHandler.albumsModel
                    textRole:"display"
                    onFocusChanged:{
                        if(!focus){
                            const foundIndex=find(editText);
                            if(foundIndex===-1){
                                currentIndex=-1;
                            }
                        }
                    }

                    popup: Popup{
                        y:albumCombo.height
                        width:albumCombo.width
                        height:Math.min(contentItem.implicitHeight,5*40)
                        implicitWidth:albumCombo.implicitWidth
                        contentItem:ListView{
                            ScrollBar.vertical: ScrollBar{
                                id:myScrollForHouse
                                policy:ScrollBar.AlwaysOn
                            }

                            clip:true
                            model:albumCombo.delegateModel
                            delegate: albumCombo.delegate
                            implicitHeight: contentHeight
                            boundsBehavior: Flickable.StopAtBounds
                        }
                    }
                }
                Row{
                    spacing:10
                    width:parent.width
                    Button{
                        text:"+ Добавить новый альбом"
                        onClicked:albumDialog.open()
                    }
                    Button{
                        text:"- Удалить"
                        onClicked:{
                            if(albumCombo.currentIndex>=0&&albumCombo.currentIndex>=0&&houseCombo.currentIndex>=0){

                                dbHandler.deleteAlbum(albumCombo.currentIndex);
                                albumCombo.currentIndex=-1;
                            }
                        }
                    }
                }

                Text{
                    text:"Жанр"
                    color:"white"
                    font.bold:true
                    font.pixelSize:12
                }

                ComboBox {
                    id: genreCombo
                    width: 250
                    model: dbHandler.genresModel

                    textRole: "display"
                    onFocusChanged:{
                        if(!focus){
                            const foundIndex=find(editText);
                            if(foundIndex===-1){
                                currentIndex=-1;
                            }
                        }
                    }

                    popup: Popup{
                        y:genreCombo.height
                        width:genreCombo.width
                        height:Math.min(contentItem.implicitHeight,5*40)
                        implicitWidth:genreCombo.implicitWidth
                        contentItem:ListView{
                            ScrollBar.vertical: ScrollBar{
                                id:myScrollForMaterialV
                                policy:ScrollBar.AlwaysOn
                            }

                            clip:true
                            model:genreCombo.delegateModel
                            delegate: genreCombo.delegate
                            implicitHeight: contentHeight
                            boundsBehavior: Flickable.StopAtBounds
                        }
                    }

                }
                Row{
                    spacing:10
                    width:parent.width
                    Button{
                        text:"+ Добавить новый жанр"
                        onClicked:genreDialog.open()
                    }
                    Button{
                        text:"- Удалить"
                        onClicked:{
                            if(genreCombo.currentIndex>=0){

                                dbHandler.deleteGenre(genreCombo.currentIndex);
                                genreCombo.currentIndex=-1;
                            }
                        }
                    }
                }

                Row{
                    spacing:10
                    width:parent.width
                    Button {
                    text: "Сохранить"
                    onClicked: {
                        if(artistCombo.currentIndex!==-1 && albumCombo.currentIndex!==-1
                                && genreCombo.currentIndex!==-1 && titleField.text!=="")

                            PlayerController.updateAudio(
                                editIndex,
                                titleField.text,
                                artistCombo.currentText,
                                albumCombo.currentText,
                                genreCombo.currentText)

                            root.hidden = true
                        }
                    }
                    Button {
                        text: "Отменить"
                        onClicked: {

                            root.hidden = true
                        }
                    }
                }
           }
       }


        Behavior on x {
            PropertyAnimation { duration: 200; easing.type: Easing.InOutQuad }
        }


        Dialog{
            id:artistDialog
            title:"Добавление нового исполнителя"
            modal:true
            standardButtons: Dialog.Ok|Dialog.Cancel
            Column{
                spacing:10
                width:parent.width
                TextField{
                    id:artistNameField
                    placeholderText:"Введите исполнителя"
                }
            }
            onAccepted:{
                if(artistNameField.text!==""){
                    dbHandler.addArtist(artistNameField.text);
                    artistNameField.text="";
                }
            }
        }
        Dialog{
            id:albumDialog
            title:"Добавление нового альбома"
            modal:true
            standardButtons: Dialog.Ok|Dialog.Cancel
            Column{
                spacing:10
                width:parent.width
                TextField{
                    id:albumNameField
                    placeholderText:"Введите альбом"
                }
            }
            onAccepted: {
                if(albumNameField.text!==""){
                    dbHandler.addAlbum(albumNameField.text);
                    albumNameField.text=""
                }
            }
        }
        Dialog{
            id:genreDialog
            title:"Добавление нового жанра"
            modal:true
            standardButtons: Dialog.Ok|Dialog.Cancel
            Column{
                spacing:10
                width:parent.width
                TextField{
                    id:genreNameField
                    placeholderText:"Введите жанр"
                }
            }
            onAccepted: {
                if(genreNameField.text!==""){
                    dbHandler.addGenre(genreNameField.text);
                    genreNameField.text=""
                }
            }
        }
}



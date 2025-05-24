import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ListView {
    id: trackList
    spacing: 2
    clip: true

    model: ListModel {
        ListElement {
            title: "Bohemian Rhapsody"
            artist: "Queen"
            album: "A Night at the Opera"
            genre: "Rock"
            duration: "5:55"
        }
        ListElement {
            title: "Hotel California"
            artist: "Eagles"
            album: "Hotel California"
            genre: "Rock"
            duration: "6:30"
        }
        ListElement {
            title: "Sweet Child O'Mine"
            artist: "Guns N' Roses"
            album: "Appetite for Destruction"
            genre: "Rock"
            duration: "5:56"
        }
    }

    delegate: TrackDelegate {}
}

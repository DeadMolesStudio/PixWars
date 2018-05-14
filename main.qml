import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2
import pixel_model 1.0

ApplicationWindow {
    id: app
    visible: true
    width: Screen.width
    height: Screen.height
    title: "PixWars"
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    flags: Qt.Window | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint | Qt.WindowSystemMenuHint

    QPixelTable{
        id: dataModel
    }

    Flickable {
        id: flicker
        width: app.width
        height: app.height
        contentWidth: Math.sqrt(dataModel.count) * grid.cellWidth
        contentHeight: grid.contentHeight
        clip: true
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        interactive: true
        onContentYChanged: grid.contentY = contentY
        Rectangle {
            id: background
            anchors.fill: parent
            color: "white"
        }

        GridView {
            id: grid
            model: dataModel.data
            anchors.fill: parent
            cellWidth: app.width / 20
            cellHeight: grid.cellWidth
            interactive: false
            clip: true
            flickableDirection: Flickable.HorizontalAndVerticalFlick
            cacheBuffer: 1000000
            boundsBehavior: Flickable.DragAndOvershootBounds

            delegate: Item {
                   property var view: GridView.view
                   property var isCurrent: GridView.isCurrentItem
                   property var rect: rect
                   id: gridItem
                   clip: true
                   height: grid.cellHeight
                   width: grid.cellWidth

                   Rectangle {
                       id: rect
                       clip: true
                       anchors.margins: 0.5
                       anchors.fill: parent
                       color: model.color
                       radius: 0
                       border {
                           color: "black"
                           width: 0
                       }

                       MouseArea {
                           anchors.fill: parent
                           onClicked: model.blocked ? myAlert() : selected()

                           function myAlert(){
                               messageDialog.text = "Pixel unblocks in " + dataModel.unblockQpixIn(model.index)
                               messageDialog.visible = true
                           }

                           function selected(){
                               view.currentItem.rect.border.width = 0
                               view.currentIndex = model.index
                               rect.border.width = 2
                               rect.border.color = "grey"
                               popup.open()
                           }

                           MessageDialog {
                               id: messageDialog
                               title: "Pixel is blocked now"
                               onAccepted: {

                               }
                               visible: false
                           }
                       }

                       Popup {
                           id: popup
                           parent: overlay
                           width: 270
                           height: 80
                           x: (app.width - popup.width) / 2
                           y: (app.height - popup.height) / 2
                           modal: true
                           focus: true

                           GridView{
                               model: ["white", "red", "blue", "green", "black", "pink", "yellow", "cyan","white", "red", "blue", "green", "black", "pink", "yellow", "cyan"]
                               anchors.fill: parent
                               cellHeight: 30
                               cellWidth: 30

                               delegate: ItemDelegate {
                                   id: colorItem
                                   spacing: 0
                                   height: 30
                                   width: 30
                                   anchors.margins: 0

                                   Rectangle {
                                       color: modelData
                                       width: 30
                                       height: 30
                                       border {
                                          width: 1
                                          color: "lightgrey"
                                       }
                                   }
                                   MouseArea {
                                       onClicked: paint()
                                       anchors.fill: parent
                                       function paint() {
                                           dataModel.setQpixById(grid.currentIndex, modelData)
                                           popup.close()

                                       }
                                   }
                                }
                            }
                        }
                    }
                }
            }
        }
}

import QtQuick 2.9
import QtQuick.Controls 2.2
import QtQuick.Window 2.3
import QtQuick.Dialogs 1.2
import pixel_model 1.0
import 'view_config.js' as Config

ApplicationWindow {
    id: app
    visible: true
    width: Screen.width
    height: Screen.height
    title: Config.title
    minimumWidth: width
    maximumWidth: width
    minimumHeight: height
    maximumHeight: height
    flags: Qt.Window | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint | Qt.WindowSystemMenuHint

    MessageDialog {
        id: helpAlert
        title: Config.helpTextTitle
        text: Config.helpText

        onAccepted: {

        }
        visible: false
    }

    QPixelTable{
        id: dataModel
    }

    Timer{
        interval: 0
        repeat: false
        running: true
        triggeredOnStart: true
        onTriggered: trigger()
        function trigger(){
            dataModel.getField(Config.serverAdress)
            helpAlert.visible = true
        }
    }

    Timer{
        interval: Config.updateInterval
        repeat: true
        running: true
        triggeredOnStart: false
        onTriggered: dataModel.checkPixels(Config.serverAdress)
    }

    Flickable {
        id: flicker
        width: app.width
        height: app.height
        contentWidth: Math.sqrt(dataModel.count) * grid.cellWidth
        contentHeight: grid.contentHeight
        clip: true
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        interactive: true
        onContentYChanged: grid.contentY = contentY
        Rectangle {
            id: background
            anchors.fill: parent
            color: Config.pixelBorderColor //создание границ при помощи цвета фона
        }

        GridView {
            id: grid
            model: dataModel.data
            anchors.fill: parent
//            cellWidth: app.width / Config.num_pixels_width
//            cellHeight: grid.cellWidth
            cellHeight: app.height / (Math.sqrt(dataModel.count))
            cellWidth: grid.cellHeight
            interactive: false
            clip: true
            flickableDirection: Flickable.HorizontalAndVerticalFlick
            cacheBuffer: Config.cacheBuffer
            boundsBehavior: Flickable.StopAtBounds

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
                       anchors.margins: Config.pixelMargins;
                       anchors.fill: parent
                       color: Config.colorsEnum.properties[model.color].color

                       MouseArea {
                           anchors.fill: parent
                           onClicked: model.blocked ? myAlert() : selected()

                           function myAlert(){
                               messageDialog.text = Config.pixelUnblockText + dataModel.unblockQpixIn(model.index)
                               messageDialog.visible = true
                           }

                           function selected(){
                               popup.open()
                               view.currentItem.rect.border.width = 0
                               view.currentIndex = model.index
                               rect.border.width = 2
                               rect.border.color = Config.selectedBorderColor;
                           }

                           MessageDialog {
                               id: messageDialog
                               title: Config.pixelBlockedText
                               onAccepted: {

                               }
                               visible: false
                           }
                       }

                       Popup {
                           id: popup
                           parent: overlay
                           width: Config.colorPickerWidth
                           height: Config.colorPickerHeight
                           x: (app.width - popup.width) / 2
                           y: (app.height - popup.height) / 2
                           modal: true
                           focus: true

                           GridView{
                               model: Config.colors
                               anchors.fill: parent
                               cellHeight: Config.colorPickCellSize
                               cellWidth: Config.colorPickCellSize
                               boundsBehavior: Flickable.StopAtBounds


                               delegate: ItemDelegate {
                                   id: colorItem
                                   spacing: 0
                                   height: Config.colorPickCellSize
                                   width: Config.colorPickCellSize
                                   anchors.margins: 0

                                   Rectangle {
                                       color: Config.colorsEnum.properties[modelData].color
                                       width: Config.colorPickCellSize
                                       height: Config.colorPickCellSize
                                       border {
                                          width: 1
                                          color: Config.colorPickerBorderColor
                                       }
                                   }
                                   MouseArea {
                                       onClicked: paint()
                                       anchors.fill: parent
                                       function paint() {
                                           popup.close()
                                           var temp = dataModel.setQpixById(Config.serverAdress,
                                                                            grid.currentIndex,
                                                                            Config.colorsEnum.properties[modelData].number);
                                           if(!temp) failAlert();
                                       }

                                       function failAlert(){
                                           messageDialog.text = Config.failText + dataModel.unblockQpixIn(model.index)
                                           messageDialog.visible = true
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

import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QMLPlot 1.0
Window {
    width: 500
    height: 500
    visible: true

    Timer {
        property int numPoints: 0
        interval: 1
        repeat: true
        running: true
        onTriggered: {
            var x = numPoints*Math.PI/100
            dataSource1.addPoint(x,Math.sin(x))
            dataSource2.addPoint(x,Math.cos(x))
            numPoints++
        }
    }
    ColumnLayout {
        anchors.fill: parent

        Figure {
            id: figure
            Layout.fillWidth: true
            Layout.fillHeight: true
            //color: "red"
            fitData: true
            xLabel: "x"
            yLabel: "f(x)"
            title: "Plot of sin(x)"
            LineGraph {
                id: graph1
                dataSource: LineGraphDataSource {
                    id: dataSource1
                }
                width: 2
                style: Qt.DotLine
            }
            LineGraph {
                id: graph2
                dataSource: LineGraphDataSource {
                    id: dataSource2
                }
                width: 1
                color: "red"
            }
        }

        Row {
            Button {
                text: "Save SVG"
                onClicked: {
                    fileDialog.mode = "svg"
                    fileDialog.open()
                }
            }
            Button {
                text: "Save PNG"
                onClicked: {
                    fileDialog.mode = "png"
                    fileDialog.open()
                }
            }
        }
    }

    FileDialog {
        id: fileDialog
        property string mode
        title: "Please choose a file"
        // folder: shortcuts.home
        selectMultiple: false
        selectExisting: false
        selectFolder: false
        onAccepted: {
            if(mode === "png") {
                figure.savePNG(fileDialog.fileUrls)
            } else if (mode === "svg") {
                figure.saveSVG(fileDialog.fileUrls)
            }
        }
    }
}


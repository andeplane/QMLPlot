import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QMLPlot 1.0
Window {
    width: 500
    height: 500
    visible: true

    Timer {
        property int numPoints: 0
        interval: 10
        repeat: true
        running: true
        onTriggered: {
            var x = numPoints*Math.PI/100
            var y = Math.sin(x)
            dataSource.addPoint(x,y)
            figure.xMin = 0
            figure.xMax = 2*Math.PI
             figure.xMax = x
             figure.xMin = (numPoints-500)*Math.PI/100
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
            xMin: 0
            xMax: 10
            yMin: slider.value
            yMax: 1
            fitData: true
            xLabel: "t [s] "
            yLabel: "T [K]"
            title: "Temperature"
            LineGraph {
                id: graph
                dataSource: dataSource
                width: 2
                // style: Qt.DotLine
            }
        }

        Row {
            Slider {
                id: slider
                value: figure.yMin
                minimumValue: -2
                maximumValue: 2
            }
        }
    }

    LineGraphDataSource {
        id: dataSource
    }
}


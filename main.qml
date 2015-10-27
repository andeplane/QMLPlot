import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
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
            var y = Math.sin(x)
            dataSource.addPoint(x,y)
            figure.xMax = x
            figure.xMin = (numPoints-500)*Math.PI/100
            numPoints++
        }
    }

    Rectangle {
        id: rect
        width: 250
        height: 80
        anchors.top: parent.top
        anchors.right: parent.right
    }
    Column {
        anchors.right: parent.right
        anchors.top: rect.bottom
        Figure {
            id: figure
            width: 250
            height: 300
            xMin: 0
            xMax: 10
            yMin: -1
            yMax: 1
            xLabel: "t [s] "
            yLabel: "T [K]"
            title: "Temperature"
            LineGraph {
                id: graph
                dataSource: dataSource
            }
        }
        Slider {
            value: figure.yMax
            onValueChanged: {
                figure.yMax = value
            }
        }
    }

    LineGraphDataSource {
        id: dataSource
    }
}


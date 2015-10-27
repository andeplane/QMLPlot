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
            graph.addPoint(x,y)
            figure.update()
            figure.xMax = x
            figure.xMin = (numPoints-500)*Math.PI/100
            numPoints++
        }
    }

    Figure {
        id: figure
        width: parent.width
        height: parent.height-50
        xMin: 0
        xMax: 10
        yMin: -1
        yMax: 1
        LineGraph {
            id: graph
        }
    }
}


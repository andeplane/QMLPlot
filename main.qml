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
    Figure {
        id: figure
        anchors.fill: parent
        //color: "red"
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
            width: 2
            // style: Qt.DotLine
        }
    }

    LineGraphDataSource {
        id: dataSource
    }
}


const numFrames = 100



var measurementBuffer = []
var data1 = []
var dataset1 = []


document.addEventListener("DOMContentLoaded", function() {
    
    
    var chart1 = new Dygraph(document.getElementById("chart1"), 

    dataset1,
    {
      title: "Motor current",
      drawPoints: true,
      valueRange: [0, 8192],
      labels: ['Time', 'Current'],
      showRoller: false,
      colors: ['#ff0000'],
      gridLineColor: ['#DDDDDD'],
      axisLineColor: ['#FFFFFF'],
      axisLineWidth : 1,
      width: (document.querySelector(".right").getBoundingClientRect().width - 10), 
      height: document.querySelector(".right").getBoundingClientRect().height /4 - 10,
      
      ylabel: 'Value',
      xlabel: 'Time [s]',
    });
    
    
    
    const interval = setInterval(() => {
        data1.push(...measurementBuffer.slice(0, 10).map(obj => ({
            timestamp: obj.timestamp,
            current_Total: obj.current_Total
          })))
        measurementBuffer.splice(0, 11)
        measurementBuffer.splice(numFrames + 10)
        data1.splice(0, data1.length - 3000); // Remove the first elements to keep the array length
        for (let i = 0; i < data1.length; i++) {
            dataset1[i] = [(data1[data1.length-1].timestamp - data1[i].timestamp)/(-1000), data1[i].current_Total]
        }

        chart1.updateOptions({
            'file': dataset1
        })
        
    }, 10);
});


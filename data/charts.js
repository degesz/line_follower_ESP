var fixedXAxis = {}

var data1 = []
var dataset1 = []


document.addEventListener("DOMContentLoaded", function() {
    
    
    var chart1 = new Dygraph(document.getElementById("chart1"), dataset1,
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
        data1.splice(0, data1.length - 50); // Remove the first elements to keep the array length at 200
        for (let i = 0; i < data1.length; i++) {
            dataset1[i] = [(data1[data1.length-1][0] - data1[i][0])/(-1000), data1[i][1]]
        }

        chart1.updateOptions({
            'file': dataset1
        })
        
    }, 16);
});


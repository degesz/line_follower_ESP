const numFrames = 100



var measurementBuffer = []
var data0 = []
var dataset0 = []
var data1 = []
var dataset1 = []


document.addEventListener("DOMContentLoaded", function() {
    
    var chart0 = new Dygraph(document.getElementById("chart0"), //////////// CURRENT CHART
    dataset0,
    {
      title: "PID Control",
      drawPoints: true,
      //valueRange: [0, 8192],
      labels: ['Time', 'Setpoint', 'Input', 'Output'],
      showRoller: false,
      showLabelsOnHighlight: false,
      colors: ['#125aae', '#e50000', '#52b001'],
      gridLineColor: ['#DDDDDD'],
      axisLineColor: ['#FFFFFF'],
      axisLineWidth : 1,
      width: (document.querySelector(".right").getBoundingClientRect().width - 10), 
      height: document.querySelector(".right").getBoundingClientRect().height /2 - 10,
      
      ylabel: 'Value',
      xlabel: 'Time [s]',
    });
    
    
    var chart1 = new Dygraph(document.getElementById("chart1"), //////////// CURRENT CHART
    dataset1,
    {
      title: "Motor current",
      drawPoints: true,
      valueRange: [0, 8192],
      labels: ['Time', 'Current L', 'Current R'],
      showRoller: false,
      showLabelsOnHighlight: false,
      colors: ['#ffd93e', '#02ce3f'],
      gridLineColor: ['#DDDDDD'],
      axisLineColor: ['#FFFFFF'],
      axisLineWidth : 1,
      width: (document.querySelector(".right").getBoundingClientRect().width - 10), 
      height: document.querySelector(".right").getBoundingClientRect().height /4 - 10,
      
      ylabel: 'Value',
      xlabel: 'Time [s]',
    });
    
    


  

     const fillChart0 = setInterval(() => {

       data0.push(...measurementBuffer.slice(0, 40).map(obj => ({
           timestamp: obj.timestamp,
           setpoint: obj.setpoint / 10.0,
           input: obj.input / 10.0,
           output: obj.output / 10.0
         })))
     data0.splice(0, data0.length - 3000); // Remove the first elements to keep the array length
     for (let i = 0; i < data0.length; i++) {
         dataset0[i] = [(data0[data0.length-1].timestamp - data0[i].timestamp)/(-1000), data0[i].setpoint, data0[i].input, data0[i].output]
     }
   
     chart0.updateOptions({
         'file': dataset0
       })
   }, 40);

    
    const fillChart1 = setInterval(() => {

        data1.push(...measurementBuffer.slice(0, 40).map(obj => ({
            timestamp: obj.timestamp,
            current_L: obj.current_L,
            current_R: obj.current_R
          })))
       measurementBuffer.splice(0, 41)
       measurementBuffer.splice(numFrames + 40)
        data1.splice(0, data1.length - 3000); // Remove the first elements to keep the array length
        for (let i = 0; i < data1.length; i++) {
            dataset1[i] = [(data1[data1.length-1].timestamp - data1[i].timestamp)/(-1000), data1[i].current_L, data1[i].current_R]
        }

        chart1.updateOptions({
            'file': dataset1
        })
        
    }, 40);
});


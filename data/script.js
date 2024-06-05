var gateway = `ws://${window.location.hostname}/ws`;
var ws;

window.addEventListener('load', onLoad);

function onLoad(event) {
    initWebSocket();
}

function initWebSocket() {
    if (gateway == "ws:///ws") {
        console.error("Local file in browser. not connecting websocket");
        return
    }
    ws = new WebSocket(gateway);
    ws.onopen = onOpen;
    ws.onclose = onClose;
    ws.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function onMessage(event) {
    console.log(event.data);
    document.getElementById('message').innerHTML = event.data;
}



window.onload = function() {

    var canvas = document.getElementById("canvas");
    canvas.width = document.querySelector(".middle").getBoundingClientRect().width - 10
    canvas.height = document.querySelector(".middle").getBoundingClientRect().height - 10

    // Get the sliders container and all input elements within it
    const slidersContainer = document.getElementById('sliders');
    const sliders = slidersContainer.getElementsByTagName('input');



    // Add event listeners to each slider
    for (let slider of sliders) {
        slider.addEventListener('mouseup', () => sendSliderValue());
    }
};

    // Function to send slider value via WebSocket
    function sendSliderValue() {
        if (ws.readyState === WebSocket.OPEN) {
            sliderData = {
                name: "color sliders",
              
            };
            sliderData.red = document.getElementById("redSlider").value;
            sliderData.green = document.getElementById("greenSlider").value;
            sliderData.blue = document.getElementById("blueSlider").value;
            
            
            ws.send(JSON.stringify(sliderData));
        }
    }


document.querySelectorAll('.slider').forEach(slider => {
    slider.addEventListener('input', (event) => {
        var valueDisplay = event.target.parentElement.querySelector('.slider-value');
        valueDisplay.textContent = event.target.value;
    });
});

function updateSlider(slider){
    slider.parentElement.querySelector('.slider-value').textContent = slider.value
}
var gateway = `ws://${window.location.hostname}/ws`;
var ws;
var autosaveActive = false;
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
    if (typeof event.data === 'string') {
        try {
            var msg = JSON.parse(event.data)
            if (msg.type == "ping") {
                var pingTime = Date.now() - msg.sendTime;
                if (pingTime < 200) {
                    document.getElementById("connection-status").classList.remove("disconnected")
                    document.getElementById("connection-status").classList.add("connected")
                    document.getElementById("connection-status").innerHTML = 'OK &ThickSpace; Ping: <span id="ping"> </span> ms'
                    document.getElementById("ping").innerText = pingTime
                }
                else{
                    document.getElementById("connection-status").classList.add("disconnected")
                    document.getElementById("connection-status").classList.remove("connected")
                    document.getElementById("connection-status").innerHTML = 'Disconnected'
                }
            } 

            else if (msg.type == "params"){
                var configContainer = document.getElementById("config-panel")
                configContainer.querySelectorAll('.config-slider')[0].value = msg.P0 / 10.0
                configContainer.querySelectorAll('.config-slider')[1].value = msg.P1 / 10.0
                configContainer.querySelectorAll('.config-slider')[2].value = msg.P2 / 10.0
                configContainer.querySelectorAll('.config-slider')[3].value = msg.P3 / 10.0
                updateSlider(configContainer.querySelectorAll('.config-slider')[0])
                updateSlider(configContainer.querySelectorAll('.config-slider')[1])
                updateSlider(configContainer.querySelectorAll('.config-slider')[2])
                updateSlider(configContainer.querySelectorAll('.config-slider')[3])
            }

            else{
                console.log("recieved message: ");
                console.log(msg)
                console.log(event.data)
            }
        } catch (e) {
            console.log('Received text data:', event.data);
        }
    } 
    else {
        //console.log('Received unknown type of data:', event.data);
        var reader = new FileReader();
        reader.onload = function() {
          //  console.log(reader.result)
            //console.log(parseMeasurements(reader.result))
            measurementBuffer.push(...parseMeasurements(reader.result))
        }
        reader.readAsArrayBuffer(event.data)
    }

}

setInterval(function() {        //Send ping messages
    ws.send(JSON.stringify({
        type: "ping",
        sendTime: Date.now(),
    }))

    if (autosaveActive) {
        saveConfig(document.getElementById("config-panel"))
    }

  }, 500);


window.onload = function() {

    var canvas = document.getElementById("map-canvas");
    canvas.width = document.querySelector(".middle").getBoundingClientRect().width - 10
    canvas.height = document.querySelector(".middle").getBoundingClientRect().height - 10


};

function parseMeasurements(buffer) {
    const measurements = [];
    const dataView = new DataView(buffer);
    const structSize = 2 * 6 + 4 * 3; // 6 int16_t (2 bytes each) + 3 uint32_t (4 bytes each)
    const numMeasurements = numFrames;

    for (let i = 0; i < numMeasurements; i++) {
        const offset = i * structSize;
        const measurement = {
            current_L: dataView.getInt16(offset, true), // true for little-endian
            current_R: dataView.getInt16(offset + 2, true),
            current_Total: dataView.getInt16(offset + 4, true),
            setpoint: dataView.getInt16(offset + 6, true),
            input: dataView.getInt16(offset + 8, true),
            encoder_L: dataView.getUint32(offset + 10, true),
            encoder_R: dataView.getUint32(offset + 14, true),
            timestamp: dataView.getUint32(offset + 18, true),
            output: dataView.getInt16(offset + 22, true),
        };
        measurements.push(measurement);
    }

    return measurements;
}


function toggleAutosave(button) {
    button.classList.toggle('forced-active')
    button.classList.toggle('config-button')
    autosaveActive = !autosaveActive
}
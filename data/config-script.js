function updateSlider(slider){
    slider.parentElement.querySelector('.slider-value').textContent = slider.value

}

function saveConfig(configContainer) {
   if (ws.readyState === WebSocket.OPEN) {
       msg = {
           type: "config-update",
           P0: configContainer.querySelectorAll('.config-slider')[0].value,
           P1: configContainer.querySelectorAll('.config-slider')[1].value,
           P2: configContainer.querySelectorAll('.config-slider')[2].value,
           P3: configContainer.querySelectorAll('.config-slider')[3].value,
           P4: configContainer.querySelectorAll('.config-slider')[4].value,

       };

       ws.send(JSON.stringify(msg));
   }
   else{alert("websocket error")}
}

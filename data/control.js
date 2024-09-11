var controlActive = false;

function toggleControl(button) {
    button.classList.toggle('forced-active')
    button.classList.toggle('config-button')
    controlActive = !controlActive
    if (controlActive) {
        ws.send(JSON.stringify({type: 'MANUAL_CONTROL'}))
    }
    else{
        ws.send(JSON.stringify({type: 'AUTO_CONTROL'}))
    }
}

setInterval(() => {
    sendControlData();
  }, 100);


function sendControlData() {
    if (controlActive) {
        speed = document.getElementById('manual-speed').value;

        ws.send(JSON.stringify({
            type: 'CONTROL_DATA',
            joyX: joyX,
            joyY: joyY,
            manualSpeed: speed 
        }))
    }
}

var joyX, joyY;
var speed;

document.addEventListener('keydown', function(event) {
    if (controlActive) {
        switch(event.key) {
            case 'w':
                joyY = 100; // Move up to max
                break;
            case 's':
                joyY = -100; // Move down to min
                break;
            case 'a':
                joyX = -100; // Move left to min
                break;
            case 'd':
                joyX = 100; // Move right to max
                break;
        }
        document.getElementById('joyX').innerText = joyX
        document.getElementById('joyY').innerText = joyY
        sendControlData();
    }

});

document.addEventListener('keyup', function(event) {
    if (controlActive) {
        switch(event.key) {
            case 'w':
            case 's':
                joyY = 0; // Reset Y axis
                break;
            case 'a':
            case 'd':
                joyX = 0; // Reset X axis
                break;
        }
        document.getElementById('joyX').innerText = joyX
        document.getElementById('joyY').innerText = joyY
        sendControlData();
    }

});

document.addEventListener('DOMContentLoaded', function() {

    var joy = new JoyStick('joyDiv', {internalFillColor: '#AAAA00'}, function(stickData) {
        joyX = stickData.x;
        joyY = stickData.y;

        if (controlActive) {
            document.getElementById('joyX').innerText = joyX
            document.getElementById('joyY').innerText = joyY
        }

    });
});

//document.addEventListener('DOMContentLoaded', function() {
//    const joystick = document.getElementById('joystick');
//    const container = document.getElementById('joystick-container');
//    const maxDistance = container.clientWidth / 2 - joystick.clientWidth / 2;
//
//    let startX = 0, startY = 0;
//    let isDragging = false;
//
//    offsetX = joystick.getBoundingClientRect().left
//    offsetY = joystick.getBoundingClientRect().top
//
//    joystick.addEventListener('mousedown', function(e) {
//        //console.log("mousedown")
//        startX = e.clientX - joystick.offsetLeft;
//        startY = e.clientY - joystick.offsetTop;
//        isDragging = true;
//    });
//
//    document.addEventListener('mousemove', function(e) {
//        if (isDragging && controlActive) {
//            let bounds = container.getBoundingClientRect();
//            const moveX = e.clientX - startX;
//            const moveY = e.clientY - startY;
//
//           // const distance = Math.sqrt((moveX + bounds.width/2) * (moveX + bounds.width/2) + (moveY + bounds.height/2) * (moveY + bounds.height/2));
//           // console.log(distance)
//            if (//e.clientX >= bounds.left &&
//                //e.clientX <= bounds.right &&
//                //e.clientY >= bounds.top &&
//                //e.clientY <= bounds.bottom
//                joyY > -100 &&
//                joyY < 100
//                ) {
//                joystick.style.top = moveY + 'px';
//                
//            }
//            if (//e.clientX >= bounds.left &&
//                //e.clientX <= bounds.right &&
//                //e.clientY >= bounds.top &&
//                //e.clientY <= bounds.bottom
//                joyX > -100 &&
//                joyX < 100 
//                ) {
//                joystick.style.left = moveX + 'px';
//            }
//
//            joyX = joystick.getBoundingClientRect().left - offsetX
//            joyY = joystick.getBoundingClientRect().top  - offsetY
//
//            document.getElementById('joyX').innerText = joyX
//            document.getElementById('joyY').innerText = joyY
//
//          //  if (joyX > 100) {
//          //      joystick.style.left = offsetX + 60;
//          //  }
//          //  if (joyX < -100) {
//          //      joystick.style.left = offsetX - 60;
//          //  }
//          //  if (joyY > 100) {
//          //      joystick.style.top = offsetY + 60;
//          //  }
//          //  if (joyY < -100) {-5      
//          //      joystick.style.top = offsetY - 60;
//          //  }
//
//        }
//    });
//      
//    
//
//    document.addEventListener('mouseup', function() {
//        if (isDragging) {
//            joystick.style.left = '50%';
//            joystick.style.top = '50%';
//            isDragging = false;
//            joyX = 0;
//            joyY = 0;
//        }
//    });
//});


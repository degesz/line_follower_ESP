

window.onload = function() {
const canvas = document.getElementById('map-canvas');
const ctx = canvas.getContext('2d');

canvas.width =  2000
canvas.height = 2000
ctx.imageSmoothingEnabled = false;


// Initial transformation values
let scale = 1;
let offsetX = 0;
let offsetY = 0;
// Variables to handle panning
let startX, startY;
let isPanning = false;

  const robot_img = new Image();
  robot_img.src = 'graphics/robot_icon.svg';
  
  robot_img.onload = function() {
      draw()

    }

    var i = 0;
    setInterval(() => {
        i++
        draw(i) ;
    }, 16);



    function draw() {
      ctx.save();
      ctx.clearRect(0, 0, canvas.width, canvas.height);
  
      // Apply transformations
      ctx.translate(offsetX, offsetY);
      ctx.scale(scale, scale);

      drawRobot(500, 500, i)
      ctx.restore();

        
        
    }

    function drawRobot(posX, posY, angle, state){
      var pivotY = 60  ;
      ctx.save();
      ctx.translate(posX, posY);
      ctx.translate(0, pivotY)   /// change pivot to point between wheels
      ctx.rotate(angle* (Math.PI / 180));
      ctx.translate(0, -pivotY)   /// change pivot back
      ctx.scale(0.2, 0.2);
      ctx.drawImage(robot_img, -robot_img.width / 2, -robot_img.height / 2);// Draw the image, centering it at the position
      
      ctx.restore();// Restore the canvas to its original state
    }
  


    // Mouse wheel event for zooming
canvas.addEventListener('wheel', (event) => {
  event.preventDefault();

  const zoomFactor = 0.1;
  const zoomIn = event.deltaY < 0;

  // Get mouse position relative to canvas
  const mouseX = event.offsetX;
  const mouseY = event.offsetY;

  // Adjust scale and offset
  const newScale = zoomIn ? scale * (1 + zoomFactor) : scale * (1 - zoomFactor);
  offsetX -= mouseX / scale - mouseX / newScale;
  offsetY -= mouseY / scale - mouseY / newScale;
  scale = newScale;

  draw();
});

// Mouse down event for starting panning
canvas.addEventListener('mousedown', (event) => {
  isPanning = true;
  startX = event.clientX - offsetX;
  startY = event.clientY - offsetY;
});

// Mouse move event for panning
canvas.addEventListener('mousemove', (event) => {
  if (!isPanning) return;

  offsetX = event.clientX - startX;
  offsetY = event.clientY - startY;

  draw();
});

// Mouse up event for ending panning
canvas.addEventListener('mouseup', () => {
  isPanning = false;
});

// Mouse leave event for ending panning
canvas.addEventListener('mouseleave', () => {
  isPanning = false;
});



};

var enc_R;
var enc_L;

function updateEncoders(measurement_frame) {
  var diff_R = measurement_frame.encoder_R - enc_R
  var diff_L = measurement_frame.encoder_L - enc_L

  enc_R = measurement_frame.encoder_R
  enc_L = measurement_frame.encoder_L

  if (Math.abs(diff_L) < 5  &&  Math.abs(diff_R) < 5) {
    return;  // if change in encoder is below noise floor, do nothing
  }
  findPivot(diff_R, diff_L)
  //console.log('DIFF:   R: ' + diff_R + '    L: ' + diff_L)
}

function findPivot(diff_R, diff_L) {
  // convert encoder increments to mm
  // 4096 = 1 magnet rev = 2.13 wheel rev       2.13 * 122 = ~260mm     measure more precisely!!!
  //260 / 4096 = 0.06348 mm per encoder increment

  var dist_R = diff_R * 0.06348
  var dist_L = diff_L * 0.06348

  var radius_R = Math.abs(dist_R / 6.283185)  //2pi, cant have negative radius
  var radius_L = Math.abs(dist_L / 6.283185)  

  if (diff_L == diff_R) {   // going straight
    
  }
  else if (diff_L >= -5 && diff_R >= -5 && diff_L > diff_R) {  // pivot right of wheels, going forward
    console.log('right of wheels, going forward')
  }
  else if (diff_L >= -5 && diff_R >= -5 && diff_L < diff_R) { // pivot left of wheels, going forward
    console.log('left of wheels, going forward')
  }
  else if (diff_L <= 5 && diff_R <= 5 && diff_L < diff_R) {  // pivot right of wheels, going backward
    console.log('right of wheels, going backward')
  }
  else if (diff_L <= 5 && diff_R <= 5 && diff_L > diff_R) { // pivot left of wheels, going backward
    console.log('left of wheels, going back')
  }
//  else if (diff_L >= -5 && diff_R <= 5 && Math.abs(diff_L) > Math.abs(diff_R)) {  // pivot between wheels on right side, going forward
//    console.log('between wheels on right side, going forward')
//  }
//  else if (diff_L <= 5 && diff_R >= -5 && Math.abs(diff_L) < Math.abs(diff_R)) { // pivot between wheels on left side, going backward
//    console.log('between wheels on left side, going backward')
//  }
//  else if (diff_L <= 5 && diff_R >= -5 && Math.abs(diff_L) > Math.abs(diff_R)) { // pivot between wheels on right side, going backward
//    console.log('between wheels on right side, going backward')
//  }
//  else if (diff_L <= 5 && diff_R >= -5 && Math.abs(diff_L) > Math.abs(diff_R)) { // pivot between wheels on left side, going forward
//    console.log('between wheels on left side, going forward')
//  }
}

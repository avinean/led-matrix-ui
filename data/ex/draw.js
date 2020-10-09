/** draw.js **/

// Create an image object. This is not attached to the DOM and is not part of the page.
var image = new Image();
var canvas;
var context;
var imgData;

var OUT_WIDTH = 0;
var OUT_HEIGTH = 0;
var OUT_DEPTH = 3;

var OUT_SRC = "";

function submitForm(oFormElement)
{
    document.getElementById("width").value = OUT_WIDTH;
    document.getElementById("height").value = OUT_HEIGTH;
    document.getElementById("depth").value = OUT_DEPTH;
    document.getElementById("data").value = OUT_SRC;

    var xhr = new XMLHttpRequest();
    xhr.onload = function(){ alert (xhr.responseText); } // success case
    xhr.onerror = function(){ alert (xhr.responseText); } // failure case
    xhr.open (oFormElement.method, oFormElement.action, true);
    xhr.send (new FormData (oFormElement));
    return false;
}

function getSrc(){
    var idx = 0;

    // var d = [];
    OUT_SRC = "";

    for ( var y = 0; y < OUT_HEIGTH; ++y ) {
        for( var x = 0; x < OUT_WIDTH; ++x ) {
            var dta = getPixel(imgData, x, y );
            // OUT_SRC += `\n\t${dta.r}, ${dta.g}, ${dta.b}, /*Color of index ${idx}*/`;
            //var index = (x + y * OUT_WIDTH) * 4;
            OUT_SRC += `${dta.r},${dta.g},${dta.b}${ (( y == OUT_HEIGTH-1 ) && ( x == OUT_WIDTH - 1 )) ? "" : "," }`;
            // d.push(dta);
            // console.log(`Data[${idx}] : x = ${x}, y = ${y}, data = [${JSON.stringify(dta)} ]`);
            idx++;
        }
        // idx++;
    }

    var text = `const unsigned char PROGMEM bitmap[] = {${OUT_SRC}\n}`;
    // var text = `{
    //     "width": ${OUT_WIDTH},
    //     "height": ${OUT_HEIGTH},
    //     "data": ${JSON.stringify(d)}
    // }`;

    console.log(`SRC: \n ${text}`);
}

function createTable(w,h){
    // imgData = context.getImageData(0, 0, OUT_WIDTH, OUT_HEIGTH);
    for ( var row=0; row < w; row++ ) {
        var x=document.getElementById('myTable').insertRow(row);
        for ( var col=0; col < h; col++ ) {
            var y=  x.insertCell(col);
            var dta = getPixel(imgData, col, row );
            // console.log(`Data[${idx}] : x = ${x}, y = ${y}, data = [${dta} ]`);
            y.style.backgroundColor = getHtmlColor(dta);
            // y.innerHTML=JSON.stringify(dta);//"Row-"+row+" Column-"+col;
            // y.innerHTML="Row-"+row+" Column-"+col;
        }
    }
}

function createImage(width, height) {
    var wrapper = document.querySelector('#parsedImg');
    var wrapperContent = '';
    var widthList = Array(width).fill().map((i, j) => j);
    var heightList = Array(height).fill().map((i, j) => j);

    wrapper.innerHTML = widthList.map(x => `<div class="picture__row">
        ${
            heightList.map(y => {
                var pixel = getPixel(imgData, y, x );
                var color = getHtmlColor(pixel);
                return `<div
                    class="picture__column"
                    style="background-color: ${color};"
                ></div>`;
            }).join('')
        }
    </div>`).join('');
}




window.addEventListener("DOMContentLoaded", function(){
    // Get a reference to the file select input field
    var fileChooser = document.getElementById('fileChooser');
    // When a selection is made the "change" event will be fired
    fileChooser.addEventListener('change', handleFileSelect, false);
});

function handleFileSelect(event){
    // Get the FileList object from the file select event
    var files = event.target.files;
    // Check if there are files in the FileList
    if(files.length === 0)
        return;
    // For this example we only want one image. We'll take the first.
    var file = files[0];
    // Check that the file is an image
    if(file.type !== '' && !file.type.match('image.*'))
        return;
    // The URL API is vendor prefixed in Chrome
    window.URL = window.URL || window.webkitURL;
    // Create a data URL from the image file
    var imageURL = window.URL.createObjectURL(file);
    loadAndDrawImage(imageURL);
}

function loadAndDrawImage(url){
    // Create an image object. This is not attached to the DOM and is not part of the page.
    image = new Image();
    // When the image has loaded, draw it to the canvas
    image.onload = function()    {
        // draw image...
        drawImage();
        createImage(OUT_WIDTH, OUT_HEIGTH);
    }
    // Now set the source of the image that we want to load
    image.src = url;
}



function drawImage(){
    // var canvas = document.createElement("canvas");
    // document.body.appendChild(canvas);
    canvas = document.getElementById('canvas')

    context = canvas.getContext("2d");

    OUT_WIDTH = image.width;
    OUT_HEIGTH = image.height;

    canvas.width  = OUT_WIDTH;
    canvas.height = OUT_HEIGTH;

    context.drawImage(image, 0, 0);

    imgData = context.getImageData(0, 0, OUT_WIDTH, OUT_HEIGTH);

    // context.scale(10, 10);
    //
    // context.drawImage(img, 0, 0);
}

function getHtmlColor(dta) {
    var r = dta.r;//Math.floor(Math.random() * 255);
    var g = dta.g;//Math.floor(Math.random() * 255);
    var b = dta.b;//Math.floor(Math.random() * 255);
    return "rgb(" + r + "," + g + "," + b + ")";
}

function color(r,g,b,a){
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = a;
}

function getPixel(imageData, x, y) {
    var index = (x + y * imageData.width) * 4;
    return new color(imageData.data[index+0], imageData.data[index+1], imageData.data[index+2], imageData.data[index+3]);
}
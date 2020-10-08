var isMouseDown = false;
var currentColor = new COLOR(0, 0, 255, 255);

function sendData( data ) {
    console.log( 'Sending data' );

    const XHR = new XMLHttpRequest();

    let urlEncodedData = "",
        urlEncodedDataPairs = [],
        name;

    // Turn the data object into an array of URL-encoded key/value pairs.
    for( name in data ) {
        urlEncodedDataPairs.push( encodeURIComponent( name ) + '=' + encodeURIComponent( data[name] ) );
    }

    // Combine the pairs into a single string and replace all %-encoded spaces to
    // the '+' character; matches the behaviour of browser form submissions.
    urlEncodedData = urlEncodedDataPairs.join( '&' ).replace( /%20/g, '+' );

    // Define what happens on successful data submission
    XHR.addEventListener( 'load', function(event) {
        console.log( 'Yeah! Data sent and response loaded.' );
    } );

    // Define what happens in case of error
    XHR.addEventListener( 'error', function(event) {
        console.log( 'Oops! Something went wrong.' );
    } );

    // Set up our request
    XHR.open( 'POST', '/pixel' );

    // Add the required HTTP header for form data POST requests
    XHR.setRequestHeader( 'Content-Type', 'application/x-www-form-urlencoded' );

    // Finally, send our data.
    XHR.send( urlEncodedData );
}

function createTable(w,h){
    // imgData = context.getImageData(0, 0, OUT_WIDTH, OUT_HEIGTH);
    for ( var row=0; row < w; row++ ) {
        var x=document.getElementById('myTable').insertRow(row);
        for ( var col=0; col < h; col++ ) {
            var y=  x.insertCell(col);
            // var dta = getPixel(imgData, col, row );
            // console.log(`Data[${idx}] : x = ${x}, y = ${y}, data = [${dta} ]`);
            // y.style.backgroundColor = getHtmlColor(dta);
            // y.innerHTML=JSON.stringify(dta);//"Row-"+row+" Column-"+col;
            // y.innerHTML = row + " : " + col;
            y.addEventListener("mouseover", func, false);
        }
    }
}

function getHtmlColor(dta) {
    var r = dta.r;//Math.floor(Math.random() * 255);
    var g = dta.g;//Math.floor(Math.random() * 255);
    var b = dta.b;//Math.floor(Math.random() * 255);
    return "rgb(" + r + "," + g + "," + b + ")";
}

function COLOR(r,g,b,a){
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = a;
}

function func()
{  // not needed since item is already global,
   // I am assuming this is here just because it's sample code?
   // var item = document.getElementById("button");
    if ( isMouseDown == true ) {
        // this.setAttribute("style", "background-color:blue;");
        this.style.background = getHtmlColor(currentColor);
        var cellIndex  = this.cellIndex;
        var rowIndex = this.parentNode.rowIndex;
        // alert("cell: " + cellIndex + " / row: " + rowIndex );
        sendData( {x:cellIndex, y:rowIndex, r:currentColor.r, g:currentColor.g, b:currentColor.b} );
    }
}

window.addEventListener("DOMContentLoaded", function(){
    document.addEventListener('mousedown', function(event) {
        if ( event.which ) isMouseDown = true;
    }, true);

    document.addEventListener('mouseup', function(event) {
        if ( event.which ) isMouseDown = false;
    }, true);
    createTable(16,16);
});
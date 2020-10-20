class Services {

  fillMatrix(body) {   
    return fetch('/fill-matrix', {  
      method: 'POST',  
      headers: {  
        'Content-Type': 'application/json',
      },  
      body: JSON.stringify(body),
    })
  }

  clearImgData({ width, height }) {
    const byteArray = new Uint8Array(width * height * 3);
    byteArray.forEach((i, j) => byteArray[j] = 0);

    const body = new Blob([byteArray], {type: "octet/stream"});
    
    return this.sendImgData(body);
  }

  sendImgData(body) {    
    return fetch('/draw', {  
      method: 'POST',  
      headers: {  
        'Content-Type': 'application/octet-stream',
      },  
      body,
    }).then(() => {
      observer.emit(IMG_UPLOADED);
    });
  }

  getMatrixState() {
    return fetch('/matrix-state').then(response => {
      const reader = response.body.getReader();
      return reader.read();
    });
  }

  getMatrixParameters() {
    return fetch('/matrix-parameters').then(res => res.json());
  }

  drawSinglePixel(params) {
    fetch('/pixel', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  setRunningText(params) {
    fetch('/running-text', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  setClock(params) {
    fetch('/clock', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  playGame(params) {
    return fetch('/games', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  control(action) {
    fetch('/' + action, { method: 'POST',});
  }

}

const services = new Services();
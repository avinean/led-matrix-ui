import { observer, IMG_UPLOADED } from './observer';

const BASE_URI = location.host ===  "localhost:3000" ? 'http://localhost:2832' : '';

class Services {

  fillMatrix(body) {   
    return fetch(BASE_URI + '/fill-matrix', {  
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
    return fetch(BASE_URI + '/draw', {  
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
    return fetch(BASE_URI + '/matrix-state').then(response => {
      const reader = response.body.getReader();
      return reader.read();
    });
  }

  getMatrixParameters() {
    return fetch(BASE_URI + '/matrix-parameters').then(res => res.json());
  }

  drawSinglePixel(params) {
    fetch(BASE_URI + '/pixel', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  setRunningText(params) {
    fetch(BASE_URI + '/running-text', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  setClock(params) {
    fetch(BASE_URI + '/clock', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  playGame(params) {
    return fetch(BASE_URI + '/games', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  control(action) {
    if (!action) return;
    fetch(BASE_URI + '/' + action, { method: 'POST',});
  }

}

export default new Services();
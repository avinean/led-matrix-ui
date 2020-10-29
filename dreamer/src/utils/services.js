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
    return fetch(BASE_URI + '/pixel', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  getImagesFromGallery() {
    return fetch('https://dreamer-led.000webhostapp.com').then(res => res.json());
  }

  setRunningText(params) {
    return fetch(BASE_URI + '/running-text', {
      method: 'POST',
      body: JSON.stringify(params),
    });
  }

  setClock(params) {
    return fetch(BASE_URI + '/clock', {
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
    return fetch(BASE_URI + '/' + action, { method: 'POST',});
  }

}

export default new Services();
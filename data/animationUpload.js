app.component('app-animation-upload', {
  template: `
      <h4 class="ui horizontal divider header">
          <i class="image icon"></i>
          Upload an animation
      </h4>
      <div class="ui card" style="width: auto;">
          <div class="content">
              <div ref="wrapper">
                  <div class="upload__button">
                      <label class="ui right labeled icon button primary">
                          <i class="image icon"></i>
                          Upload image
                          <input type="file" ref="uploader" hidden @change="handleFileSelect" accept="image/gif"/>
                      </label >
                  </div>
                  <img ref="imgRoot" style="display: none;"/>
              </div>
          </div>
      </div>
  `,
  data() {
      return {
          image: null,
          imgData: null,
          imgLoaded: false,
          framesList: [],
          OUT_WIDTH: 0,
          OUT_HEIGTH: 0,
          matrixParams: {
              height: 0,
              width: 0,
          },
          timer: null,
      };
  },
  computed: {
      OUT_SRC() {        
          const widthList = Array(this.OUT_WIDTH).fill().map((i, j) => j);
          const heightList = Array(this.OUT_HEIGTH).fill().map((i, j) => j);

          const byteArray = new Uint8Array(this.OUT_HEIGTH * this.OUT_WIDTH * 3);

          heightList.forEach(y => {
              widthList.forEach(x => {
                  let index = (y * widthList.length + x) * 3;
                  const {r, g, b} = this.getPixel(this.imgData, x, y);
                  byteArray[index++] = r;
                  byteArray[index++] = g;
                  byteArray[index++] = b;
              });
          });

          return byteArray;
      },
  },
  methods: {
      handleFileSelect(event){
        const [ file ] = event.target.files;
        if(!file) return;
        
        if(file.type !== '' && !file.type.match('image.*')) return;
        
        window.URL = window.URL || window.webkitURL;
        
        const imageURL = window.URL.createObjectURL(file);
        this.loadAndDrawImage(imageURL);
      },
      loadAndDrawImage(url) {
        this.image.addEventListener('load', () => {
          // this.drawImage();
          this.parseImage();
          this.imgLoaded = true;
        });

        this.image.setAttribute('rel:auto_play', 0);
        this.image.setAttribute('rel:rubble', 0);

        this.image.src = url;
      },
      parseImage() {
        const rub = new SuperGif({ gif: this.image, progressbar_height: 0 });
        rub.load(() => {
          clearTimeout(this.timer);
          this.framesList = [];

          for (let i = 0; i < rub.get_length(); i++) {
            rub.move_to(i); 
            this.cloneCanvas(rub.get_canvas());
          }

          this.initAnimation();
        });
      },
      cloneCanvas(oldCanvas) {
        const wrapper = this.$refs.wrapper;
        let { width } = wrapper.getBoundingClientRect();
        if (width > 600) width = 600;

        const newCanvas = document.createElement('canvas');
        const context = newCanvas.getContext('2d');
    
        const imgWidth = this.image.width;
        const imgHeight = this.image.height;
        const imgRatio = imgWidth / imgHeight;
  
        let xOffset = 0;
        let yOffset = 0;
        if (imgHeight !== imgWidth) {
            if (imgWidth > imgHeight) {
                yOffset = Math.floor((this.matrixParams.height * imgRatio - this.matrixParams.height) / 2);
            }
            if (imgWidth < imgHeight) {
                xOffset = Math.floor((this.matrixParams.width / imgRatio - this.matrixParams.width) / 2);
            }
        }
  
        this.OUT_WIDTH = this.matrixParams.width;
        this.OUT_HEIGTH = this.matrixParams.height;

        newCanvas.width = this.matrixParams.width;
        newCanvas.height = this.matrixParams.height;
    
        context.drawImage(oldCanvas, 0, 0, newCanvas.width, newCanvas.height, xOffset, yOffset, newCanvas.width, newCanvas.height);

        this.imgData = context.getImageData(0, 0, this.OUT_WIDTH, this.OUT_HEIGTH);

        this.framesList.push(new Blob([this.OUT_SRC], {type: "octet/stream"}));
      },
      getPixel(imageData, x, y) {
          const index = (x + y * imageData.width) * 4;
          return {
              r: imageData.data[index+0], 
              g: imageData.data[index+1], 
              b: imageData.data[index+2], 
              a: imageData.data[index+3],
          };
      },
      getMatrixParameters() {
          return services.getMatrixParameters()
              .then(params => {
                  this.matrixParams = params;
              });
      },
      initAnimation() {
        if (!this.framesList.length) return;
        this.sendImgData(0)
      },
      sendImgData(id) {
        const realId = (id > (this.framesList.length - 1)) ? 0 : id;
        services.sendImgData(this.framesList[realId]);

        this.timer = setTimeout(() => {
          this.sendImgData(realId + 1);
        }, 500)
      }
  },
  mounted() {
    this.getMatrixParameters();
    this.image = this.$refs.imgRoot;
  },
  unmounted() {
    clearTimeout(this.timer);
  }
});
app.component('app-picture-upload', {
    template: `
        <h4 class="ui horizontal divider header">
            <i class="image icon"></i>
            Upload a picture
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div ref="wrapper">
                    <div class="upload__button">
                        <label class="ui right labeled icon button primary">
                            <i class="image icon"></i>
                            Upload image
                            <input type="file" ref="uploader" hidden @change="handleFileSelect" accept="image/jpeg"/>
                        </label >
                    </div>
                    <canvas
                        class="upload__drawer"
                        ref="canvas"
                        width="16"
                        height="16"
                    ></canvas>
                </div>
            </div>
        </div>
    `,
    data() {
        return {
            image: new Image(),
            imgData: null,
            imgLoaded: false,
            OUT_WIDTH: 0,
            OUT_HEIGTH: 0,
            matrixParams: {
                height: 0,
                width: 0,
            },
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
            
            var imageURL = window.URL.createObjectURL(file);
            this.loadAndDrawImage(imageURL);
        },
        loadAndDrawImage(url) {
            this.image = new Image();
            this.image.addEventListener('load', () => {
                this.drawImage();
                this.imgLoaded = true;
            });
            this.image.src = url;
        },
        drawImage() {
            const wrapper = this.$refs.wrapper;
            let { width } = wrapper.getBoundingClientRect();
            if (width > 600) width = 600;

            const canvas = this.$refs.canvas;
            const context = canvas.getContext("2d");
        
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

            console.log(xOffset, yOffset)
      
            this.OUT_WIDTH = this.matrixParams.width;
            this.OUT_HEIGTH = this.matrixParams.height;

            canvas.width  = this.matrixParams.width;
            canvas.height = this.matrixParams.height;
        
            context.drawImage(this.image, 0, 0, canvas.width, canvas.height, xOffset, yOffset, canvas.width, canvas.height);
        
            this.imgData = context.getImageData(0, 0, canvas.width, canvas.height);

            this.sendImgData();
        },
        getPixel(imageData, x, y) {
            var index = (x + y * imageData.width) * 4;
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
        sendImgData() {
            const body = new Blob([this.OUT_SRC], {type: "octet/stream"});
            services.sendImgData(body);
        }
    },
    mounted() {
        this.getMatrixParameters();
    }
});
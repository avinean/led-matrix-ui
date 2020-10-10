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
                            Choose image
                            <input type="file" ref="uploader" hidden @change="handleFileSelect" accept="image/jpeg"/>
                        </label >
                    </div>
                    <canvas
                        class="upload__drawer"
                        ref="canvas"
                        width="640"
                        height="480"
                        hidden
                    ></canvas>
                    <div v-if="imgLoaded" class="upload__button">
                        <button class="ui right labeled icon button green" @click="sendImgData">
                            <i class="upload icon"></i>
                            Upload image
                        </button >
                    </div>
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
            OUT_DEPTH: 3,
        };
    },
    computed: {
        OUT_SRC() {        
            var widthList = Array(this.OUT_WIDTH).fill().map((i, j) => j);
            var heightList = Array(this.OUT_HEIGTH).fill().map((i, j) => j);

            return heightList.map(y => widthList.map(x => {
                const dta = this.getPixel(this.imgData, x, y );
                return `${dta.r},${dta.g},${dta.b}${ (( y == this.OUT_HEIGTH-1 ) && ( x == this.OUT_WIDTH - 1 )) ? "" : "," }`;
            }).join('')).join('');
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
      
            this.OUT_WIDTH = width;
            this.OUT_HEIGTH = width / imgRatio;

            canvas.width  = this.OUT_WIDTH;
            canvas.height = this.OUT_HEIGTH;
            canvas.style.display = 'block';
        
            context.drawImage(this.image, 0, 0, this.OUT_WIDTH, this.OUT_HEIGTH);
        
            this.imgData = context.getImageData(0, 0, this.OUT_WIDTH, this.OUT_HEIGTH);
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
        sendImgData() {
            const data = {
                width: this.OUT_WIDTH,
                height: this.OUT_HEIGTH,
                depth: this.OUT_DEPTH,
                data: this.OUT_SRC
            };

            
            fetch('/draw', {
                method: 'POST',
                body: JSON.stringify(data),
            });
        }
    }
});
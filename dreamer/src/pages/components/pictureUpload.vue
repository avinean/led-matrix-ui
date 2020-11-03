<template>
    <div>
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
                            <input
                                type="file"
                                ref="uploader"
                                hidden
                                accept="image/jpeg"
                                multiple
                                @change="handleFileSelect"
                            />
                        </label >
                    </div>
                    <canvas
                        class="upload__drawer"
                        ref="canvas"
                        width="16"
                        height="16"
                        hidden
                    ></canvas>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '../../utils/services';
import {store, IMG_CHOSEN_FROM_GALLERY, IMGS_CHOSEN_FROM_GALLERY, EFFECT_SPEED} from '../../utils/store';

export default {
    name: 'app-picture-upload',
    data() {
        return {
            image: new Image(),
            imageList: [],
            imgData: null,
            OUT_WIDTH: 0,
            OUT_HEIGTH: 0,
            matrixParams: {
                height: 0,
                width: 0,
            },

            slideShowTimer: null,

            drawPictureFromGalleryBind: this.drawPictureFromGallery.bind(this),
            drawPicturesFromGalleryBind: this.drawPicturesFromGallery.bind(this),
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
        handleFileSelect (event) {
            const [file] = event.target.files;
            if (!file) return;

            if (event.target.files.length === 1) {
                this.loadAndDrawImage(this.checkFile(file));
            } else {
                let index = 0;
                const srcList = []
                const timer = setInterval(() => {
                    let img = event.target.files[index];
                    if (!img) {
                        clearInterval(timer);
                        return;
                    }
                    this.loadAndDrawImage(
                        this.checkFile(img),
                        (image) => {
                            srcList.push(this.drawImage(image));
                            if (srcList.length === event.target.files.length) {
                                console.log(srcList)
                                this.sendMultiImgData(srcList);
                            }
                        }
                    )
                    index++;
                }, 100);
            }
        },
        checkFile(file) {
            if (file.type !== '' && !file.type.match('image.*')) return;

            window.URL = window.URL || window.webkitURL;

            return window.URL.createObjectURL(file);
        },
        loadAndDrawImage (url, cb) {
            this.image = new Image();
            this.image.addEventListener('load', () => {
                if (cb) {
                    cb(this.image);
                } else {
                    this.drawImage();
                }
            });
            this.image.src = url;
        },
        drawImage (image) {
            if (image) this.image = image;
            const wrapper = this.$refs.wrapper;
            let {width} = wrapper.getBoundingClientRect();
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

            this.OUT_WIDTH = this.matrixParams.width;
            this.OUT_HEIGTH = this.matrixParams.height;

            canvas.width = this.matrixParams.width;
            canvas.height = this.matrixParams.height;

            context.drawImage(this.image, 0, 0, canvas.width, canvas.height, xOffset, yOffset, canvas.width, canvas.height);

            this.imgData = context.getImageData(0, 0, canvas.width, canvas.height);

            if (!image) {
                this.sendImgData();
            }

            return this.OUT_SRC;
        },
        getPixel (imageData, x, y) {
            const index = (x + y * imageData.width) * 4;
            return {
                r: imageData.data[index],
                g: imageData.data[index + 1],
                b: imageData.data[index + 2],
                a: imageData.data[index + 3],
            };
        },
        getMatrixParameters () {
            return services.getMatrixParameters()
                .then(params => {
                    this.matrixParams = params;
                });
        },
        sendImgData () {
            const body = new Blob([this.OUT_SRC], {type: "octet/stream"});
            services.sendImgData(body);
        },
        sendMultiImgData (srcList) {
            const body = new Blob(srcList, {type: "octet/stream"});
            services.sendMultiImgData(body);
        },
        drawPictureFromGallery (image) {
            this.image = image;
            this.drawImage();
        },
        drawPicturesFromGallery (images) {
            let index = 0;
            const srcList = []
            const timer = setInterval(() => {
                let img = images[index];
                if (!img) {
                    console.log(srcList)
                    this.sendMultiImgData(srcList);
                    clearInterval(timer);
                    return;
                }
                srcList.push(this.drawImage(img));
                index++;
            }, 100);
        },
    },
    mounted() {
        this.getMatrixParameters();

        store.on(IMG_CHOSEN_FROM_GALLERY, this.drawPictureFromGalleryBind);
        store.on(IMGS_CHOSEN_FROM_GALLERY, this.drawPicturesFromGalleryBind);
    },
    unmounted () {
        store.remove(IMG_CHOSEN_FROM_GALLERY, this.drawPictureFromGalleryBind);
        store.remove(IMGS_CHOSEN_FROM_GALLERY, this.drawPicturesFromGalleryBind);
    }
}
</script>

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
import services from '/@utils/services';
import canvasMixins from '/@mixins/canvasMixins';

export default {
    name: 'app-picture-upload',
    inject: ['store'],
    mixins: [canvasMixins],
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
        };
    },
    watch: {
       'store.state.imageList'(images) {
           if (!images.length) return;
           this.drawRecursively(images);
       }
    },
    methods: {
        handleFileSelect (event) {
            const [file] = event.target.files;
            if (!file) return;

            const srcList = [];
            [...event.target.files].forEach(img => {
                this.loadAndDrawImage(
                    this.checkFile(img),
                    (image) => {
                        srcList.push(image);
                        if (srcList.length === event.target.files.length) {
                            this.drawRecursively(srcList);
                        }
                    }
                )
            })
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
        drawImage () {
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

            const body = new Blob([this.OUT_SRC], {type: "octet/stream"});
            return services.sendImgData(body);
        },
        getMatrixParameters () {
            return services.getMatrixParameters()
                .then(params => {
                    this.matrixParams = params;
                });
        },
        async drawRecursively([img, ...rest]) {
            if (!img) return;
            this.image = img;
            await this.drawImage();
            await this.drawRecursively(rest);
        },
    },
    mounted() {
        this.getMatrixParameters();
    },
}
</script>

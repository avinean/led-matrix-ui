app.component('app-picture-drawer', {
    template: `
        <h4 class="ui horizontal divider header">
            <i class="bar pencil icon"></i>
            Draw a picture
        </h4>
        <div class="ui card" style="width: auto;">
        <div class="content">
            <div ref="wrapper">
                <div class="upload__button">
                    <button
                        class="ui right labeled icon button green"
                        @click="isFillMode = true; picker.show();"
                    >
                        <i class="eye dropper icon"></i>
                        Fill field
                    </button>
                </div>
            </div>
        </div>
    </div>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div ref="colorPicker"></div>
                <div
                    ref="drawer"
                    class="drawer"
                    @touchstart="startDraw"
                    @touchmove="doDraw"
                    @touchend="endDraw"
                    @mousedown="startDraw"
                    @mousemove="doDraw"
                    @mouseup="endDraw"
                    @contextmenu.prevent="picker.show()"
                >
                    <div
                        v-for="(row, i) in matrix"
                        :key="row"
                        :style="'width: ' + charSize * matrixParams.width + 'px;'"
                        class="drawer__row"
                    >
                        <div
                            v-for="(col, j) in row"
                            class="drawer__col"
                            :style="'width: ' + charSize + 'px; height: ' + charSize + 'px; background: ' + col.background + ';'"
                            :data-x="j"
                            :data-y="i"
                        ></div>
                    </div>
                </div>
            </div>
        </div>
    `,
    data() {
        return {
            currentColor: 'rgb(0, 0, 0, 0)',
            srcColor: [0, 0, 0, 0],
            matrixParams: {
                height: 0,
                width: 0,
            },
            matrix: [
                [
                    {
                        background: 'rgb(0,0,0)',
                    },
                ],
            ],
            isDrawing: false,
            prevDrawnElement: null,
            prevTouchedElement: null,
            charSize: 0,
            picker: null,
            isFillMode: false,
        };
    },
    watch: {
        matrixParams: {
            deep: true,
            handler() {
                this.calculateCharSize();
            },
        },
    },
    methods: {
        calculateCharSize() {
            const drawer = this.$refs.drawer;
            if (!drawer) return 0;

            const { width } = drawer.getBoundingClientRect();

            this.charSize = width / this.matrixParams.width;
        },
        initPicker() {
            const pickerEl = this.$refs.colorPicker;
            this.picker = new Picker(pickerEl);
            this.picker.setOptions({
                popup: 'bottom',
            });
            
            this.picker.onDone = color =>  {
                this.srcColor = color.rgba;
                this.currentColor = color.rgbaString;
                if (this.isFillMode) {
                    this.fillDrawer();
                }
            };
        },
        callPicker(event) {
            var xcoord = event.touches? event.touches[0].clientX : event.clientX;
            var ycoord = event.touches? event.touches[0].clientY : event.clientY;
            
            var targetElement = document.elementFromPoint(xcoord, ycoord);
            this.prevTouchedElement = targetElement;

            setTimeout(() => {
                if (targetElement === this.prevTouchedElement) {
                    this.picker.show();
                }
            }, 1000);
        },
        startDraw(event) {
            this.isDrawing = true;
            this.doDraw(event);
            this.callPicker(event);
        },
        doDraw(event) {
            event.preventDefault();
            if (!this.isDrawing) return;
            
            var xcoord = event.touches? event.touches[0].clientX : event.clientX;
            var ycoord = event.touches? event.touches[0].clientY : event.clientY;
            
            var targetElement = document.elementFromPoint(xcoord, ycoord);

            if (this.prevDrawnElement === targetElement) return;
            this.prevDrawnElement = targetElement;
            this.prevTouchedElement = targetElement;
            
            if (targetElement && targetElement.classList.contains('drawer__col')) {
                const { x, y } = targetElement.dataset;
                this.matrix[y][x].background = this.currentColor;

                const [r, g, b] = this.srcColor;
                this.sendData({ x: +x, y: +y, r, g, b });
            }

        },
        endDraw() {
            this.isDrawing = false;
        },
        initMatrix() {
            const widthList = Array(this.matrixParams.width).fill().map((i, j) => j);
            const heightList = Array(this.matrixParams.height).fill().map((i, j) => j);

            this.matrix = heightList.map(i => widthList.map(j => {
                    const index = i * widthList.length + j ;

                    return {
                        background: 'rgb(0,0,0)',
                    }
                })
            );
        },
        redrawMatrix(values) {
            let index = 0;
            this.matrix.forEach((row, i) => {
                row.forEach((col, j) => {
                    const r = values[index++];
                    const g = values[index++];
                    const b = values[index++];
                    col.background = `rgb(${r},${g},${b})`;
                })
            })
        },
        initRefreshing() {
            setTimeout(async () => {
                await this.refresh();
                this.initRefreshing();
            }, 50);
        },
        sendData(params) {
            services.drawSinglePixel(params);
        },
        getMatrixParameters() {
            return services.getMatrixParameters()
                .then(params => {
                    this.matrixParams = params;
                });
        },
        refresh() {
            services.getMatrixState().then(({ value }) => {
                // if user is currently drawing than 
                // drawer shouldn't be refreshed from server
                if (this.isDrawing || !value) return;
                this.redrawMatrix(value);
            });
        },
        fillDrawer() {
            const [r, g, b] = this.srcColor;
            services.fillMatrix({ r, g, b });
        }
    },
    async mounted() {
        observer.on(IMG_UPLOADED, () => {
            this.refresh();
        });

        await this.getMatrixParameters();
        this.initMatrix();
        this.initPicker();
        this.calculateCharSize();
        this.initRefreshing();
    }
});
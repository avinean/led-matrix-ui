ц<template>
    <div>
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
                            Upload gif
                            <input
                                type="file"
                                hidden
                                @change="upload"
                                accept="image/gif"
                            />
                        </label >
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';

export default {
    name: 'app-animation-upload',
    methods: {
        upload (event) {
            const [file] = event.target.files;

            const formData = new FormData();
            formData.append('update', file);

            services.sendFile(formData);
        }
    }
}
</script>

<template>
    <div>
        <h4 class="ui horizontal divider header">
            <i class="cogs icon"></i>
            Settings
        </h4>
        <div class="ui card" style="width: auto;">
            <div class="content">
                <div class="mb-10">
                    <label class="ui right labeled icon button primary">
                        <i class="upload icon"></i>
                        Upload update
                        <input
                            type="file"
                            ref="uploader"
                            hidden
                            multiple
                            @change="upload"
                        />
                    </label >
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '../utils/services';

export default {
    name: 'app-settings',
    data() {
        return {
        };
    },
    methods: {
        upload (event) {
            const [file] = event.target.files;

            const formData = new FormData();
            formData.append('update', file);

            services.sendUpdate(formData);
        }
    },
}
</script>

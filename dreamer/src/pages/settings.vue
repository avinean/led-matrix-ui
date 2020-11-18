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
                <div class="ui divider"></div>
                <div class="mb-10">
                    Refreshing time
                    <app-range
                        :min="1"
                        :max="25"
                        v-model="refreshingTime"
                        @change="setRefreshingTime"
                    ></app-range>
                </div>
            </div>
        </div>
    </div>
</template>

<script>
import services from '/@utils/services';
import AppRange from '/@components/range.vue';

export default {
    name: 'app-settings',
    inject: ['store'],
    components: {
        AppRange,
    },
    computed: {
      refreshingTime: {
          get() {
              return this.store.state.refreshingTime / 1000;
          },
          set(value) {
              this.store.setRefreshingTime(value * 1000);
          }
      }
    },
    methods: {
        upload (event) {
            const [file] = event.target.files;

            const formData = new FormData();
            formData.append('update', file);

            services.sendFile(formData);
        },
        setRefreshingTime(){
            services.setRefreshingTime(this.refreshingTime);
        }
    },
}
</script>

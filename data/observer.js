const observer = {
  events: {},
  emit(event, ...args) {
      const e = this.events[event];
      if (!(e && e instanceof Array)) return;

      e.forEach((cb) => {
          cb(...args);
      })
  },
  on(event, cb, once) {
      let e = this.events[event];
      if (!(e && e instanceof Array)) {
          this.events[event] = [];
          e = this.events[event];
      }

      if (once && e.includes(cb)) return;

      e.push(cb);
  },
  remove(event, cb) {
    let e = this.events[event];
    if (!(e && e instanceof Array)) {
        this.events[event] = [];
        e = this.events[event];
    }

    const index = e.indexOf(cb);
    if (index < 0) return;

    e.splice(index, 1);
  }
}
console.log(observer)

const IMG_UPLOADED = 'IMG_UPLOADED';
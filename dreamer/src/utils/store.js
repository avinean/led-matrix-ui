/* data keys */
export const EFFECT_SPEED = 'EFFECT_SPEED';
export const EFFECT = 'EFFECT';

/* events keys */
export const IMG_UPLOADED = 'IMG_UPLOADED';
export const IMG_CHOSEN_FROM_GALLERY = 'IMG_CHOSEN_FROM_GALLERY';
export const IMGS_CHOSEN_FROM_GALLERY = 'IMGS_CHOSEN_FROM_GALLERY';


const data = {
    [EFFECT_SPEED]: 2000,
    [EFFECT]: null
};
const events = {};

export const store = {
    emit (event, ...args) {
        const e = events[event];
        if (!(e && e instanceof Array)) return;

        e.forEach((cb) => {
            cb(...args);
        })
    },
    on (event, cb, once) {
        let e = events[event];
        if (!(e && e instanceof Array)) {
            events[event] = [];
            e = events[event];
        }

        if (once && e.includes(cb)) return;

        e.push(cb);
    },
    remove (event, cb) {
        let e = events[event];
        if (!(e && e instanceof Array)) {
            events[event] = [];
            e = events[event];
        }

        const index = e.indexOf(cb);
        if (index < 0) return;

        e.splice(index, 1);
    },
    set(key, value) {
        if (key in data)
            data[key] = value;
    },
    get(key) {
        if (key in data)
            return data[key];
    }
};


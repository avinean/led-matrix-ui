npm run build
gzip -r ./dist/_assets
rm -r ../../fw/data/_assets
rm ../../fw/data/index.html
cp -r ./dist/* ../../fw/data
cd ../../fw/data

if [[ ! -e ./gifs ]]; then
    mkdir ./gifs
elif [[ ! -d ./gifs ]]; then	
    echo "./gifs already exists but is not a directory" 1>&2
fi
cp -r ../../web/dreamer/gifs/* ./gifs

if [[ ! -e ./imgs ]]; then
    mkdir ./imgs
elif [[ ! -d ./imgs ]]; then	
    echo "./imgs already exists but is not a directory" 1>&2
fi
cp -r ../../web/dreamer/imgs/* ./imgs
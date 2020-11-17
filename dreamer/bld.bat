npm run build 
rem cd ./dist
rem gzip -vr ./_assets
rem cd ../
rem rm -r ../data/_assets
DEL /F/Q/S ..\data\_assets\*.*
rem rm ../data/index.html
DEL /F/Q/S ..\data\index.html   
Xcopy /E /I /Y .\dist ..\data   
1.bat                             
cd ../data 

rem if [[ ! -e ./gifs ]]; then
rem     mkdir ./gifs
rem elif [[ ! -d ./gifs ]]; then	
rem     echo "./gifs already exists but is not a directory" 1>&2
rem fi
Xcopy /E /I ..\dreamer\gifs .\gifs 

rem if [[ ! -e ./imgs ]]; then
rem     mkdir ./imgs
rem elif [[ ! -d ./imgs ]]; then	
rem     echo "./imgs already exists but is not a directory" 1>&2
rem fi
Xcopy /E /I ..\dreamer\imgs .\imgs 
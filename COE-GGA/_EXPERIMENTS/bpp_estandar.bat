@echo off
set datetime=%date:~-4,4%-%date:~-7,2%-%date:~-10,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%
set outputfile=..\..\_OUTPUT\resultados_%datetime%.json

echo Ruta actual: %cd%
pushd ..\_INPUT\bpp_estandar
 echo { >> %outputfile%
for %%f in (*) do (
    echo Procesando archivo: %%f
    echo "%%f": >> %outputfile%
    ..\..\..\x64\Debug\COE-GGA.exe "%%f" >> %outputfile%
    echo , >> %outputfile%
)
echo "descripcion":"Archivo de resultados de la carpeta bpp_estandar, FFD"} >> %outputfile%
popd

echo Proceso finalizado. Ver resultados en %outputfile%
pause

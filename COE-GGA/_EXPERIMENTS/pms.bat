@echo off
set datetime=%date:~-4,4%-%date:~-7,2%-%date:~-10,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%
set outputfile2=..\..\..\_OUTPUT\resultados_%datetime%.txt

echo Ruta actual: %cd%
for /d %%f in (..\_INPUT\pms\*) do (
pushd %%f
echo Procesando Carpeta: %%f
for %%i in (*) do (
  echo Procesando archivo: %%i
     echo Procesando archivo: %%i >> %outputfile2%
    ..\..\..\..\x64\Debug\COE-GGA.exe "%%i" >> %outputfile2%

)
popd
)

echo Proceso finalizado. Ver resultados en %outputfile2%
pause

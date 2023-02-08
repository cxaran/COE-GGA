@echo off
cls
setlocal enabledelayedexpansion

echo Ruta actual: %cd%
for /d %%f in (..\_INPUT\pms\*) do (
pushd %%f
set sub=%%f
set sub=!sub:*\_INPUT\pms\=!
set outputfolder=..\_OUTPUT\!sub!_
set datetime=%date:~-4,4%-%date:~-7,2%-%date:~-10,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%
set outputfile=resultados_!datetime!.txt
md ..\..\!outputfolder! 2>nul
  echo Procesando Carpeta: %%f
  for %%i in (*) do (
    if /i not "%%i"=="list.txt" (
     echo Procesando archivo: %%i
     echo Procesando archivo: %%i >> ..\..\!outputfolder!\!outputfile!
     ..\..\..\..\x64\Debug\COE-GGA.exe "%%i" >> ..\..\!outputfolder!\!outputfile!
    )
  )
popd
)

echo Proceso finalizado. Ver resultados en %outputfolder%
pause



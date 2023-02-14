@echo off
setlocal enableDelayedExpansion

set datetime=%date:~-4,4%-%date:~-7,2%-%date:~-10,2%_%time:~0,2%-%time:~3,2%-%time:~6,2%
set outputfolder=..\_OUTPUT\resultados_%datetime%
mkdir %outputfolder%

echo Ruta actual: %cd%
pushd ..\_INPUT\pms
for /d %%d in (*) do (
  pushd %%d
  echo Procesando carpeta: %%d
  set outputfile=..\..\%outputfolder%\%%d.json
  echo { >> !outputfile!
  for %%f in (*) do (
    if /i not "%%f"=="list.txt" (
      echo Procesando archivo: %%f
      echo "%%f": >> !outputfile!
      ..\..\..\..\x64\Debug\COE-GGA.exe "%%f" >> !outputfile!
      echo , >> !outputfile!
    )
  )
  echo "descripcion":"Archivo de resultados de la carpeta %%d"} >> !outputfile!
  echo Ver resultados en !outputfile!
  popd
)
popd

echo Proceso finalizado. Ver resultados en %outputfolder%
pause


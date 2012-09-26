@echo off & setlocal enableextensions
::
:: Created by Martin on 25 September 2012.
::
:: Create WholeValue archive
::

set EXTS=bak exe obj tmp
set ROOT=%~dp0
set DATEPROG=C:\Programs\MinGW\msys\1.0\bin\date.exe

cd %ROOT%..\..\

for %%f in ( %EXTS% ) do (
   find . -name *.%%f -exec rm {} ;
)

call :Now dt tm
cd .. && tar czvf %dt%-WholeValue.tar.gz WholeValue

endlocal & goto :EOF

::
:: Subroutine Now( [out]date, [out]time )
::
:Now
setlocal
set TMPNAME=%ROOT%archive.tmp
%DATEPROG% +%%Y%%m%%d > %TMPNAME%
for /f "tokens=1 delims=" %%f in ( %TMPNAME% ) do (
    set YMD=%%f
)
del %TMPNAME%
endlocal & set %1=%YMD%& set %2=%TIME:~0,-3%& goto :EOF

::
:: end of file
::

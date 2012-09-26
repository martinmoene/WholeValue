@echo off
::
:: Created by Martin on 23 September 2012.
::
:: Compile WholeValue performance tests, GCC
::

call :Compile TestPerformance-Contained.exe      -O2  &&^
call :Compile TestPerformance-Contained.exe      -O2 -DWV_DEFINE_OPERATORS_IN_TERMS_OF_A_MINIMAL_NUMBER_OF_FUNDAMENTAL_OPERATORS &&^
call :Compile TestPerformance-BoostOperators.exe -O2 -DWV_USE_BOOST_OPERATORS
goto :EOF

:Compile
setlocal
set NAME=%1
set OPT=%2 %3 %4 %5 %6
g++ -Wall -Wextra --pedantic %OPT% -I../../../include/ -I%BOOST_INCLUDE% -o %NAME% ../../Test/TestPerformance.cpp &&^
echo.
echo Options: %OPT% &&^%NAME%
echo.
endlocal & goto :EOF

::
:: end of file
::

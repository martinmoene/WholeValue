@echo off
::
:: Created by Martin on 23 September 2012.
::
:: Compile WholeValue performance tests, MS VC6, VS6
::

if [%BOOST_INCLUDE%] == [] goto :BOOST_ERROR
if [%CATCH_INCLUDE%] == [] goto :CATCH_ERROR

if [%1] == [-h] goto :USAGE
if [%1] == [help] goto :USAGE

call :Compile TestPerformance-Contained.exe      -O2  &&^
call :Compile TestPerformance-Contained.exe      -Ox  &&^
call :Compile TestPerformance-Contained.exe      -O2 -DWV_DEFINE_OPERATORS_IN_TERMS_OF_A_MINIMAL_NUMBER_OF_FUNDAMENTAL_OPERATORS &&^
call :Compile TestPerformance-BoostOperators.exe -O2 -DWV_USE_BOOST_OPERATORS
call :Compile TestPerformance-BoostOperators.exe -Ox -DWV_USE_BOOST_OPERATORS
goto :EOF

:Compile
setlocal
set NAME=%1
set OPT=%2 %3 %4 %5 %6
cl -nologo -W3 -EHsc -GR %OPT% -I../../../include/ -I%BOOST_INCLUDE% -Fe%NAME% ../../Test/TestPerformance.cpp &&^
echo.
echo Options: %OPT% &&^%NAME%
echo.
endlocal & goto :EOF

:BOOST_ERROR
echo Error: Please specify environment variable BOOST_INCLUDE to point to the Boost library include directory.
echo Boost: http://boost.org
goto :EOF

:CATCH_ERROR
echo Error: Please specify environment variable CATCH_INCLUDE to point to the Catch test framework include directory.
echo Catch: https://github.com/philsquared/Catch
echo Catch-VC6: https://github.com/martinmoene/Catch
goto :EOF

:USAGE
echo Usage: %0 [-h]
goto :EOF

endlocal

::
:: End of file
::

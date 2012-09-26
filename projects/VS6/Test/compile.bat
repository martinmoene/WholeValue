@echo off & setlocal
::
:: Created by Martin on 16 September 2012.
::
:: Compile WholeValue selftests, VC6
::

set G_OPT=-DWV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE

if [%CATCH_INCLUDE%] == [] goto :CATCH_ERROR

if [%1] == [-h] goto :USAGE
if [%1] == [help] goto :USAGE
if [%1] == [all] goto :ALL
if [%1] == [all-bo] goto :ALL_BO
if [%1] == [default] goto :DEFAULT
if [%1] == [default-bo] goto :DEFAULT_BO
if [%1] == [optimize] goto :OPTIMIZE
if [%1] == [optimize-bo] goto :OPTIMIZE_BO
if [%1] == [debug] goto :DEBUG
if [%1] == [debug-bo] goto :DEBUG_BO
if not [%1] == [] goto :USAGE
goto :DEFAULT

:ALL
echo Default build:   && call :DEFAULT &&^
echo Optimized build: && call :OPTIMIZE &&^
echo Debug build:     && call :DEBUG
goto :EOF

:ALL_BO
echo Default Boost.Operators build:   && call :DEFAULT_BO &&^
echo Optimized Boost.Operators build: && call :OPTIMIZE_BO &&^
echo Debug Boost.Operators build:     && call :DEBUG_BO
goto :EOF

:DEFAULT
call :COMPILE
goto :EOF

:DEFAULT_BO
call :CHECK_BOOST &&^
call :COMPILE -DWV_USE_BOOST_OPERATORS
goto :EOF

:OPTIMIZE
call :COMPILE -Ox
goto :EOF

:OPTIMIZE_BO
call :CHECK_BOOST &&^
call :COMPILE -Ox -DWV_USE_BOOST_OPERATORS
goto :EOF

:DEBUG
call :COMPILE -MDd -D_DEBUG
goto :EOF

:DEBUG_BO
call :CHECK_BOOST &&^
call :COMPILE -MDd -D_DEBUG -DWV_USE_BOOST_OPERATORS
goto :EOF

:COMPILE
setlocal
set OPT=%*
cl -nologo -W3 -EHsc -GR %G_OPT% %OPT% -I../../../include/ -I%BOOST_INCLUDE% -I%CATCH_INCLUDE% -FeTest.exe ../../Test/TestMain.cpp ../../Test/TestArithmetic.cpp ../../Test/TestBits.cpp ../../Test/TestQuantity.cpp ../../Test/TestSafeBool.cpp ../../Test/TestWholeValue.cpp && Test
endlocal & goto :EOF

:CHECK_BOOST
if [%BOOST_INCLUDE%] == [] goto :BOOST_ERROR
goto :EOF

:BOOST_ERROR
echo Error: Please specify environment variable BOOST_INCLUDE to point to the Boost library include directory.
echo Boost: http://boost.org
exit /b 1

:CATCH_ERROR
echo Error: Please specify environment variable CATCH_INCLUDE to point to the Catch test framework include directory.
echo Catch: https://github.com/philsquared/Catch
echo Catch-VC6: https://github.com/martinmoene/Catch
exit /b 1

:USAGE
echo Usage: %0 [-h] [default] [optimize] [debug] [all] {-bo}
goto :EOF

endlocal

::
:: End of file
::

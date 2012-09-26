@echo off
::
:: Created by Martin on 16 September 2012.
::
:: Compile WholeValue selftests, GCC
::

if [%BOOST_INCLUDE%] == [] goto :BOOST_ERROR
if [%CATCH_INCLUDE%] == [] goto :CATCH_ERROR

set BO=-DWV_USE_BOOST_OPERATORS

g++ -Wall -Wextra --pedantic %BO% -I../../../include/ -I%BOOST_INCLUDE% -I%CATCH_INCLUDE% -o Test ../../Test/TestMain.cpp ../../Test/TestArithmetic.cpp ../../Test/TestBits.cpp ../../Test/TestQuantity.cpp ../../Test/TestSafeBool.cpp ../../Test/TestWholeValue.cpp && Test

goto :EOF

:BOOST_ERROR
echo Error: Please specify environment variable BOOST_INCLUDE to point to the Boost library include directory.
echo Boost: http://boost.org
goto :EOF

:CATCH_ERROR
echo Error: Please specify environment variable CATCH_INCLUDE to point to the Catch test framework include directory.
echo Catch: https://github.com/philsquared/Catch
echo Catch-VC6: https://github.com/martinmoene/Catch
goto :EOF

:: End of file

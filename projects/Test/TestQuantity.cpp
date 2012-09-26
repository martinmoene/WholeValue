/*
 * TestMain.cpp
 *
 * Created by Martin on 16 September 2012.
 * Copyright 2012 Universiteit Leiden. All rights reserved.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * These tests use the Catch test framework by Phil Nash.
 * - https://github.com/philsquared/Catch - MSVC8, 9, 2010, GCC 4.2, LLVM 4.0
 * - https://github.com/martinmoene/Catch - MSVC6 in addition to the above
 */

#include "catch.hpp"

//#define WV_USE_BOOST_OPERATORS
//#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY
#include "whole_value.h"

#include <iomanip>

// Define fundamental value types:
WV_DEFINE_QUANTITY_TYPE( Quant, double )
WV_DEFINE_QUANTITY_TYPE( IQuant, int )

// Define 'domain' value types:
WV_DEFINE_TYPE( Speed, Quant )
WV_DEFINE_TYPE( Count, IQuant )

#ifdef G_WV_COMPILER_IS_MSVC6
    // VC6 has no argument dependent lookup (ADL)
    using namespace wv;
#endif

/* ----------------------------------------------------------------------------
 * quantity operations:
 *
 * quantity type.
 *
 * -   ++x, x++, --x, x--
 *
 * -   x == x, x != x
 * -   x < x, x <= x,
 * -   x > x, x >= x,
 *
 * - [ T + x, T - x ]
 * - [ x + T, x - T ]
 * -   x + x, x - x
 *
 * - [ x * T, T * x, x / T, x % T ]
 */

namespace {

void operations()
{
    Count x; (void)x;

    ++x;
    --x;
    x++;
    x--;

    x == x;
    x != x;

    x < x;
    x <= x;
    x > x;
    x >= x;

    x += x;
    x -= x;
//    x *= x;
//    x /= x;
//    x %= x;

    x + x;
    x - x;
//    x * x;
//    x / x;
//    x % x;


#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY
    // involving constants right:

    x == 7;
    x != 7;

    x < 7;
    x <= 7;
    x > 7;
    x >= 7;

    x += 7;
    x -= 7;
    x *= 7;
    x /= 7;
    x %= 7;

    x + 7;
    x - 7;
    x * 7;
    x / 7;
    x % 7;

    // involving constants left:

    7 == x;
    7 != x;

    7 < x;
    7 <= x;
    7 > x;
    7 >= x;

//    7 += x;
//    7 -= x;
//    7 *= x;
//    7 /= x;
//    7 %= x;

    7 + x;
    7 - x;
    7 * x;
//    7 / x;
//    7 % x;

#endif
}

}

/* ----------------------------------------------------------------------------
 * quantity:
 *
 * Please note: the Catch test framework cannot handle quantity::operator==(),
 * hence we use REQUIRE( !!( expression-involving-whole-value ) ) where needed.
 */

TEST_CASE( "quantity/construction",
           "Quantity construction." )
{
    SECTION( "quantity/construction/default",
             "Default constructor creates value with default constructed base type." )
    {
        Count c;
        int i = int();
        REQUIRE( i == 0 );
        REQUIRE( c.value() == i );
    }

    SECTION( "quantity/construction/initialise",
             "Initializing constructor create whole value with given value." )
    {
        Count c( 33 );
        REQUIRE( c.value() == 33 );
    }

    SECTION( "quantity/construction/copy-assign",
             "Copy-assignment constructor creates whole value with given value." )
    {
        Count a( 33 );
        Count b( a  );
        Count c = a;
        REQUIRE( a.value() == b.value() );
        REQUIRE( a.value() == c.value() );
    }
}

TEST_CASE( "quantity/assignment",
           "Quantity assignment." )
{
    Count a( 33 );
    Count b( 44 );
    a = b;
    REQUIRE( a.value() == 44 );
    REQUIRE( a.value() == b.value() );
}

TEST_CASE( "quantity/comparison",
           "Quantity comparison" )
{
    SECTION( "quantity/comparison/eq", "" )
    {
        REQUIRE( !!( Count(12) == Count(12) ) );
        REQUIRE(  !( Count(12) == Count(17) ) );
    }

    SECTION( "quantity/comparison/ne", "" )
    {
        REQUIRE( !!( Count(12) != Count(17) ) );
        REQUIRE(  !( Count(12) != Count(12) ) );
    }

    SECTION( "quantity/comparison/lt", "" )
    {
        REQUIRE( !!( Count(12) < Count(17) ) );
        REQUIRE(  !( Count(12) < Count(12) ) );
    }

    SECTION( "quantity/comparison/le", "" )
    {
        REQUIRE( !!( Count(12) <= Count(17) ) );
        REQUIRE(  !( Count(12) <= Count( 9) ) );
    }

    SECTION( "quantity/comparison/gt", "" )
    {
        REQUIRE( !!( Count(17) > Count(12) ) );
        REQUIRE(  !( Count(12) > Count(12) ) );
    }

    SECTION( "quantity/comparison/ge", "" )
    {
        REQUIRE( !!( Count(17) >= Count(12) ) );
        REQUIRE(  !( Count( 9) >= Count(12) ) );
    }

}

TEST_CASE( "quantity/unit-step",
           "Quantity increment and decrement." )
{
    SECTION( "quantity/unit-step/preincrement", "" )
    {
        { Count count7(7); REQUIRE( !!( ++count7 == Count(8) ) ); }
        { Count count7(7); REQUIRE(  !( ++count7 == Count(7) ) ); }
    }

    SECTION( "quantity/unit-step/postincrement", "" )
    {
        { Count count7(7); REQUIRE( !!( count7++ == Count(7) ) ); }
        { Count count7(7); REQUIRE(  !( count7++ == Count(8) ) ); }
    }

    SECTION( "quantity/unit-step/predecrement", "" )
    {
        { Count count7(7); REQUIRE( !!( --count7 == Count(6) ) ); }
        { Count count7(7); REQUIRE(  !( --count7 == Count(7) ) ); }
    }

    SECTION( "quantity/unit-step/postdecrement", "" )
    {
        { Count count7(7); REQUIRE( !!( count7-- == Count(7) ) ); }
        { Count count7(7); REQUIRE(  !( count7-- == Count(6) ) ); }
    }
}

TEST_CASE( "quantity/arithmetic",
           "Quantity arithmetic." )
{
    SECTION( "quantity/arithmetic/add", "" )
    {
        Count value7(7);

        REQUIRE( !!( ( value7 += Count(5) ) == Count(12) ) );

        REQUIRE( !!( Count(7) + Count(5) == Count(12) ) );
        REQUIRE(  !( Count(7) + Count(7) == Count(12) ) );
    }

    SECTION( "quantity/arithmetic/subtract", "" )
    {
        Count value12(12);

        REQUIRE( !!( ( value12 -= Count(5) ) == Count(7) ) );

        REQUIRE( !!( Count(12) - Count(5) == Count( 7) ) );
        REQUIRE(  !( Count(12) - Count(7) == Count(12) ) );
    }

    SECTION( "quantity/arithmetic/multiply", "" )
    {
#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY
        Count value7(7);

        REQUIRE( !!( ( value7 *= 5 ) == Count(35) ) );

        REQUIRE( !!( Count(7) * 5 == Count(35) ) );
        REQUIRE(  !( Count(7) * 7 == Count(35) ) );

        REQUIRE( !!( 5 * Count(7) == Count(35) ) );
        REQUIRE(  !( 7 * Count(7) == Count(35) ) );
#endif
    }

    SECTION( "quantity/arithmetic/divide", "" )
    {
#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY
        Count value49(49);

        REQUIRE( !!( ( value49 /= 7 ) == Count(7) ) );

        REQUIRE( !!( Count(49) / 7 == Count(7) ) );
        REQUIRE(  !( Count(49) / 8 == Count(7) ) );
#endif
    }

    SECTION( "quantity/arithmetic/mod", "" )
    {
#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY
        Count value3(3);

        REQUIRE( !!( ( value3 %= 2 ) == Count(1) ) );

        REQUIRE( !!( Count(3) % 2 == Count(1) ) );
        REQUIRE(  !( Count(3) % 3 == Count(1) ) );
#endif
    }
}

TEST_CASE( "quantity/methods",
           "Quantity methods" )
{
    SECTION( "quantity/methods/as", "" )
    {
        Count a( 'a' );
#ifdef G_WV_COMPILER_IS_MSVC6
        char chr = a.as( char() );
#else
        char chr = a.as<char>();
#endif
        REQUIRE( a.value() == 'a' );
        REQUIRE( chr == 'a' );
    }
}

TEST_CASE( "quantity/functions",
           "Quantity free functions." )
{
    SECTION( "quantity/functions/abs", "" )
    {
        REQUIRE( !!( abs( Count(+12) ) == Count(+12) ) );
        REQUIRE( !!( abs( Count(-12) ) == Count(+12) ) );

        REQUIRE( Approx( abs( Speed(+1.23 ) ).value() ) == +1.23 );
        REQUIRE( Approx( abs( Speed(-1.23 ) ).value() ) == +1.23 );
    }

    SECTION( "quantity/functions/to_integer", "" )
    {
        REQUIRE( to_integer( Count(+12) ) == +12 );
        REQUIRE( to_integer( Count(-12) ) == -12 );
    }

    SECTION( "quantity/functions/to_real", "" )
    {
        REQUIRE( Approx( to_real( Speed(+1.23) ) ) == +1.23 );
        REQUIRE( Approx( to_real( Speed(-1.23) ) ) == -1.23 );
    }
}

namespace quantity_streaming {

WV_ADD_STREAM_INSERTION_OPERATOR( IQuant )
WV_ADD_STREAM_INSERTION_OPERATOR( Quant )

TEST_CASE( "quantity/streaming",
           "Quantity streaming." )
{
    SECTION( "quantity/streaming/integer", "" )
    {
        Count c( 321 );
        std::ostringstream os;
        os << c;
        REQUIRE( os.str() == "321" );
    }

    SECTION( "quantity/streaming/real", "" )
    {
        Speed f( 1.23456 );
        std::ostringstream os;
        os << f;
        REQUIRE( os.str() == "1.23456" );
    }
}
} // namespace quantity_streaming

namespace quantity_streaming_iomanip {

WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP( Quant, std::fixed << std::setprecision(2) )

TEST_CASE( "quantity/streaming/iomanip",
           "Quantity streaming io manipulators." )
{
    SECTION( "quantity/streaming/iomanip/integer", "" )
    {
        Speed f( 1.23456789e3 );
        std::ostringstream os;
        os << f;
        REQUIRE( os.str() == "1234.57" );
    }
}
} // namespace quantity_streaming_iomanip

/*
 * end of file
 */

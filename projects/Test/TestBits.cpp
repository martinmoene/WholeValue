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
//#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
#include "whole_value.h"

#include <iomanip>

// Define fundamental value types:
WV_DEFINE_BITS_TYPE( Mask, unsigned int )

// Define 'domain' value types:
WV_DEFINE_TYPE( BitMask, Mask )
WV_DEFINE_TYPE( BitMaskX, Mask )

#ifdef G_WV_COMPILER_IS_MSVC6
    // VC6 has no argument dependent lookup (ADL)
    using namespace wv;
#endif

/* ----------------------------------------------------------------------------
 * bits operations:
 *
 * - mask == mask  - !=
 * - mask == 0x123 - !=
 * - 0x123 == mask - !=
 * - mask ^= mask  - &=, |=
 * - mask ^  mask  - & , |
 * - mask ^  0x123 - & , |
 * - mask <<= N    - >>=
 * - mask <<  N    - >>
 */

namespace {

void operations()
{
    BitMask x;
    BitMaskX y;

    x = x;
    y = y;
//    x = y;

    BitMask(1) == BitMaskX(1);
    volatile bool r = ( x == y ); (void) r;

    x == x;
    x != x;

    x ^= x;
    x &= x;
    x |= x;
//    x <<= x;
//    x >>= x;

    x ^ x;
    x & x;
    x | x;
//    x << x;
//    x >> x;

#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
    // involving constants right:

    x == 7;
    x != 7;

    x ^= 7;
    x &= 7;
    x |= 7;
    x <<= 7;
    x >>= 7;

    x ^ 7;
    x & 7;
    x | 7;
    x << 7;
    x >> 7;

    // involving constants left:

    7 == x;
    7 != x;

//    7 ^= x;
//    7 &= x;
//    7 |= x;
//    x <<= x;
//    x >>= x;

    7 ^ x;
    7 & x;
    7 | x;
//    x << x;
//    x >> x;
#endif
}

}

/* ----------------------------------------------------------------------------
 * bits:
 *
 * Please note: the Catch test framework cannot handle quantity::operator==(),
 * hence we use REQUIRE( !!( expression-involving-whole-value ) ) where needed.
 */

TEST_CASE( "bits/construction",
           "Bits construction." )
{
    SECTION( "bits/construction/default",
             "Default constructor creates value with default constructed base type." )
    {
        BitMask m;
        BitMask::value_type v = BitMask::value_type();
        REQUIRE( v == 0 );
        REQUIRE( m.value() == v );
    }

    SECTION( "bits/construction/initialise",
             "Initializing constructor create whole value with given value." )
    {
        BitMask m( 0x1234 );
        REQUIRE( m.value() == 0x1234 );
    }

    SECTION( "bits/construction/copy-assign",
             "Copy-assignment constructor creates whole value with given value." )
    {
        BitMask a( 0x1234 );
        BitMask b( a  );
        BitMask c = a;
        REQUIRE( a.value() == b.value() );
        REQUIRE( a.value() == c.value() );
    }
}

TEST_CASE( "bits/assignment",
           "Bits assignment." )
{
    BitMask a( 0x1234 );
    BitMask b( 0x5e5e );
    a = b;
    REQUIRE( a.value() == 0x5e5e );
    REQUIRE( a.value() == b.value() );
}

TEST_CASE( "bits/comparison",
           "Bits comparison" )
{
    SECTION( "bits/comparison/eq", "" )
    {
        REQUIRE( !!( BitMask( 0x1234 ) == BitMask( 0x1234 ) ) );
        REQUIRE(  !( BitMask( 0x1234 ) == BitMask( 0x0234 ) ) );

#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
        REQUIRE( !!( BitMask( 0x1234 ) ==          0x1234   ) );
        REQUIRE( !!(          0x1234   == BitMask( 0x1234 ) ) );

        BitMask::value_type mask = 0x1234;

        REQUIRE( !!( BitMask( 0x1234 ) ==          mask     ) );
        REQUIRE( !!(              mask == BitMask( 0x1234 ) ) );
#endif
    }

    SECTION( "bits/comparison/ne", "" )
    {
        REQUIRE( !!( BitMask( 0x4321 ) != BitMask( 0xF321 ) ) );
        REQUIRE(  !( BitMask( 0x4321 ) != BitMask( 0x4321 ) ) );

#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
        REQUIRE( !!( BitMask( 0x4321 ) !=          0xF321   ) );
        REQUIRE( !!(          0xF321   != BitMask( 0x4321 ) ) );

        BitMask::value_type mask = 0xF321;

        REQUIRE( !!( BitMask( 0x4321 ) !=          mask     ) );
        REQUIRE( !!(          mask     != BitMask( 0x4321 ) ) );
#endif
    }
}

TEST_CASE( "bits/bitwise",
           "Bits bitwise operations." )
{
    SECTION( "bits/bitwise/xor", "" )
    {
        {
        BitMask mask7( 0x7 );
        REQUIRE( !!( (       mask7 ^= BitMask(0x3) ) == BitMask(0x4) ) );
        REQUIRE( !!( ( BitMask(0x7) ^ BitMask(0x3) ) == BitMask(0x4) ) );
        }

#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
        {
        BitMask mask7( 0x7 );
        REQUIRE( !!( (       mask7 ^=         0x3  ) == BitMask(0x4) ) );
        REQUIRE( !!( ( BitMask(0x7) ^         0x3  ) == BitMask(0x4) ) );
        REQUIRE( !!( (         0x3  ^ BitMask(0x7) ) == BitMask(0x4) ) );
        }
#endif
    }

    SECTION( "bits/bitwise/and", "" )
    {
        {
        BitMask mask7( 0x7 );
        REQUIRE( !!( (       mask7 &= BitMask(0x3) ) == BitMask(0x3) ) );
        REQUIRE( !!( ( BitMask(0x7) & BitMask(0x3) ) == BitMask(0x3) ) );
        }

#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
        {
        BitMask mask7( 0x7 );
        REQUIRE( !!( (       mask7 &=         0x3  ) == BitMask(0x3) ) );
        REQUIRE( !!( ( BitMask(0x7) &         0x3  ) == BitMask(0x3) ) );
        REQUIRE( !!( (         0x3  & BitMask(0x7) ) == BitMask(0x3) ) );
        }
#endif
    }

    SECTION( "bits/bitwise/or", "" )
    {
        {
        BitMask mask2( 0x2 );
        REQUIRE( !!( (       mask2 |= BitMask(0x1) ) == BitMask(0x3) ) );
        REQUIRE( !!( ( BitMask(0x2) | BitMask(0x1) ) == BitMask(0x3) ) );
        }

#ifdef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
        {
        BitMask mask2( 0x2 );
        REQUIRE( !!( (       mask2 |=         0x1  ) == BitMask(0x3) ) );
        REQUIRE( !!( ( BitMask(0x2) |         0x1  ) == BitMask(0x3) ) );
        REQUIRE( !!( (         0x1  | BitMask(0x2) ) == BitMask(0x3) ) );
        }
#endif
    }

    SECTION( "bits/bitwise/left-shift", "" )
    {
        BitMask mask1( 0x1 );

        REQUIRE( !!( ( mask1       <<= 3 ) == BitMask(0x8) ) );
        REQUIRE( !!( ( BitMask(0x1) << 3 ) == BitMask(0x8) ) );
    }

    SECTION( "bits/bitwise/right-shift", "" )
    {
        BitMask mask8( 0x8 );

        REQUIRE( !!( ( mask8       >>= 3 ) == BitMask(0x1) ) );
        REQUIRE( !!( ( BitMask(0x8) >> 3 ) == BitMask(0x1) ) );
    }
}

namespace bits_streaming {

WV_ADD_STREAM_INSERTION_OPERATOR( Mask )

TEST_CASE( "bits/streaming",
           "Bits streaming." )
{
    BitMask m( 1234 );
    std::ostringstream os;
    os << m;
    REQUIRE( os.str() == "1234" );
}
} // namespace bits_streaming

namespace bits_streaming_iomanip {

WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP( Mask, std::hex << std::showbase )

TEST_CASE( "bits/streaming/iomanip",
           "Bits streaming io manipulators." )
{
    BitMask m( 0x1234 );
    std::ostringstream os;
    os << m;
    REQUIRE( os.str() == "0x1234" );
}
} // namespace bits_streaming_iomanip

/*
 * end of file
 */

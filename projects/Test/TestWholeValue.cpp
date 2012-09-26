/*
 * TestWholeValue.cpp
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
#include "whole_value.h"

#include <iomanip>

// Define fundamental value types:
WV_DEFINE_VALUE_TYPE( Value, int )

// Define 'domain' value types:
WV_DEFINE_TYPE( MyValue, Value )

#ifdef G_WV_COMPILER_IS_MSVC6
    // VC6 has no argument dependent lookup (ADL)
    using namespace wv;
#endif

/*
 * Please note: the Catch test framework cannot handle quantity::operator==(),
 * hence we use REQUIRE( !!( expression-involving-whole-value ) ) where needed.
 */

/* ----------------------------------------------------------------------------
 * whole_value:
 */

TEST_CASE( "wv/construction",
           "Whole value construction." )
{
    SECTION( "wv/construction/default",
             "Default constructor creates value with default constructed base type." )
    {
        Value v;
        Value::value_type x = Value::value_type();
        REQUIRE( x == 0 );
        REQUIRE( v.value() == x );
    }

    SECTION( "wv/construction/initialise",
             "Initializing constructor create whole value with given value." )
    {
        Value v( 1234 );
        REQUIRE( v.value() == 1234 );
    }

    SECTION( "wv/construction/copy-assign",
             "Copy-assignment constructor creates whole value with given value." )
    {
        Value a( 1234 );
        Value b( a  );
        Value c = a;
        REQUIRE( a.value() == b.value() );
        REQUIRE( a.value() == c.value() );
    }
}

TEST_CASE( "wv/assignment",
           "Whole value assignment." )
{
    Value a( 1234 );
    Value b( 3333 );
    a = b;
    REQUIRE( a.value() == 3333 );
    REQUIRE( a.value() == b.value() );
}

namespace wv_streaming {

WV_ADD_STREAM_INSERTION_OPERATOR( Value )

TEST_CASE( "wv/streaming",
           "Whole value streaming." )
{
    MyValue m( 1234 );
    std::ostringstream os;
    os << m;
    REQUIRE( os.str() == "1234" );
}
} // namespace bits_streaming

namespace bits_streaming_iomanip {

WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP( Value, std::hex << std::showbase )

TEST_CASE( "wv/streaming/iomanip",
           "Whole value streaming io manipulators." )
{
    MyValue m( 0x1234 );
    std::ostringstream os;
    os << m;
    REQUIRE( os.str() == "0x1234" );
}
} // namespace bits_streaming_iomanip

/*
 * end of file
 */

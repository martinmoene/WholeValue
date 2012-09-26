/*
 * TestSafeBool.cpp
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
WV_DEFINE_SAFE_BOOL_TYPE ( SafeBool )

// Define 'domain' value types:
WV_DEFINE_TYPE( Truth, SafeBool )
WV_DEFINE_TYPE( Truth2, SafeBool )

#ifdef G_WV_COMPILER_IS_MSVC6
    // VC6 has no argument dependent lookup (ADL)
    using namespace wv;
#endif

/*
 * Please note: the Catch test framework cannot handle quantity::operator==(),
 * hence we use REQUIRE( !!( expression-involving-whole-value ) ) where needed.
 */

/* ----------------------------------------------------------------------------
 * safebool:
 */

TEST_CASE( "safebool/construction",
           "Safe bool construction" )
{
    SECTION( "safebool/construction/default",
             "Default constructor creates value with default constructed base type." )
    {
        Truth t;
        REQUIRE( t.value() == false );
    }

    SECTION( "safebool/construction/initialise",
             "Initializing constructor create whole value with given value." )
    {
        Truth t( true );
        REQUIRE( t.value() == true );
    }

    SECTION( "safebool/construction/copy-assign",
             "Copy-assignment constructor creates whole value with given value." )
    {
        Truth a( true );
        Truth b( a  );
        Truth c = a;
        REQUIRE( a.value() == b.value() );
        REQUIRE( a.value() == c.value() );
    }
}

TEST_CASE( "safebool/comparison",
           "Safe bool comparison." )
{
    SECTION( "safebool/conversion/eq",
             "" )
    {
        Truth a, b, c(true);
        REQUIRE( !!( a == b ) );
        REQUIRE(  !( a == c ) );
    }

    SECTION( "safebool/conversion/ne",
             "" )
    {
        Truth a, b, c(true);
        REQUIRE( !!( a != c ) );
        REQUIRE(  !( a != b ) );
    }
}

TEST_CASE( "safebool/conversion",
           "Safe conversion." )
{
    SECTION( "safebool/conversion/to-bool",
             "Allow conversion to bool." )
    {
        Truth a;
        Truth b( true );
        bool x = a;
        bool y = b;
        REQUIRE( x == a.value() );
        REQUIRE( y == b.value() );
    }

    SECTION( "safebool/conversion/to-non-bool",
             "Disallow conversion to non-bool (uncomment)." )
    {
        Truth a;
        // uncomment to activate compile-time error:
        //int x = a;
    }

    SECTION( "safebool/conversion/comparation",
             "Disallow comparation between different safe bool types (uncomment)." )
    {
        Truth a;
        Truth2 b;
        // uncomment to activate compile-time error:
        //bool r = a == b;
    }
}

namespace safebool_streaming {

WV_ADD_STREAM_INSERTION_OPERATOR( SafeBool )

TEST_CASE( "safebool/streaming",
           "Safe streaming." )
{
    Truth f( false );
    Truth t( true );
    std::ostringstream os;
    os << f << "," << t;
    REQUIRE( os.str() == "0,1" );
}
} // namespace safebool_streaming

namespace safebool_streaming_iomanip {

WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP( SafeBool, std::hex << std::boolalpha )

TEST_CASE( "safebool/streaming/iomanip",
           "Safe streaming io manipulators." )
{
    Truth f( false );
    Truth t( true );
    std::ostringstream os;
    os << f << "," << t;
    REQUIRE( os.str() == "false,true" );
}
} // namespace safebool_streaming_iomanip

/*
 * end of file
 */

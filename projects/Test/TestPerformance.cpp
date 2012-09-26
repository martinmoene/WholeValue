
//#define WV_USE_BOOST_OPERATORS
#define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE
#include "whole_value.h"

#include <iostream>
#include <time.h>

WV_DEFINE_ARITHMETIC_TYPE ( Real, double )
WV_DEFINE_TYPE( Factor, Real )

WV_ADD_STREAM_INSERTION_OPERATOR( Real )

const int meg = 1000000;
const int k = 200; // 60; // scale factor - make it bigger for faster machines
//const int k = 10; // 60; // scale factor - make it bigger for faster machines

template< typename T1, typename T2, typename T3 >
T1 do_work( T1 v1, T2 v2, T3 v3 )
{
	// Do a bunch of work.  We don't really care about the answer;
	// this is just to exercise addition, subtraction, multiplication, and division.
	T1 x1 = v1;
	T2 x2 = v2;
	T3 x3 = v3;
	for( int i = 0; i < meg; i++ )
	{
		for( int j = 0; j < k; j++ )
		{
			x2 = v2;
//			x2 = -x2 - v2;
			x3 *= 1.00002;
			x1 += x2 - x3;
		}
	}
	return x1;
}

const Factor biga( 2 );
const Factor bigb( 0.5 );

Factor do_more_work(
			Factor v1,
			Factor v2,
			Factor v3 )
{
	// Do a bunch of work.  We don't really care about the answer;
	// this is just to exercise addition, subtraction, multiplication, and division.
	Factor x1 = v1;
	Factor x2 = v2;
	Factor x3 = v3;
	for( int i = 0; i < meg; i++ )
	{
		for( int j = 0; j < k; j++ )
		{
			x2 = v2;
//			x2 = -x2 - v2;
			x3 *= wv::to_real( biga * 1.00002 + bigb );
			x1 += x2 - x3;
		}
	}
	return x1;
}

int main()
{
    std::cout << "Performance test of whole_value library." << std::endl;

	clock_t t0 = clock();

	// do some work with doubles
	volatile double d = do_work( 0.1, 0.2, 0.3 );

	clock_t t1 = clock();

	// do exact same work with Factor
//	Real s = do_work( Real(0.1), Real(0.2), Real(0.3) );
	Real s = do_work( Real(0.1), Real(0.2), Real(0.3) );

	clock_t t2 = clock();

	Factor s2 = do_more_work( Factor(0.1), Factor(0.2), Factor(0.3) );

	clock_t t3 = clock();

	Factor s3 = do_more_work( Factor(0.1), Factor(0.2), Factor(0.3) );

	clock_t t4 = clock();

	const double cps = CLOCKS_PER_SEC;
	std::cout << "one double work loop =     "    << (t1-t0)/cps/k << " usec" << std::endl;
	std::cout << "one Real loop =   "             << (t2-t1)/cps/k << " usec " << std::endl;
	std::cout << "one Factor work loop =   "      << (t3-t2)/cps/k << " usec " << std::endl;
	std::cout << "one whole_value++ work loop = " << (t4-t3)/cps/k << " usec " << std::endl;

	std::cout << "d = " << d << std::endl;
	std::cout << "s = " << s << std::endl;

    return 0;
}

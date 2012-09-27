/*
 * whole_value.h
 * whole_value, bits, arithmetic, quantity and safe_bool classes.
 *
 * Created by Martin on 16 September 2012.
 * Copyright 2012 Universiteit Leiden. All rights reserved.
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

/*
 * Macros to control behaviour:
 * - WV_USE_BOOST_OPERATORS: Define macro WV_USE_BOOST_OPERATORS to use
 *   Boost.Operators to defined part of the bitwise function for bits<> and
 *   arithmetic functions for arithmetic<> and quantity<>.
 *
 * - WV_DEFINE_OPERATORS_IN_TERMS_OF_A_MINIMAL_NUMBER_OF_FUNDAMENTAL_OPERATORS
 *   If WV_USE_BOOST_OPERATORS is NOT defined, this controls whether the
 *   operators are defined in terms of fundamental operators such as op=(), or
 *   directly defined in terms of the operator for the underlying value_type.
 *   For VC6 it has a negative performance impact to define operators in terms
 *   of a few fundamental operators.
 *
 * See:
 * - http://www.twonine.co.uk/articles/GenProgGoesLive.pdf
 * - http://www.artima.com/cppsource/safeboolP.html
 * - http://stackoverflow.com/questions/6242768/is-the-safe-bool-idiom-obsolete
 * - http://stackoverflow.com/questions/3940908/can-the-safe-bool-idiom-be-implemented-without-having-to-derive-from-a-safe-bool
 */

#ifndef G_WV_WHOLE_VALUE_H_INCLUDED
#define G_WV_WHOLE_VALUE_H_INCLUDED

/**
 * create whole value type.
 */
#define WV_DEFINE_VALUE_TYPE( type_name, underlying_type ) \
    struct type_name##_tag{}; \
    typedef ::wv::whole_value<underlying_type, type_name##_tag> type_name;

/**
 * create value type with bitwise operations.
 */
#define WV_DEFINE_BITS_TYPE( type_name, underlying_type ) \
    struct type_name##_tag{}; \
    typedef ::wv::bits<underlying_type, type_name##_tag> type_name;

/**
 * create arithmetic value type.
 */
#define WV_DEFINE_ARITHMETIC_TYPE( type_name, underlying_type ) \
    struct type_name##_tag{}; \
    typedef ::wv::arithmetic<underlying_type, type_name##_tag> type_name;

/**
 * create quantity value type.
 */
#define WV_DEFINE_QUANTITY_TYPE( type_name, underlying_type ) \
    struct type_name##_tag{}; \
    typedef ::wv::quantity<underlying_type, type_name##_tag> type_name;

/**
 * create a value-type sub type.
 */
#define WV_DEFINE_TYPE( sub, super ) \
    class sub : public super { \
        public: sub() : super() {} \
        public: sub(super const & x) : super(x) {} \
        public: explicit sub(value_type const x) : super(x) {} \
    };

/**
 * create safe bool type.
 */
#define WV_DEFINE_SAFE_BOOL_TYPE( type_name ) \
    struct type_name##_tag{}; \
    typedef wv::safe_bool<type_name##_tag> type_name;

/**
 * add streaming operator to whole value type.
 */
#define WV_ADD_STREAM_INSERTION_OPERATOR( type_name ) \
    inline std::ostream& operator<<( std::ostream & os, type_name const & obj ) \
    { \
        return os << obj.value(); \
    }

/**
 * add streaming operator including manipulator(s) to whole value type.
 */
#define WV_ADD_STREAM_INSERTION_OPERATOR_IOMANIP( type_name, manipulators ) \
    inline std::ostream& operator<<( std::ostream & os, type_name const & obj ) \
    { \
        std::ios::fmtflags oldflags = os.flags(); \
        os << manipulators << obj.value(); \
        os.flags( oldflags ); \
        return os; \
    }

// --------------------------------------------------------------------------

#if defined( _MSC_VER )
# define G_WV_COMPILER_IS_MSVC
# if ( _MSC_VER >= 1200 ) && ( _MSC_VER < 1300 )
#  define G_WV_COMPILER_IS_MSVC6
# endif
#endif

#if defined( WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE )
# define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_VALUE
# define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS
# define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_ARITHMETIC
# define WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY
#endif

#if defined( WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_VALUE )
# define WV_EXPLICIT_WHOLE_VALUE
#else
# define WV_EXPLICIT_WHOLE_VALUE explicit
#endif

#if defined( WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_BITS )
# define WV_EXPLICIT_BITS
#else
# define WV_EXPLICIT_BITS explicit
#endif

#if defined( WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_ARITHMETIC )
# define WV_EXPLICIT_ARITHMETIC
#else
# define WV_EXPLICIT_ARITHMETIC explicit
#endif

#if defined( WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY )
# define WV_EXPLICIT_QUANTITY
#else
# define WV_EXPLICIT_QUANTITY explicit
#endif

#if defined( WV_USE_BOOST_OPERATORS )
# if defined( G_WV_COMPILER_IS_MSVC6 )
#  include <cstdlib>
   namespace std { using ::abort; }
#  define BOOST_FORCE_SYMMETRIC_OPERATORS
# endif
# include <boost/operators.hpp>
#endif

#include <algorithm> // std::swap()
#include <cstdlib>   // std::abs()

/**
 * namespace for whole value library.
 */
namespace wv {

/**
 * value type without any operations other than assignment.
 */
template< typename T, typename TG >
class whole_value
{
public:
    typedef T value_type;

    whole_value() : m_value( value_type() ) {}

    WV_EXPLICIT_WHOLE_VALUE whole_value( value_type const value ) : m_value( value ) {}

    whole_value( whole_value const & other ) : m_value( other.m_value ) {}

#ifdef G_WV_COMPILER_IS_MSVC6
    whole_value & operator=( whole_value const & other )
    {
        m_value = other.m_value;
        return *this;
    }
#else
    whole_value & operator=( whole_value other )
    {
      swap( *this, other );
      return *this;
    }
#endif

    friend void swap( whole_value & first, whole_value & second )
    {
        using std::swap;
        swap( first.m_value, second.m_value );
    }

    /*
     * Conversion to underlying type is needed so that operations can be added
     * as freestanding functions/operators.
     */
    value_type value() const { return m_value; }

#ifdef G_WV_COMPILER_IS_MSVC6
    template<typename U>
    U as( U ) const { return static_cast<U>( m_value ); }
#else
    template<typename U>
    U as() const { return static_cast<U>( m_value ); }
#endif

private:
    value_type m_value;
};

/**
 * type with bitwise operations.
 */
template< typename T, typename TG >
class bits
#ifdef WV_USE_BOOST_OPERATORS
    : boost::equality_comparable< bits<T,TG>
    , boost::bitwise1< bits<T,TG>
    , boost::left_shiftable2< bits<T,TG>, int
    , boost::right_shiftable2< bits<T,TG>, int
    > > > >
#endif
{
public:
    typedef T value_type;

    bits() : m_value( value_type() ) {}

    WV_EXPLICIT_BITS bits( value_type const value ) : m_value( value ) {}

    bits( bits const & other ) : m_value( other.m_value ) {}

#ifdef G_WV_COMPILER_IS_MSVC6
    bits & operator=( bits const & other )
    {
        m_value = other.m_value;
        return *this;
    }
#else
    bits & operator=( bits other )
    {
      swap( *this, other );
      return *this;
    }
#endif

    friend void swap( bits & first, bits & second )
    {
        using std::swap;
        swap( first.m_value, second.m_value );
    }

    /*
     * Conversion to underlying type is needed so that operations can be added
     * as freestanding functions/operators.
     */
    value_type value() const { return m_value; }

    /*
     * equality operator; defined as friend to enable 7==x unless
     * initializing constructor is declared explicit.
     */
    friend bool operator==( bits const & x, bits const & y )
    {
        return x.m_value == y.m_value;
    }

    bits & operator^=( bits const & other )
    {
        this->m_value ^= other.m_value;
        return *this;
    }

    bits & operator&=( bits const & other )
    {
        this->m_value &= other.m_value;
        return *this;
    }

    bits & operator|=( bits const & other )
    {
        this->m_value |= other.m_value;
        return *this;
    }

    bits & operator<<=( int const n )
    {
        this->m_value <<= n;
        return *this;
    }

    bits & operator>>=( int const n )
    {
        this->m_value >>= n;
        return *this;
    }

#ifdef WV_USE_BOOST_OPERATORS
    // rest of operators generated by Boost.Operators
#else
# if WV_DEFINE_OPERATORS_IN_TERMS_OF_A_MINIMAL_NUMBER_OF_FUNDAMENTAL_OPERATORS

    friend bool operator!=( bits const & x, bits const & y ) { return ! ( x == y ); }

    friend bits operator^ ( bits x, bits const & y ) { return x ^= y; }
    friend bits operator| ( bits x, bits const & y ) { return x |= y; }
    friend bits operator& ( bits x, bits const & y ) { return x &= y; }

    friend bits operator<<( bits x,  int const   n ) { return x <<= n; }
    friend bits operator>>( bits x,  int const   n ) { return x >>= n; }

# else
    /*
     * For VC6, the following implementation is significantly faster than
     * using Boost.Operators, or defining in terms of fundamental operators.
     */
    friend bool operator!=( bits const & x, bits const & y ) { return x.m_value != y.m_value; }

    friend bits operator^ ( bits const & x, bits const & y ) { return bits( x.m_value ^ y.m_value ); }
    friend bits operator| ( bits const & x, bits const & y ) { return bits( x.m_value | y.m_value ); }
    friend bits operator& ( bits const & x, bits const & y ) { return bits( x.m_value & y.m_value ); }

    friend bits operator<<( bits const & x,  int const   n ) { return bits(x) <<= n; }
    friend bits operator>>( bits const & x,  int const   n ) { return bits(x) >>= n; }
# endif
#endif

private:
    value_type m_value;
};

/**
 * arithmetic type (dimensionless).
 */
template< typename T, typename TG >
class arithmetic
#ifdef WV_USE_BOOST_OPERATORS
    : boost::unit_steppable< arithmetic<T,TG>
    , boost::totally_ordered1< arithmetic<T,TG>
    , boost::integer_arithmetic1< arithmetic<T,TG>
    > > >
#endif
{
public:
    typedef T value_type;

    arithmetic() : m_value( value_type() ) {}

    WV_EXPLICIT_ARITHMETIC arithmetic( value_type const value ) : m_value( value ) {}

    arithmetic( arithmetic const & other ) : m_value( other.m_value ) {}

#ifdef G_WV_COMPILER_IS_MSVC6
    arithmetic & operator=( arithmetic const & other )
    {
        m_value = other.m_value;
        return *this;
    }
#else
    arithmetic & operator=( arithmetic other )
    {
      swap( *this, other );
      return *this;
    }
#endif

    friend void swap( arithmetic & first, arithmetic & second )
    {
        using std::swap;
        swap( first.m_value, second.m_value );
    }

    /*
     * Conversion to underlying type is needed so that operations can be added
     * as freestanding functions/operators.
     */
    value_type value() const { return m_value; }

#ifdef G_WV_COMPILER_IS_MSVC6
    template<typename U>
    U as( U ) const { return static_cast<U>( m_value ); }
#else
    template<typename U>
    U as() const { return static_cast<U>( m_value ); }
#endif

    /*
     * equality operator; defined as friend to enable 7==x unless
     * initializing constructor is declared explicit.
     */
    friend bool operator==( arithmetic const & x, arithmetic const & y )
    {
        return x.m_value == y.m_value;
    }

    /*
     * less-than operator; defined as friend to enable 7<x unless
     * initializing constructor is declared explicit.
     */
    friend bool operator<( arithmetic const & x, arithmetic const & y )
    {
        return x.m_value < y.m_value;
    }

    arithmetic operator+() const
    {
        return *this;
    }

    arithmetic operator-() const
    {
        return arithmetic( -m_value );
    }

    arithmetic & operator++()
    {
        ++m_value;
        return *this;
    }

    arithmetic & operator--()
    {
        --m_value;
        return *this;
    }

    arithmetic & operator+=( arithmetic const & other )
    {
        m_value += other.m_value;
        return *this;
    }

    arithmetic & operator-=( arithmetic const & other )
    {
        m_value -= other.m_value;
        return *this;
    }

    arithmetic & operator*=( arithmetic const & other )
    {
        m_value *= other.m_value;
        return *this;
    }

    arithmetic & operator/=( arithmetic const & other )
    {
        m_value /= other.m_value;
        return *this;
    }

    arithmetic & operator%=( arithmetic const & other )
    {
        m_value %= other.m_value;
        return *this;
    }

#ifdef WV_USE_BOOST_OPERATORS
    // rest of operators generated by Boost.Operators
#else
    arithmetic operator++( int ) { arithmetic tmp(*this); ++*this; return tmp; }
    arithmetic operator--( int ) { arithmetic tmp(*this); --*this; return tmp; }

# if WV_DEFINE_OPERATORS_IN_TERMS_OF_A_MINIMAL_NUMBER_OF_FUNDAMENTAL_OPERATORS

    friend bool operator!=( arithmetic const & x, arithmetic const & y ) { return ! ( x == y ); }

    friend bool operator> ( arithmetic const & x, arithmetic const & y ) { return y < x; }
    friend bool operator<=( arithmetic const & x, arithmetic const & y ) { return ! ( y < x ); }
    friend bool operator>=( arithmetic const & x, arithmetic const & y ) { return ! ( x < y ); }

    friend arithmetic operator+( arithmetic x, arithmetic const & y ) { return x += y; }
    friend arithmetic operator-( arithmetic x, arithmetic const & y ) { return x -= y; }

    friend arithmetic operator*( arithmetic x, arithmetic const & y ) { return x *= y; }
    friend arithmetic operator/( arithmetic x, arithmetic const & y ) { return x /= y; }
    friend arithmetic operator%( arithmetic x, arithmetic const & y ) { return x %= y; }

# else
    /*
     * For VC6, the following implementation is significantly faster than
     * using Boost.Operators, or defining in terms of fundamental operators.
     */
    friend bool operator!=( arithmetic const & x, arithmetic const & y ) { return x.m_value != y.m_value; }

    friend bool operator> ( arithmetic const & x, arithmetic const & y ) { return x.m_value >  y.m_value; }
    friend bool operator<=( arithmetic const & x, arithmetic const & y ) { return x.m_value <= y.m_value; }
    friend bool operator>=( arithmetic const & x, arithmetic const & y ) { return x.m_value >= y.m_value; }

    friend arithmetic operator+( arithmetic const & x, arithmetic const & y ) { return arithmetic( x.m_value + y.m_value ); }
    friend arithmetic operator-( arithmetic const & x, arithmetic const & y ) { return arithmetic( x.m_value - y.m_value ); }

    friend arithmetic operator*( arithmetic const & x, arithmetic const & y ) { return arithmetic( x.m_value * y.m_value ); }
    friend arithmetic operator/( arithmetic const & x, arithmetic const & y ) { return arithmetic( x.m_value / y.m_value ); }
    friend arithmetic operator%( arithmetic const & x, arithmetic const & y ) { return arithmetic( x.m_value % y.m_value ); }
# endif
#endif // WV_USE_BOOST_OPERATORS

private:
    value_type m_value;
};

/**
 * quantity type (more like having 'dimension', e.g. no T * T, T * U).
 */
template< typename T, typename TG >
class quantity
{
public:
    typedef T value_type;

    quantity() : m_value( value_type() ) {}

    WV_EXPLICIT_QUANTITY quantity( value_type const value ) : m_value( value ) {}

    quantity( quantity const & other ) : m_value( other.m_value ) {}

#ifdef G_WV_COMPILER_IS_MSVC6
    quantity & operator=( quantity const & other )
    {
        m_value = other.m_value;
        return *this;
    }
#else
    quantity & operator=( quantity other )
    {
      swap( *this, other );
      return *this;
    }
#endif

    friend void swap( quantity & first, quantity & second )
    {
        using std::swap;
        swap( first.m_value, second.m_value );
    }

    /*
     * Conversion to underlying type is needed so that operations can be added
     * as freestanding functions/operators.
     */
    value_type value() const { return m_value; }

#ifdef G_WV_COMPILER_IS_MSVC6
    template<typename U>
    U as( U ) const { return static_cast<U>( m_value ); }
#else
    template<typename U>
    U as() const { return static_cast<U>( m_value ); }
#endif

    /*
     * equality operator; defined as friend to enable 7==x unless
     * initializing constructor is declared explicit.
     */
    friend bool operator==( quantity const & x, quantity const & y )
    {
        return x.m_value == y.m_value;
    }

    /*
     * less-than operator; defined as friend to enable 7<x unless
     * initializing constructor is declared explicit.
     */
    friend bool operator<( quantity const & x, quantity const & y )
    {
        return x.m_value < y.m_value;
    }

    quantity operator+() const
    {
        return *this;
    }

    quantity operator-() const
    {
        return quantity( -this->m_value );
    }

    quantity & operator++()
    {
        ++this->m_value;
        return *this;
    }

    quantity & operator--()
    {
        --this->m_value;
        return *this;
    }

    quantity & operator+=( quantity const & other )
    {
        this->m_value += other.m_value;
        return *this;
    }

    quantity & operator-=( quantity const & other )
    {
        this->m_value -= other.m_value;
        return *this;
    }

#ifndef WV_ALLOW_CONVERSION_FROM_UNDERLYING_TYPE_FOR_QUANTITY
private:
#endif
    quantity & operator*=( value_type const value )
    {
        this->m_value *= value;
        return *this;
    }

    quantity & operator/=( value_type const value )
    {
        this->m_value /= value;
        return *this;
    }

    quantity & operator%=( value_type const value )
    {
        this->m_value %= value;
        return *this;
    }

public:
    quantity operator++( int ) { quantity tmp(*this); ++*this; return tmp; }
    quantity operator--( int ) { quantity tmp(*this); --*this; return tmp; }

#if WV_DEFINE_OPERATORS_IN_TERMS_OF_A_MINIMAL_NUMBER_OF_FUNDAMENTAL_OPERATORS

    friend bool operator!=( quantity const & x, quantity const & y ) { return ! ( x == y ); }

    friend bool operator> ( quantity const & x, quantity const & y ) { return y < x; }
    friend bool operator<=( quantity const & x, quantity const & y ) { return ! ( y < x ); }
    friend bool operator>=( quantity const & x, quantity const & y ) { return ! ( x < y ); }

    friend quantity operator+( quantity x, quantity const & y ) { return x += y; }
    friend quantity operator-( quantity x, quantity const & y ) { return x -= y; }

    friend quantity operator*( quantity x, value_type const y ) { return x *= y; }
    friend quantity operator*( value_type const x, quantity y ) { return y *= x; }

    friend quantity operator/( quantity x, value_type const y ) { return x /= y; }
    friend quantity operator%( quantity x, value_type const y ) { return x %= y; }

#else
    /*
     * For VC6, the following implementation is significantly faster than
     * using Boost.Operators, or defining in terms of fundamental operators.
     */
    friend bool operator!=( quantity const & x, quantity const & y ) { return x.m_value != y.m_value; }

    friend bool operator> ( quantity const & x, quantity const & y ) { return x.m_value >  y.m_value; }
    friend bool operator<=( quantity const & x, quantity const & y ) { return x.m_value <= y.m_value; }
    friend bool operator>=( quantity const & x, quantity const & y ) { return x.m_value >= y.m_value; }

    friend quantity operator+( quantity const & x, quantity const & y ) { return quantity( x.m_value + y.m_value ); }
    friend quantity operator-( quantity const & x, quantity const & y ) { return quantity( x.m_value - y.m_value ); }

    friend quantity operator*( quantity const & x, value_type const y ) { return quantity( x.m_value * y         ); }
    friend quantity operator*( value_type const x, quantity const & y ) { return quantity( x         * y.m_value ); }

    friend quantity operator/( quantity const & x, value_type const y ) { return quantity( x.m_value / y ); }
    friend quantity operator%( quantity const & x, value_type const y ) { return quantity( x.m_value % y ); }
#endif

private:
    value_type m_value;
};

/**
 *safe bool type.
 */
template < typename TG >
class safe_bool : public whole_value< bool, TG >
{
private:
    /**
     * safe boolean operator return type.
     */
    typedef void ( safe_bool::*bool_type )() const;

    /**
     * address of method used as 'boolean',
     * inaccessibility of method used for prevented comparison error message.
     */
    void ERROR_this_type_does_not_support_comparisons() const {}

public:
    /**
     * default/initializing constructor.
     */
    explicit safe_bool( bool value = false ) : whole_value<bool,TG>( value ) {}

    /**
     * safe conversion to 'boolean';
     * in C++11, this could be: explicit operator bool() const;
     */
    operator bool_type() const
    {
        return this->value() ? &safe_bool::ERROR_this_type_does_not_support_comparisons : 0;
    }
};

#ifdef G_WV_COMPILER_IS_MSVC6
// VC6 cannot provide the helpful error message as below.
#else
/**
 * prevent equality comparison between safe bool pairs of different type.
 */
template <typename T, typename U>
inline
bool operator==( safe_bool<T> const & x, safe_bool<U> const & y)
{
    x.ERROR_this_type_does_not_support_comparisons();
    return false;
}

/**
 * prevent inequality comparison between any safe bool pairs.
 */
template <typename T, typename U>
inline
bool operator!=( safe_bool<T> const & x, safe_bool<U> const & y )
{
    x.ERROR_this_type_does_not_support_comparisons();
    return false;
}
#endif

/**
 * allow equality comparison between safe bool pairs of equal type.
 */
template <typename T>
inline
bool operator==( safe_bool<T> const & x, safe_bool<T> const & y )
{
    return x.value() == y.value();
}

/**
 * allow inequality comparison between safe bool pairs of equal type.
 */
template <typename T>
inline
bool operator!=( safe_bool<T> const & x, safe_bool<T> const & y )
{
    return x.value() != y.value();
}

#ifdef G_WV_COMPILER_IS_MSVC6

namespace detail {

    /**
     * abs() for VC6.
     */
    template <typename T>
    inline T abs( T const x )
    {
        return detail::Abs<T>::apply( x );
    }

    template <typename T>
    struct Abs
    {
        static T apply( T const x ) { return x < 0 ? -x: +x; }
    };
} // namespace detail
#endif

/**
 * return absolute value.
 */
template < typename T, typename U >
inline whole_value<T,U> abs( whole_value<T,U> const & x )
{
#ifdef G_WV_COMPILER_IS_MSVC6
    return whole_value<T,U>( detail::abs( x.value() ) );
#else
    return whole_value<T,U>( std::abs( x.value() ) );
#endif
}

/**
 * return absolute value.
 */
template < typename T, typename U >
inline arithmetic<T,U> abs( arithmetic<T,U> const & x )
{
#ifdef G_WV_COMPILER_IS_MSVC6
    return arithmetic<T,U>( detail::abs( x.value() ) );
#else
    return arithmetic<T,U>( std::abs( x.value() ) );
#endif
}

/**
 * return absolute value.
 */
template < typename T, typename U >
inline quantity<T,U> abs( quantity<T,U> const & x )
{
#ifdef G_WV_COMPILER_IS_MSVC6
    return quantity<T,U>( detail::abs( x.value() ) );
#else
    return quantity<T,U>( std::abs( x.value() ) );
#endif
}

/**
 * value as underlying type.
 */
template < typename T, typename U > inline T to_value( whole_value<T,U> const & x ) { return x.value(); }
template < typename T, typename U > inline T to_value(        bits<T,U> const & x ) { return x.value(); }
template < typename T, typename U > inline T to_value(  arithmetic<T,U> const & x ) { return x.value(); }
template < typename T, typename U > inline T to_value(    quantity<T,U> const & x ) { return x.value(); }

/**
 * value as integer (long).
 */
template < typename T, typename U > inline long to_integer( whole_value<T,U> const & x ) { return x.value(); }
template < typename T, typename U > inline long to_integer(  arithmetic<T,U> const & x ) { return x.value(); }
template < typename T, typename U > inline long to_integer(    quantity<T,U> const & x ) { return x.value(); }

/**
 * value as real (double).
 */
template < typename T, typename U > inline double to_real ( whole_value<T,U> const & x ) { return x.value(); }
template < typename T, typename U > inline double to_real (  arithmetic<T,U> const & x ) { return x.value(); }
template < typename T, typename U > inline double to_real (    quantity<T,U> const & x ) { return x.value(); }

} // namespace wv

#endif // G_WV_WHOLE_VALUE_H_INCLUDED

/*
 * end of file
 */

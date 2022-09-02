#pragma once

#include <variant>

namespace lrvalue
{
  namespace detail
  {
    template< typename... Functions >
    struct Overload : Functions...
    {
        using Functions::operator()...;
        Overload( Functions &&... functions ) : Functions( std::forward< Functions >( functions ) )... {}
    };

    template< typename T >
    class Reference
    {
      T &_value;
    public:
      Reference( T &value ) : _value( value ) {}
      operator const T &() const { return _value; }
      operator T &() { return _value; }
    };

    template< typename T >
    class Reference< const T >
    {
      const T &_value;
    public:
      Reference( const T &value ) : _value( value ) {}
      operator const T &() const { return _value; }
    };

    template< typename T >
    class Value
    {
      T _value;
    public:
      Value( T &&value ) : _value( std::move( value ) ) {}
      operator const T &() const { return _value; }
      operator T &() { return _value; }
    };
  }

  template< typename T >
  class LRValue
  {
    using type = std::decay_t< T >;
    using reference_type = detail::Reference< type >;
    using value_type = detail::Value< type >;
    std::variant< reference_type, value_type > _value;

  public:
    LRValue( type &value ) : _value( reference_type( value ) ) {}
    LRValue( type &&value ) : _value( value_type( std::move( value ) ) ) {}

    operator const type &() const { return std::visit( []( const auto &v ) -> const type & { return v; }, _value ); }
    operator type &() { return std::visit( []( auto &v ) -> type & { return v; }, _value ); }

    operator LRValue< const type > ()
    { return std::visit( []( auto &v ) -> LRValue< const type >{ return { v }; }, _value ); }
  };

  template< typename T >
  class LRValue< const T >
  {
    using type = std::decay_t< T >;
    using reference_type = detail::Reference< const type >;
    using value_type = detail::Value< type >;
    std::variant< reference_type, value_type > _value;

  public:
    LRValue( type &value ) : _value( reference_type( value ) ) {}
    LRValue( const type &value ) : _value( reference_type( value ) ) {}
    LRValue( type &&value ) : _value( value_type( std::move( value ) ) ) {}

    operator const type &() const
    { return std::visit( []( const auto &v ) -> const type & { return v; }, _value ); }
  };

  template< typename T >
  LRValue( T && ) -> LRValue< T >;

  template< typename T >
  LRValue( const T & ) -> LRValue< const T >;

  template< typename T >
  using Storage = std::variant< detail::Reference< T >, detail::Value< T > >;

  template< typename T >
  T &get( Storage< T > &storage )
  { return std::visit( []( auto &d ) -> T & { return d; }, storage ); }

  template< typename T >
  const T &get( const Storage< T > &storage )
  { return std::visit( []( const auto &d ) -> const T & { return d; }, storage ); }
}

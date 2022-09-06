#pragma once

#include <variant>
#include <functional>

namespace lrvalue
{
  template< typename T >
  class LRValue
  {
    using type = std::decay_t< T >;
    using reference_type = std::reference_wrapper< type >;
    using value_type = type;
    std::variant< reference_type, value_type > _value;

  public:
    LRValue( type &value ) : _value( reference_type( value ) ) {}
    // LRValue( const type &value ) : _value( value_type( value ) ) {}
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
    using reference_type = std::reference_wrapper< const type >;
    using value_type = type;
    std::variant< reference_type, value_type > _value;

  public:
    LRValue( type &value ) : _value( reference_type( value ) ) {}
    LRValue( const type &value ) : _value( reference_type( value ) ) {}
    LRValue( type &&value ) : _value( value_type( std::move( value ) ) ) {}

    operator const type &() const { return std::visit( []( const auto &v ) -> const type & { return v; }, _value ); }
    // operator type() const { return std::visit( []( const auto &v ) -> type { return v; }, _value ); }
  };

  template< typename T >
  LRValue( T && ) -> LRValue< T >;

  template< typename T >
  LRValue( const T & ) -> LRValue< const T >;

  template< typename T >
  using Storage = std::variant< std::reference_wrapper< T >, T >;

  template< typename T >
  T &get( Storage< T > &storage )
  { return std::visit( []( auto &d ) -> T & { return d; }, storage ); }

  template< typename T >
  const T &get( const Storage< T > &storage )
  { return std::visit( []( const auto &d ) -> const T & { return d; }, storage ); }
}

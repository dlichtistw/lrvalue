#include <string>
#include <iostream>

#include "lrvalue.h"

using namespace lrvalue;

namespace
{
  class MyClass
  {
    using type = const std::string;
    using reference_type = detail::Reference< type >;
    using value_type = detail::Value< type >;

  public:
    explicit MyClass( std::string &value ) : _storage( reference_type( value ) ) {}
    explicit MyClass( const std::string &value ) : _storage( reference_type( value ) ) {}
    explicit MyClass( std::string &&value ) : _storage( value_type( std::move( value ) ) ) {}

    void print() const
    {
        std::cout << get( _storage ) << std::endl;
    }

  private:
    Storage< type > _storage;
  };
}

int main()
{
  {
    std::string s{ "hello" };
    MyClass myObject{ s };
    myObject.print();
  }

  {
    const std::string s{ "hello" };
    MyClass myObject{ s };
    myObject.print();
  }

  {
    MyClass myObject{ std::string{ "hello" } };
    myObject.print();
  }
}

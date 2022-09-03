#pragma once

#include <map>
#include <iostream>

namespace lrvalue::test
{
  enum class Call
  {
    DefaultConstructor,
    CopyConstructor,
    MoveConstructor,
    CopyAssignment,
    MoveAssignment,
    Destructor,
  };

  std::ostream &operator<<( std::ostream &stream, const Call call );

  class Sentinel
  {
    using Counter = std::map< Call, int >;
    static Counter _globalCounter;
    mutable Counter _instanceCounter;

    void count( const Call call );

  public:
    Sentinel();
    Sentinel( const Sentinel & );
    Sentinel( Sentinel && );

    Sentinel &operator=( const Sentinel & );
    Sentinel &operator=( Sentinel && );

    ~Sentinel();

    static int calls();
    static int calls( const Call call );

    int operator()() const;
    int operator()( const Call call ) const;

    static void clearGlobal();
    void clearThis();
  };
}

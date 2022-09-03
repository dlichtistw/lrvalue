#include <cassert>

#include "lrvalue.h"
#include "test_util.h"

using namespace lrvalue;
using namespace lrvalue::test;

namespace
{
  void f( LRValue< const Sentinel > ) {}

  void callWithConstant()
  {
    const Sentinel sentinel;
    Sentinel::clearGlobal();

    f( sentinel );

    assert( Sentinel::calls() == 0 );

    Sentinel::clearGlobal();

    f( Sentinel{} );

    assert( Sentinel::calls( Call::DefaultConstructor ) == 1 );
    assert( Sentinel::calls( Call::MoveConstructor ) == 2 );
    assert( Sentinel::calls( Call::Destructor ) == 3 );
    assert( Sentinel::calls() == 6 );
  }

  void g( LRValue< Sentinel > ) {}

  void callWithMutable()
  {
    Sentinel sentinel;
    Sentinel::clearGlobal();

    f( sentinel );

    assert( Sentinel::calls() == 0 );

    Sentinel::clearGlobal();

    f( Sentinel{} );

    assert( Sentinel::calls( Call::DefaultConstructor ) == 1 );
    assert( Sentinel::calls( Call::MoveConstructor ) == 2 );
    assert( Sentinel::calls( Call::Destructor ) == 3 );
    assert( Sentinel::calls() == 6 );
  }
}

int main()
{
  callWithConstant();  
  callWithMutable();
}

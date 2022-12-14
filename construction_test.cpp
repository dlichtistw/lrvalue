#include <cassert>

#include "lrvalue.h"
#include "test_util.h"

using namespace lrvalue;
using namespace lrvalue::test;

namespace
{
  void ConstructionByMutableReference()
  {
    Sentinel sentinel;
    Sentinel::clearGlobal();

    LRValue< Sentinel >{ sentinel };
    LRValue< const Sentinel >{ sentinel };

    assert( Sentinel::calls() == 0 );
  }

  void ConstructionByConstantReference()
  {
    const Sentinel sentinel;
    Sentinel::clearGlobal();

    LRValue< const Sentinel >{ sentinel };

    assert( Sentinel::calls() == 0 );
  }

  void ConstructionByTemporary()
  {
    Sentinel::clearGlobal();
    LRValue mut{ Sentinel{} };

    assert( Sentinel::calls( Call::DefaultConstructor ) == 1 );
    assert( Sentinel::calls( Call::MoveConstructor ) == 2 );
    assert( Sentinel::calls( Call::Destructor ) == 2 );
    assert( Sentinel::calls() == 5 );

    Sentinel::clearGlobal();

    LRValue< const Sentinel > con{ Sentinel{} };

    assert( Sentinel::calls( Call::DefaultConstructor ) == 1 );
    assert( Sentinel::calls( Call::MoveConstructor ) == 2 );
    assert( Sentinel::calls( Call::Destructor ) == 2 );

    assert( Sentinel::calls() == 5 );
  }

  void ConstructionByDeduction()
  {
    Sentinel sentinel;
    Sentinel::clearGlobal();

    LRValue fromMutable{ sentinel };
    LRValue fromConstant{ std::as_const( sentinel ) };
    LRValue fromTemporary{ Sentinel{} };

    assert( Sentinel::calls( Call::DefaultConstructor ) == 1 );
    assert( Sentinel::calls( Call::MoveConstructor ) == 2 );
    assert( Sentinel::calls( Call::Destructor ) == 2 );
    assert( Sentinel::calls() == 5 );
  }
}

int main()
{
  ConstructionByMutableReference();
  ConstructionByConstantReference();
  ConstructionByTemporary();
  ConstructionByDeduction();
}

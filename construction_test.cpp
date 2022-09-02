#include <cassert>

#include "lrvalue.h"
#include "test_util.h"

using namespace lrvalue;
using namespace lrvalue::test;

namespace
{
  void constructionByMutableReference()
  {
    Sentinel sentinel;

    Sentinel::clear();

    LRValue< Sentinel > mut{ sentinel };
    LRValue< const Sentinel > con{ sentinel };

    assert( Sentinel::calls() == 0 );
  }

  void ConstructionByConstantReference()
  {
    Sentinel sentinel;

    Sentinel::clear();

    LRValue con{ sentinel };

    assert( Sentinel::calls() == 0 );
  }

  void ConstructionByTemporary()
  {
    Sentinel::clear();

    LRValue mut{ Sentinel() };

    assert( Sentinel::calls( Call::DefaultConstructor ) == 1 );
    assert( Sentinel::calls( Call::MoveConstructor ) == 2 );
    assert( Sentinel::calls( Call::Destructor ) == 2 );

    LRValue< const Sentinel > con{ Sentinel() };

    assert( Sentinel::calls( Call::DefaultConstructor ) == 2 );
    assert( Sentinel::calls( Call::MoveConstructor ) == 4 );
    assert( Sentinel::calls( Call::Destructor ) == 4 );

    assert( Sentinel::calls() == 10 );
  }
}

int main()
{
  constructionByMutableReference();
  ConstructionByConstantReference();
  ConstructionByTemporary();
}

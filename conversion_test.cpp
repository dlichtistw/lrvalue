#include <cassert>

#include "lrvalue.h"
#include "test_util.h"

using namespace lrvalue;
using namespace lrvalue::test;

namespace
{
  void CastToStoredType()
  {
    Sentinel sentinel;
    LRValue fromMutable{ sentinel };
    Sentinel::clearGlobal();

    static_cast< Sentinel & >( fromMutable );
    static_cast< const Sentinel & >( fromMutable );

    assert( Sentinel::calls() == 0 );

    LRValue fromConstant{ std::as_const( sentinel ) };
    Sentinel::clearGlobal();

    static_cast< const Sentinel & >( fromConstant );

    assert( Sentinel::calls() == 0 );

    LRValue fromTemporary{ Sentinel() };
    Sentinel::clearGlobal();

    static_cast< Sentinel & >( fromTemporary );
    static_cast< const Sentinel & >( fromTemporary );

    assert( Sentinel::calls() == 0 );
  }

  void CastToConstant()
  {
    Sentinel sentinel;
    LRValue fromMutable{ sentinel };
    Sentinel::clearGlobal();

    static_cast< LRValue< const Sentinel > >( fromMutable );

    assert( Sentinel::calls() == 0 );

    LRValue fromConstant{ std::as_const( sentinel ) };
    Sentinel::clearGlobal();

    static_cast< LRValue< const Sentinel > >( fromConstant );

    assert( Sentinel::calls() == 0 );

    LRValue< Sentinel > fromTemporary{ Sentinel{} };
    Sentinel::clearGlobal();

    static_cast< LRValue< const Sentinel > >( fromTemporary );

    assert( Sentinel::calls() == 0 );
  }
}

int main()
{
  CastToStoredType();
  CastToConstant();
}

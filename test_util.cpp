#include "test_util.h"

#include <numeric>
#include <stdexcept>

using namespace lrvalue::test;

namespace
{
  int sum( const std::map< Call, int > &counter )
  {
    return std::accumulate( counter.begin(), counter.end(), 0,
                            []( int sum, const auto &entry ){ return sum + entry.second; } );
  }
}

std::ostream &lrvalue::test::operator<<( std::ostream &stream, const Call call )
{
  switch ( call )
  {
    case Call::DefaultConstructor: return stream << "DefaultConstructor";
    case Call::CopyConstructor: return stream << "CopyConstructor";
    case Call::MoveConstructor: return stream << "MoveConstructor";
    case Call::CopyAssignment: return stream << "CopyAssignment";
    case Call::MoveAssignment: return stream << "MoveAssignment";
    case Call::Destructor: return stream << "Destructor";
  }

  throw std::domain_error( "Unknown call id." );
}

void Sentinel::count( const Call call )
{
  ++_globalCounter[ call ];
  const int c = ++_instanceCounter[ call ];
  std::cout << "Call " << c << " to " << call << std::endl;
}

Sentinel::Sentinel() { count( Call::DefaultConstructor ); }
Sentinel::Sentinel( const Sentinel &sentinel ) : _instanceCounter( sentinel._instanceCounter ) { count( Call::CopyConstructor ); }
Sentinel::Sentinel( Sentinel &&sentinel ) : _instanceCounter( std::move( sentinel._instanceCounter ) ) { count( Call::MoveConstructor ); }

Sentinel &Sentinel::operator=( const Sentinel &sentinel )
{
  _instanceCounter = sentinel._instanceCounter;
  count( Call::CopyAssignment );
  return *this;
}
Sentinel &Sentinel::operator=( Sentinel &&sentinel )
{
  _instanceCounter = std::move( sentinel._instanceCounter );
  count( Call::MoveAssignment );
  return *this;
}

Sentinel::~Sentinel() { count( Call::Destructor ); }

int Sentinel::calls() { return sum( _globalCounter ); }
int Sentinel::calls( const Call call ) { return _globalCounter[ call ]; }

int Sentinel::operator()() const { return sum( _instanceCounter ); }
int Sentinel::operator()( const Call call ) const { return _instanceCounter[ call ]; }

void Sentinel::clearGlobal()
{
  _globalCounter.clear();
  std::cout << "Global call counter cleared." << std::endl;
}

void Sentinel::clearThis()
{
  _instanceCounter.clear();
  std::cout << "Instance call counter cleared." << std::endl;
}

Sentinel::Counter Sentinel::_globalCounter;


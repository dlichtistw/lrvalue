#include "test_util.h"

#include <numeric>
#include <stdexcept>

using namespace lrvalue::test;

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
  const int c = ++_counter[ call ];
  std::cout << "Call " << c << " to " << call << std::endl;
}

Sentinel::Sentinel(){ count( Call::DefaultConstructor ); }
Sentinel::Sentinel( const Sentinel & ){ count( Call::CopyConstructor ); }
Sentinel::Sentinel( Sentinel && ){ count( Call::MoveConstructor ); }

Sentinel &Sentinel::operator=( const Sentinel & ){ count( Call::CopyAssignment ); return *this; }
Sentinel &Sentinel::operator=( Sentinel && ){ count( Call::MoveAssignment ); return *this; }

Sentinel::~Sentinel(){ count( Call::Destructor ); }

int Sentinel::calls()
{
  return std::accumulate( _counter.begin(), _counter.end(), 0,
                          []( int sum, const Counter::value_type &entry ){ return sum + entry.second; } );
}
int Sentinel::calls( const Call call ){ return _counter[ call ]; }
void Sentinel::clear(){ _counter.clear(); }

Sentinel::Counter Sentinel::_counter{};


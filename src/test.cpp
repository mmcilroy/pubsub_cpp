#include "pubsub/publisher.hpp"

#include <cassert>
#include <cstring>
#include <iostream>

const size_t N = 4 * 1024 * 1024;
const size_t Q = 8;

using namespace pubsub;

struct test_data
{
    long t;
    long v;
};

void do_publish_st( publisher< test_data >* p, long t )
{
    for( size_t i=0; i<N; )
    {
        p->publish( 1, [&]( test_data& e, size_t n ) {
            e.t = t; e.v = i++; //std::cout << "p " << e.v << std::endl;
        } );
    }
}

void do_publish_mt( publisher< test_data >* p, long t )
{
    static std::mutex mut;

    for( size_t i=0; i<N; )
    {
        {
            std::unique_lock< std::mutex > lock( mut );
            p->publish( 1, [&]( test_data& e, size_t n ) {
                e.t = t; e.v = i++;
            } );
        }
    }
}

void do_subscribe( subscriber< test_data >* s, size_t P )
{
    test_data e[P]; memset( e, 0, sizeof( e ) );
    size_t c = 0;

    s->dispatch( [&]( const test_data& r, size_t a ) {
        assert( r.v == e[r.t].v++ );
        if( r.v >= N-1 ) c++;
        return c >= P;
    } );
}

void testcase( size_t P, size_t S, bool parallel )
{
    publisher< test_data > p0( Q );

    std::vector< subscriber< test_data >* > sv;
    for( int i=0; i<S; i++ )
    {
        if( parallel )
        {
            sv.push_back( &p0.subscribe() );
        }
        else
        {
            if( i == 0 ) {
                sv.push_back( &p0.subscribe() );
            } else {
                sv.push_back( &sv[i-1]->subscribe() );
            }
        }
    }

    std::vector< std::thread > tv;
    for( int i=0; i<S; i++ ) {
        tv.push_back( std::thread( do_subscribe, sv[i], P ) );
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::vector< std::thread > pv;

    for( int i=0; i<P; i++ ) {
        pv.push_back( std::thread( ( P == 1 ? do_publish_st : do_publish_mt ), &p0, i ) );
    }

    for( int i=0; i<S; i++ ) {
        tv[i].join();
    }

    for( int i=0; i<P; i++ ) {
        pv[i].join();
    }

    auto millis = std::chrono::duration_cast< std::chrono::milliseconds >(
        std::chrono::high_resolution_clock::now() - start ).count() + 1;

    std::cout << P << " to " << S << " " << ( parallel ? "parallel" : "pipeline" ) << ": " << ( N * P * 1000 ) / millis << std::endl;
}

int main()
{
    std::cout.imbue( std::locale( "" ) );

    // parallel
    testcase( 1, 1, true );
    testcase( 1, 2, true );
    testcase( 1, 4, true );

    testcase( 2, 1, true );
    testcase( 2, 2, true );
    testcase( 2, 4, true );

    testcase( 4, 1, true );
    testcase( 4, 2, true );
    testcase( 4, 4, true );
    testcase( 8, 8, true );

    // pipeline
    testcase( 1, 1, false );
    testcase( 1, 2, false );
    testcase( 1, 4, false );

    testcase( 2, 1, false );
    testcase( 2, 2, false );
    testcase( 2, 4, false );

    testcase( 4, 1, false );
    testcase( 4, 2, false );
    testcase( 4, 4, false );
    testcase( 8, 8, false );
}

#include    <netinet/in.h>
#include    <sys/socket.h>
#include    <stdio.h>
#include    <string.h>
#include    <netdb.h>
#include    <strings.h>

// A couple of functions for converting IP addresses to and from dotted decimal notation.

char *
get_istring( unsigned long x, char * s, unsigned int len )
{
    int     a,b,c,d;

    d = x & 0x000000ff;
    c = (x >> 8) & 0x000000ff;
    b = (x >> 16) & 0x000000ff;
    a = (x >> 24) & 0x000000ff;
    snprintf( s, len, "%d.%d.%d.%d", a,b,c,d );
    s[len-1] = '\0';
    return s;
}

long
get_iaddr_string( char * string )
{
    int     a,b,c,d;

    if ( string == 0 )
    {
        return 0;
    }
    else if ( sscanf( string, "%d.%d.%d.%d", &a, &b, &c, &d ) < 4 )
    {
        return 0;
    }
    else
    {
        return (a<<24) | (b<<16) | (c<<8) | d;
    }
}

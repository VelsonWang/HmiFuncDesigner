/*
Copyright (c) 2016 Wilson Birney <wpbirney@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifndef XSOCKET_HPP
#define XSOCKET_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <cstring> //memset
#include <algorithm>
#include <vector>

//NOT READY FOR PRODUCTION USE BY ANY MEANS, so keep this 0x00 until otherwise
#define XSOCK_VERSION	0x00

//Defaults all sockets to nonblocking
//#define XS_NONBLOCKING

//cross platform includes
#ifndef _WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
static WSAData _wsaData;
static bool _wsaInitDone = false;
#define SHUT_RD SD_RECEIVE
#define SHUT_WR	SD_SEND
#define SHUT_RDWR SD_BOTH
#endif


namespace net {

inline void init()	{
//no-op on *nix
#ifdef _WIN32
	if( !_wsaInitDone ) {
		WSAStartup(MAKEWORD(2,2), &_wsaData);
		_wsaInitDone = true;
	}
#endif
}

enum class af	{
	inet = AF_INET,
	inet6 = AF_INET6,
	unspec = AF_UNSPEC
};

enum class sock	{
	stream = SOCK_STREAM,
	dgram = SOCK_DGRAM
};

enum class shut	{
	rd = SHUT_RD,
	wr = SHUT_WR,
	rdwr = SHUT_RDWR
};

struct endpoint;

typedef std::vector<char> buffer;
typedef std::vector<endpoint> endpoint_buffer;

/*
 *	generic endpoint class for uniform access to ipv4 and ipv6
 */

struct endpoint {
	endpoint()	{
		memset( &addr, 0, sizeof( sockaddr_storage ) );
		addrlen = sizeof( sockaddr_storage );
	}

	endpoint( int port )				{
		set( "0", port );
	}
	endpoint( std::string ip, int port )		{
		set( ip, port );
	}
	endpoint( std::string ip, int port, af fam )	{
		set( ip, port, fam );
	}

	bool operator== ( const endpoint& e )	{
		if( memcmp( &addr, e.data(), e.size() ) == 0 )
			return true;
		return false;
	}

	bool operator!= ( const endpoint& e )	{
		return !operator==( e );
	}

	//returns a vector of all possible endpoints for host:port for the specified sock_type and address family
	static endpoint_buffer resolve( const char* host, const char* service, af f )	{

		//set up our addrinfo hints for the getaddrinfo call
		addrinfo hints;
		memset( &hints, 0, sizeof( addrinfo ) );
		hints.ai_family = (int)f;
		hints.ai_socktype = 0;

		//if host is nullptr, we need to set AI_PASSIVE, meaning its meant for binding
		if( host == nullptr )
			hints.ai_flags = AI_PASSIVE;

		addrinfo *res, *rp;

		int i = getaddrinfo( host, service, &hints, &res );
		if( i != 0 )	{
			std::cout << gai_strerror( i ) << std::endl;
			return endpoint_buffer();
		}

		endpoint_buffer buffer;

		for( rp = res; rp != nullptr; rp = rp->ai_next )	{
			endpoint ep;
			memcpy( &ep.addr, rp->ai_addr, rp->ai_addrlen );
			ep.initialize( rp->ai_addrlen, (af)rp->ai_family );

			if( std::find( buffer.begin(), buffer.end(), ep ) == buffer.end() )
				buffer.push_back( ep );
		}

		freeaddrinfo( res );

		return buffer;
	}

	void set( std::string ip, int port, af f=af::unspec )	{
		const char *host = ip.c_str();
		if( ip == "0" )
			host = nullptr;
		endpoint_buffer epList = endpoint::resolve( host, std::to_string(port).c_str(), f );
		*this = epList[0];
	}

	//must be called after you manually write into addr
	void initialize( socklen_t alen, af fam )	{
		addrlen = alen;
		addrfam = fam;
	}

	std::string get_host( int flags=0 ) const	{
		char hostbuf[INET6_ADDRSTRLEN];
		getnameinfo( data(), size(), &hostbuf[0], INET6_ADDRSTRLEN, nullptr, 0, flags );
		return &hostbuf[0];
	}

	std::string get_ip() const	{
		return get_host( NI_NUMERICHOST );
	}

	std::string get_service( int flags=0 ) const	{
		char servbuf[INET6_ADDRSTRLEN];
		getnameinfo( data(), size(), nullptr, 0, &servbuf[0], INET6_ADDRSTRLEN, flags );
		return &servbuf[0];
	}

	int get_port() const	{
		return std::atoi( get_service( NI_NUMERICSERV ).c_str() );
	}

	af get_af() const	{
		return addrfam;
	}

	sockaddr* data() const	{
		return (sockaddr*)&addr;
	}

	int size() const	{
		return addrlen;
	}

	std::string to_string() const	{
		std::stringstream ss;
		ss << get_ip() << ":" << get_port();
		return ss.str();
	}

	sockaddr_storage addr;
	socklen_t addrlen;
	af addrfam;
};



/*
 *	base socket class
 */

struct socket {
	socket()	{
		fd = -1;
	}

	socket( af fam, sock socktype )	{
		init( fam, socktype );
	}

	socket( af fam, sock socktype, int port ) : socket( fam, socktype )	{
		int r = bind( port );
		if( r == -1 )	{
			close();
			fd = -1;
		}
	}

	socket& operator =( int newfd )	{
		fd = newfd;
		return *this;
	}

	int init( af fam, sock socktype )	{
		fd = ::socket( (int)fam, (int)socktype, 0);
		addrfam = fam;
#ifdef XS_NONBLOCKING
		setnonblocking( true );
#endif
		return fd;
	}

	int accept( endpoint* ep )	{
		socklen_t al = ep->size();
		int i = ::accept( fd, ep->data(), &al );
		ep->initialize( al, addrfam );
		return i;
	}

	int listen( int n )	{
		return ::listen( fd, n );
	}

	int bind( const endpoint ep )	{
		return ::bind( fd, ep.data(), ep.size() );
	}

	int bind( const std::string addr, int port )	{
		return bind( endpoint( addr, port, addrfam ) );
	}

	int bind( int port )	{
		return bind( "0", port );
	}

	int connect( const endpoint ep )	{
		return ::connect( fd, ep.data(), ep.size() );
	}

	std::size_t sendto( const char* data, std::size_t len, endpoint ep )	{
		return ::sendto( fd, data, len, 0, ep.data(), ep.size() );
	}

	std::size_t sendto( const std::vector<char>& data, endpoint ep )	{
		return sendto( data.data(), data.size(), ep );
	}

	std::size_t sendto( const std::string* data, endpoint ep )	{
		return sendto( data->c_str(), data->size(), ep );
	}

	std::size_t recvfrom( char* buf, std::size_t len, endpoint* ep )	{
		socklen_t al = ep->size();
		int i = ::recvfrom( fd, buf, len, 0, ep->data(), &al );
		ep->initialize( al, addrfam );
		return i;
	}

	std::size_t recvfrom( std::vector<char>& buf, endpoint* ep )	{
		return recvfrom( buf.data(), buf.size(), ep );
	}

	std::size_t recvfrom( std::string* buf, std::size_t len, endpoint* ep )	{
		std::vector<char> buffer( len );
		int r = recvfrom( buffer, ep );
		if( r > 0 )
			*buf = std::string( buffer.data(), r );
		return r;
	}

	std::size_t send( const char* data, std::size_t len )	{
		return ::send( fd, data, len, 0 );
	}

	std::size_t send( const std::vector<char>& data )	{
		return send( data.data(), data.size() );
	}

	std::size_t send( const std::string* data )	{
		return send( (char*)data->c_str(), data->size() );
	}

	std::size_t recv( char* buf, std::size_t len )	{
		return ::recv( fd, buf, len, 0 );
	}

	std::size_t recv( std::vector<char>& buf )	{
		return recv( buf.data(), buf.size() );
	}

	std::size_t recv( std::string* buf, std::size_t len )	{
		std::vector<char> buffer( len );
		int r = recv( buffer );
		if( r > 0 )
			*buf = std::string( buffer.data(), r );
		return r;
	}

	int close()	{
#ifndef _WIN32
		return ::close( fd );
#else
		return ::closesocket( fd );
#endif
	}

	int shutdown( shut how )	{
		return ::shutdown( fd, (int)how );
	}

	int setnonblocking( bool block )	{
#ifndef _WIN32
		return fcntl( fd, F_SETFL, O_NONBLOCK, block );
#else
		DWORD nb = (int)block;
		return ioctlsocket( fd,  FIONBIO,  &nb );
#endif
	}

	// TODO: implement the timeout for windows
	int settimeout( int sec, int us )	{
#ifndef _WIN32
		timeval tv;
		tv.tv_sec = sec;
		tv.tv_usec = us;
		int i = setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
		return i;
#else
		return -1;
#endif
	}

	endpoint getlocaladdr()	const {
		return getname(fd, getsockname, addrfam );
	}

	endpoint getremoteaddr() const	{
		return getname(fd, getpeername, addrfam );
	}

	int geterror() const	{
		return errno;
	}

	bool good() const	{
		if( fd != -1 )
			return true;
		return false;
	}

  private:

	// getname calls getsockname/getpeername and returns it as an endpoint type
	template< typename t >
	inline endpoint getname(int fd, t target, af fam) const {
		endpoint ep;
		socklen_t al = ep.size();
		target(fd, ep.data(), &al);
		ep.initialize( al, fam );
		return ep;
	}

	int fd;
	af addrfam;
};

} //namespace net

#endif // XSOCKET_HPP

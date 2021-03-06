/**
 * @file   httpcmd.h
 * @author Devil Wang <wxjeacen@gmail.com>
 * @date   Thu May 12 17:15:44 2011
 *
 * @brief  Http Cmd class set
 *
 *
 */
#include "TThreadPool.hh"
#include <string>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Exception.hpp>
#include <sstream>
#include <list>
#include <vector>

class minIni;

struct MyCookie
{
    std::string name;
    std::string value;
    std::string domain;
    std::string path;
    time_t expires;
    bool tail;
    bool secure;

    friend std::ostream & operator<<(std::ostream &strm, const MyCookie &cook);

};

struct CookieHandle{

public:

    //std::list<MyCookie> cookielist;

    MyCookie MakeCookie(const std::string &str_cookie);

protected:

    std::vector<std::string> & split_cookie_str(const std::string &str, std::vector<std::string> &in);

    std::vector<std::string> splitCookieStr(const std::string &str);

    std::vector<std::string> &splitCookieStr(const std::string &str, std::vector<std::string> &in);

    int StrToInt(const std::string &str);

};

class WriterMemoryClass
{
public:

    WriterMemoryClass();

    ~WriterMemoryClass();

    void * Realloc(void * ptr, size_t );

    size_t WriteMemoryCallback(char *  ptr, size_t size, size_t nmemb);

    void print ();

    size_t write(std::string &);

    char *m_pBuffer;

    size_t m_Size;
};


class HttpCmd{

protected:
    minIni * ini;

    std::string version;

    std::string prefix;

    std::string postfix;

    std::string secretkey;

    std::string url;

    std::list<std::string> cookies;

    CookieHandle cookiehandler;

public:
    HttpCmd();
    ~HttpCmd();
    cURLpp::Cleanup cleanup;

    cURLpp::Easy httprequest;

    WriterMemoryClass mWriterChunk;

    void putCookies(const std::list<std::string>  & cookielist);

    std::list<std::string> getCookies();

    size_t readData(const std::string & s );

    size_t writeData(std::string &) ;

    std::string encrypt(const std::string &);

    std::string decrypt(const std::string &);

    std::string indata;

    std::string outdata;

};

class HttpConnect:public HttpCmd,public TThreadPool::TJob
{
    std::string host;

    int port;

public:

    HttpConnect(const std::string & host, const int port );

    void setHostPort( const std::string & host, const int & port);

    HttpConnect():HttpCmd(){}

    ~HttpConnect(){}

    void run(void *);

};

class HttpIdle:public HttpCmd,public TThreadPool::TJob
{

public :
    HttpIdle(std::list<std::string> cookies);
    void run(void *);
};

class HttpSend:public HttpCmd,public TThreadPool::TJob
{

public:

    HttpSend(std::list<std::string> cookieslist);

    void run(void *);
};

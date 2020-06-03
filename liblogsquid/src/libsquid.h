#ifndef LIBSQUIDTOOLS_H
#define LIBSQUIDTOOLS_H

#include <iostream>
#include <cstdlib>  // strtod ...
#include <cstdint>  
#include <string>
#include <sstream>  // stringstream
#include <vector>
#include <map>


namespace nsLogFormat {

class ReservedWords
{
public:
  ReservedWords();

  enum eLogFormats {
       FmtSquid,
       FmtCommon,
       FmtCombined,
       FmtReferrer,
       FmtUserAgent,
       FmtNone
  };

  enum  eRWords {
          unix_timestamp, // %ts.%03tu
          sec_since_epch, // %ts
          milliseconds,   // %03tu
          response_time,  // %tr
          client_src_ip_addr,  // %>a
          squid_req_st_and_http_st_send, // %Ss/%03>Hs
          squid_req_status, // %Ss
          squid_hier_status, // Sh
          squid_req_and_hier_status,  // %Ss:%sh
          squid_hier_st_and_server_ip,
          http_status_send, // %>Hs
          total_size_req, // %<st
          req_method, // %rm
          req_url_rcv, // %ru
          mime_type, // %mt
          server_ip, // %<a
          user_name_from_ident, // %ui
          user_name, // %un
          local_time, // %tl
          req_proto_ver, // %rv
          origin_rcv_req_header, // %>h
          referrer, // %{Referrer}
          user_agent, // %{User-Agent}
          not_found
  };

  inline eRWords getToken(std::string key);
  //inline int8_t getIndex(std::string part, eRWords rw);
  inline int8_t getIndex(std::string fmt, std::string part, eRWords rw);

private:
   std::map<std::string, eRWords> mKey;
   std::map<eRWords, int8_t> mFmtSquid;
   std::map<eRWords, int8_t> mFmtCommon;
   std::map<eRWords, int8_t> mFmtCombined;
   std::map<eRWords, int8_t> mFmtReferrer;
   std::map<eRWords, int8_t> mFmtUserAgent;

};

class LogSquid {

public:
     std::vector<std::string> parse(std::string slog);
     static std::string to_lowercase(std::string s);

     void setFormat(std::string s) { slogFormat = s; };
     void setPart(std::string s) { slogPart = s; };
     void setLogReg(std::string s) { slogReg = s; };

     std::string getPart();

private:
     std::string slogFormat;
     std::string slogPart;
     std::string slogReg;
};


}; // nsLogFormat

#endif // LIBSQUIDTOOLS_H

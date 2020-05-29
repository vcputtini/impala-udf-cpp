#ifndef LIBSQUIDTOOLS_H
#define LIBSQUIDTOOLS_H

#include <iostream>
#include <cstdlib>  // strtod ...
#include <string>
#include <sstream>  // stringstream
#include <vector>


namespace nsLogFormat {

class LogSquid {
public:
  enum eLogFormats {
           FmtSquid,
           FmtCommon,
           FmtCombined,
           FmtReferrer,
           FmtUserAgent,
           FmtNone
   };

   enum eKeySquid {
     unix_timestamp,
     response_time,
     client_src_ip_addr,
     squid_req_st_http_st_code,
     total_size_reply,
     request_method,
     request_url,
     user_name,
     squid_hier_status_server_ip,
     mime_content_type,
     none
   };

  // *************************************************************************
   static std::vector<std::string> parse(std::string slog);


   // *************************************************************************
   static std::string to_lowercase(std::string s);
};

}; // nsLogFormat

#endif // LIBSQUIDTOOLS_H

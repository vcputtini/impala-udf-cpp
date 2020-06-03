#include "libsquid.h"

namespace nsLogFormat {

/* ************************* ReservedWords ********************************** */
/*
 * NAME: ReservedWords Constructor
 * SUBJECT: Maps initialization
 * PARAMS:
 *  IN: none
 *  OUT: none
 * COMMENTS:
 */
ReservedWords::ReservedWords()
{
   mKey = {
      { "unix_timestamp", unix_timestamp }, // %ts.%03tu
      { "sec_since_epch", sec_since_epch }, // %ts
      { "milliseconds", milliseconds },     // %tu
      { "response_time", response_time },   // %tr
      { "client_src_ip_addr", client_src_ip_addr }, // %>a
      { "squid_req_st_and_http_st_send", squid_req_st_and_http_st_send }, // %Ss/%03>Hs
      { "squid_req_status", squid_req_status }, // %Ss
      { "squid_hier_status", squid_hier_status }, // %Sh
      { "squid_req_and_hier_status", squid_req_and_hier_status }, // %Ss/%sh
      { "squid_hier_st_and_server_ip", squid_hier_st_and_server_ip }, // %Sh/%<a
      { "http_status_send", http_status_send }, // %>Hs
      { "total_size_req", total_size_req }, // %<st
      { "req_method", req_method }, // %rm
      { "req_url_rcv", req_url_rcv }, // %ru
      { "mime_type", mime_type }, // %mt
      { "server_ip", server_ip }, // %<a
      { "user_name_from_ident", user_name_from_ident }, // %ui
      { "user_name", user_name }, // %un
      { "local_time", local_time }, // %tl
      { "req_proto_ver", req_proto_ver }, // %rv
      { "origin_rcv_req_header",  origin_rcv_req_header }, // %>h
      { "referrer", referrer }, // %{Referrer}
      { "user_agent", user_agent }, // %{User-Agent}
      { "", not_found }
    }; // mKey

    mFmtSquid = {
      { unix_timestamp, 0 },
      { response_time, 1 },
      { client_src_ip_addr, 2 },
      { squid_req_st_and_http_st_send, 3 },
      { total_size_req, 4 },
      { req_method, 5 },
      { req_url_rcv, 6 },
      { user_name, 7 },
      { squid_hier_st_and_server_ip, 8 },
      { mime_type, 9 },
      { not_found, -1 }
    };

    mFmtCommon = {
      { client_src_ip_addr, 0 },
      { user_name_from_ident, 1 },
      { user_name, 2 },
      { local_time, 3 },
      { req_method, 4 },
      { req_url_rcv, 5 },
      { req_proto_ver, 6 },
      { http_status_send, 7 },
      { total_size_req, 8 },
      { squid_req_and_hier_status, 9 },
      { not_found, -1 }
    };

    mFmtCombined = {
      { client_src_ip_addr, 0 },
      { user_name_from_ident, 1 },
      { user_name, 2 },
      { local_time, 3 },
      { req_method, 4 },
      { req_url_rcv, 5 },
      { req_proto_ver, 6 },
      { http_status_send, 7 },
      { total_size_req, 8 },
      { referrer, 9 },
      { user_agent, 10 },
      { squid_req_and_hier_status, 11 },
      { not_found, -1 }
    };

    mFmtReferrer = {
      { unix_timestamp, 0 },
      { client_src_ip_addr, 1 },
      { referrer, 2 },
      { req_url_rcv, 3 },
      { not_found, -1 }
    };

    mFmtUserAgent = {
      { client_src_ip_addr, 0 },
      { local_time, 1 },
      { user_agent, 2 }
    };
}

/*
 * NAME: getToken
 * SUBJECT: Retruns the token tha refers
 * PARAMS:
 *  IN: keyword
 *  OUT: eRWords token
 * COMMENTS:
 * e.g.: getToken("local_time")
 */
ReservedWords::eRWords ReservedWords::getToken(std::string key)
{
  std::map<std::string, eRWords>::iterator itmap;
  itmap = mKey.find(key);
  if( itmap != mKey.end() ) {
      return itmap->second;
  }
  return not_found;
}

/*
 * NAME: getIndex
 * SUBJECT: Returns the value of the part's position index within the log line.
 * PARAMS:
 *  IN:
 *   arg 1: string - What's the log format: squid | common | combined |
 *                   referrer | useragent
 *   arg 2: string - Part's name.
 *   arg 3: eRWords - constant enum
 *  OUT: int8_t index
 * COMMENTS:
 */
int8_t ReservedWords::getIndex(std::string fmt, std::string part, eRWords rw)
{
  std::map<eRWords, int8_t>::iterator itmap;
  if( fmt == "squid" ) {
      itmap = mFmtSquid.find(rw);
      if( itmap != mFmtSquid.end() ) return itmap->second;
  } else if( fmt == "common" ) {
      itmap = mFmtCommon.find(rw);
      if( itmap != mFmtCommon.end() ) return itmap->second;
  } else if( fmt == "combined" ) {
      itmap = mFmtCombined.find(rw);
      if( itmap != mFmtCombined.end() ) return itmap->second;
  } else if( fmt == "referrer" ) {
      itmap = mFmtReferrer.find(rw);
      if( itmap != mFmtReferrer.end() ) return itmap->second;
  } else if( fmt == "useragent" ) {
      itmap = mFmtUserAgent.find(rw);
      if( itmap != mFmtUserAgent.end() ) return itmap->second;
  }

  return not_found;
}

/* ************************* LogSquid *************************************** */

/*
 * very simple implementation of the conversion
 * from uppercase to lowercase
 */
std::string LogSquid::to_lowercase(std::string s) {
  std::string x;
  for(size_t i=0; i<s.length(); i++ ) {
     x += std::tolower(s[i]);
  }
  return x;
}

/*
 * NAME: getPart
 * SUBJECT: Returns the information regarding the chosen part of the log.
 * PARAMS:
 *  IN: none
 *  OUT: string
 * COMMENTS:
 */
std::string LogSquid::getPart()
{
   ReservedWords rw;
   std::vector<std::string> v;
   v = parse(slogReg);
   return v[ rw.getIndex(slogFormat, slogPart, rw.getToken(slogPart)) ];
}

/*
 * NAME: parse
 * SUBJECT: Analyze and tokenize the log line generated by Squid.
 * PARAMS:
 *  IN: Field containing the log line generated by the Squid-cache proxy
 *      Note: The delimiter must be a white space
 *  OUT: Returns a vector with the parts of the log lines.
 * COMMENTS:
 * Uses a general-purpose algorithm very common and simple for this.
 * As for performance, we believe it will be sufficient for most cases.
 * We consider the compiler to be an excellent programmer,
 * so that it will be able to generate a very optimized code. :-)
 */
std::vector<std::string> LogSquid::parse(std::string slog)
{
  std::string sbuf;
  std::vector<std::string> vbuf;

  for(size_t i=0; i<= slog.length(); i++) {
    if( slog[i] == ' ' || slog[i] == 0) {
        sbuf.erase(0, sbuf.find_first_not_of(' '));
        vbuf.push_back(sbuf);
        sbuf.clear();
    } else if(slog[i] == '[' ){
        i++;
        sbuf.clear();
        while( slog[i] != ']' ) {
           sbuf += slog[i];
           i++;
        }
    } else if (slog[i] == '\"' ) {
        i++;
        sbuf.clear();
        while( slog[i] != '\"' ) {
           if( slogFormat == "common" || slogFormat == "combined" ) {
               if( slog[i] == ' ' ) {
                       sbuf.erase(0, sbuf.find_first_not_of(' '));
                       vbuf.push_back(sbuf);
                       sbuf.clear();
                }
           }
           sbuf += slog[i];
           i++;
        }
    } else{
        sbuf += slog[i];
    }
  }
  // Remove empty elements from the vector
  // Adapted from: https://www.techiedelight.com/remove-elements-vector-inside-loop-cpp/
  for(auto it = vbuf.begin(); it != vbuf.end(); it++) {
      if(*it == "") {
         vbuf.erase(it--);
      }
  }

  return vbuf;
}

}; // nsLogFormat

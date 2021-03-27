/***************************************************************************
 * Copyright (c) 2020-21                                                    *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 * 1. Redistributions of source code must retain the above copyright       *
 *    notice, this list of conditions and the following disclaimer.        *
 * 2. Redistributions in binary form must reproduce the above copyright    *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *
 * 4. Neither the name of the Author     nor the names of its contributors *
 *    may be used to endorse or promote products derived from this software*
 *    without specific prior written permission.                           *
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
 * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
 * INTERRUPTION)                                                           *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OFSUCH DAMAGE.                                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef LIBSQUIDTOOLS_H
#define LIBSQUIDTOOLS_H

#include <iostream>
#include <cstdlib>  // strtod ...
#include <cstdint>
#include <string>
#include <sstream>  // stringstream, istringstream
#include <vector>
#include <map>


namespace nsLogFormat {

class ReservedWords
{
public:
  ReservedWords();

  /* note: Future use */
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

  inline eLogFormats getLogFormat(std::string part);
  inline int8_t getIndex(std::string fmt, std::string part, eRWords rw);
  inline eRWords getToken(std::string key);

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
     void setFormat(std::string s) { slogFormat = to_lowercase(s); };
     void setPart(std::string s) { slogPart = to_lowercase(s); };
     void setLogReg(std::string s) { slogReg = s; };
     std::string getPart();
     int64_t getTotalSizeReq();  // BigIntVal == int64_t

     static std::string to_lowercase(std::string s);

private:
     std::string slogFormat;
     std::string slogPart;
     std::string slogReg;
};


}; // nsLogFormat

#endif // LIBSQUIDTOOLS_H

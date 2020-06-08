/***************************************************************************
 * Copyright (c) 2020                                                      *
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
#include "common.h"
#include "udfsquid.h"
#include "libsquid.h"

using namespace impala_udf;

using namespace nsLogFormat;

IMPALA_UDF_EXPORT
StringVal udf_logparts(FunctionContext* ctx, const StringVal& logformat,
                    const StringVal& part,
                    const StringVal& log)
{
  if(logformat.is_null || logformat.len == 0) {
    std::stringstream ss;
    ss << "Argument 1 (LOGFORMAT): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }

  std::string slogformat((const char *)logformat.ptr, logformat.len);
  std::string f="squidcommoncombinedreferreruseragent";
  std::size_t found = f.find(slogformat);
  if( found == std::string::npos) {
    std::stringstream ss;
    ss << "Argument 1 (LOGFORMAT): Invalid!. Valid are: squid|common|combined|referrer|useragent";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }

  if(part.is_null || part.len == 0) {
    std::stringstream ss;
    ss << "Argument 2 (PART): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }
  if(log.is_null || log.len == 0) {
    std::stringstream ss;
    ss << "Argument 3 (LOG): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }

  std::string spart((const char *)part.ptr, part.len);
  std::string slog((const char *)log.ptr, log.len);

  LogSquid ls;
  ls.setFormat(slogformat);
  ls.setPart(spart);
  ls.setLogReg(slog);
  std::string str;
  str = ls.getPart();

  StringVal result(ctx, str.size());
  memcpy(result.ptr, str.c_str(), str.size() );

  return result;
}

IMPALA_UDF_EXPORT
BigIntVal udf_totalsizereq(FunctionContext* ctx, const StringVal& logformat,
                    const StringVal& log)
{
  if(logformat.is_null || logformat.len == 0) {
    std::stringstream ss;
    ss << "Argument 1 (LOGFORMAT): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return BigIntVal::null();
  }

  if(log.is_null || log.len == 0) {
    std::stringstream ss;
    ss << "Argument 2 (LOG): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return BigIntVal::null();
  }

  std::string slogformat((const char *)logformat.ptr, logformat.len);
  std::string slog((const char *)log.ptr, log.len);

  std::string f="squidcommoncombined";
  std::size_t found = f.find(slogformat);
  if( found == std::string::npos) {
    std::stringstream ss;
    ss << "Argument 1 (LOGFORMAT): Invalid!. Valid are: squid|common|combined";
    ctx->SetError(ss.str().c_str());
    return BigIntVal::null();
  }

  LogSquid ls;
  ls.setFormat(slogformat);
  ls.setLogReg(slog);
  int64_t tsq = ls.getTotalSizeReq();

  return tsq; //BigIntVal(tsq);
}

/* ************************************************************************** */


#include "common.h"
#include "udfsquid.h"
#include "libsquid.h"

using namespace impala_udf;

using namespace nsLogFormat;

IMPALA_UDF_EXPORT
StringVal log_parts(FunctionContext* ctx, const StringVal& logformat,
                    const StringVal& part,
                    const StringVal& log)
{
  if(part.is_null) {
    std::stringstream ss;
    ss << "Argument 1 (LOGFORMAT): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }

  if(part.is_null) {
    std::stringstream ss;
    ss << "Argument 2 (PART): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }
  if(log.is_null) {
    std::stringstream ss;
    ss << "Argument 3 (LOG): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }

  std::string slogformat((const char *)logformat.ptr, logformat.len);
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

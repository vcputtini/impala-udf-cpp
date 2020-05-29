
#include "common.h"
#include "udfsquid.h"
#include "libsquid.h"

using namespace impala_udf;

IMPALA_UDF_EXPORT
StringVal log_parts(FunctionContext* ctx, StringVal& part,
                          StringVal& log)
{

  if(part.is_null) {
    std::stringstream ss;
    ss << "Argument 1 (PART): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }
  if(log.is_null) {
    std::stringstream ss;
    ss << "Argument 2 (LOG): Cannot be empty. ";
    ctx->SetError(ss.str().c_str());
    return StringVal::null();
  }

  std::string spart((const char *)part.ptr, part.len);
  std::string slog((const char *)log.ptr, log.len);

  std::vector<std::string> v;
  v = nsLogFormat::LogSquid::parse(slog);

  std::string sp(nsLogFormat::LogSquid::to_lowercase(spart));
  if(sp == "squid") {
    ;
  } else if(sp == "common") {
    ;
  } else if(sp == "combined") {
    ;
  } else if(sp == "referrer") {
    ;
  } else if(sp == "useragent") {
    ;
  }

  return StringVal::null();

}

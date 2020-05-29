#ifndef LIBLOGSQUID_UDF_H
#define LIBLOGSQUID_UDF_H

#include <impala_udf/udf.h>

using namespace impala_udf;

StringVal log_parts(FunctionContext* ctx, StringVal& part,
                          StringVal& log);


#endif // LIBLOGSQUID_UDF_H

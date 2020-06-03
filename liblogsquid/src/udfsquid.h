#ifndef LIBLOGSQUID_UDF_H
#define LIBLOGSQUID_UDF_H

#include <impala_udf/udf.h>

using namespace impala_udf;

StringVal udf_logparts(FunctionContext* ctx, const StringVal& logformat,
                    const StringVal& part,
                    const StringVal& log);


#endif // LIBLOGSQUID_UDF_H

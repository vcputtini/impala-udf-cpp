// Copyright 2019 Cloudera Inc.
//
// Licensed under the Apache License, Version 2.0 (the 'License');
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an AS IS BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef IMPALA_UDF_COMMON_H
#define IMPALA_UDF_COMMON_H

// Macro to prepend to function definitions that will export the
// symbols to be visible for loading by Impala. This is present in
// udf.h in some versions of Impala, but we can't assume that it
// is present yet.
#ifndef IMPALA_UDF_EXPORT
#define IMPALA_UDF_EXPORT __attribute__ ((visibility ("default")))
#endif

#endif



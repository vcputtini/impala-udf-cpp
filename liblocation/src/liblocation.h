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
#ifndef LIBLOCATION_UDF_H
#define LIBLOCATION_UDF_H

#include <impala_udf/udf.h>

using namespace impala_udf;

DoubleVal udf_g_distance(FunctionContext* ctx,
                    const DoubleVal& lat_a, const DoubleVal& lon_b,
                    const DoubleVal& lat_b, const DoubleVal& lon_a,
                    const StringVal& mt);

DoubleVal udf_g_azimuth(FunctionContext* ctx,
                    const DoubleVal& lat_a, const DoubleVal& lon_a,
                    const DoubleVal& lat_b, const DoubleVal& lon_b);

StringVal udf_g_latlon_to_utm(FunctionContext* ctx,
                    const DoubleVal& lat, const DoubleVal& lon,
                    const IntVal& zone );
StringVal udf_g_utm_to_latlon(FunctionContext* ctx,
                    const DoubleVal& x, const DoubleVal& y,
                    const BooleanVal& south_hemi, const IntVal& zone );

DoubleVal udf_g_getutmvalues(FunctionContext* ctx, const StringVal& field,
                    const StringVal& s_json);

BooleanVal udf_g_isvalidlat(FunctionContext* ctx, const DoubleVal& lat);
BooleanVal udf_g_isvalidlon(FunctionContext* ctx, const DoubleVal& lon);


DoubleVal udf_unit_convert(FunctionContext* ctx,
                    const StringVal& cnv, const DoubleVal& dval);


#endif // LIBLOCATION_UDF_H

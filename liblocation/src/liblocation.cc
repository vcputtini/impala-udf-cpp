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
#include "liblocation.h"
#include "locationtools.h"

#include <string>
#include <sstream>

#include <boost/algorithm/string.hpp>

using namespace impala_udf;

IMPALA_UDF_EXPORT
DoubleVal udf_g_azimuth(FunctionContext* ctx,
                    const DoubleVal& lat_a, const DoubleVal& lon_a,
                    const DoubleVal& lat_b, const DoubleVal& lon_b)
{
  if( !LocationTools::isValidLat(lat_a.val) ) {
    std::stringstream ss;
    ss << "Latitude of the coordinate of point A is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }
  if( !LocationTools::isValidLon(lon_a.val) ) {
    std::stringstream ss;
    ss << "Longitude of the coordinate of point A is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }
  if( !LocationTools::isValidLat(lat_b.val) ) {
    std::stringstream ss;
    ss << "Latitude of the coordinate of point B is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }
  if( !LocationTools::isValidLon(lon_b.val) ) {
    std::stringstream ss;
    ss << "Longitude of the coordinate of point B is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }

   return DoubleVal(LocationTools::azimuth(lat_a.val, lon_a.val,
                                 lat_b.val, lon_b.val) );
}


IMPALA_UDF_EXPORT
DoubleVal udf_g_getutmvalues(FunctionContext* ctx, const StringVal& field,
                        const StringVal& s_json)
{
  if(field.is_null) {
    std::stringstream ss;
    ss << "Argument 1: Cannot be null. ";
    ctx->SetError(ss.str().c_str());
    return DoubleVal::null();
  }

  if(s_json.is_null) {
    std::stringstream ss;
    ss << "Argument in json format cannot be null.";
    ctx->SetError(ss.str().c_str());
    return DoubleVal::null();
  }

  double d_val;
  std::string s((const char *)s_json.ptr, s_json.len);
  std::string fld((const char *)field.ptr, field.len);
  std::string s_err = LocationTools::get_utmvalues(fld, s, d_val);
  if(!s_err.empty()) {
    std::stringstream ss;
    ss << s_err << " Value Invalid: " << d_val;
    ctx->SetError(ss.str().c_str());
    return DoubleVal::null();
  }
  return DoubleVal(d_val);
}

IMPALA_UDF_EXPORT
DoubleVal udf_g_distance(FunctionContext* ctx,
                    const DoubleVal& lat_a, const DoubleVal& lon_a,
                    const DoubleVal& lat_b, const DoubleVal& lon_b,
                    const StringVal& mt)
{

  if( !LocationTools::isValidLat(lat_a.val) ) {
    std::stringstream ss;
    ss << "Latitude of the coordinate of point A is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }
  if( !LocationTools::isValidLon(lon_a.val) ) {
    std::stringstream ss;
    ss << "Longitude of the coordinate of point A is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }
  if( !LocationTools::isValidLat(lat_b.val) ) {
    std::stringstream ss;
    ss << "Latitude of the coordinate of point B is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }
  if( !LocationTools::isValidLon(lon_b.val) ) {
    std::stringstream ss;
    ss << "Longitude of the coordinate of point B is invalid!";
    ctx->SetError(ss.str().c_str());
     return DoubleVal::null();
  }

  std::string metrical((const char*)mt.ptr, mt.len);
  boost::algorithm::to_lower(metrical);

  double d = LocationTools::distance(lat_a.val, lon_a.val,
                                     lat_b.val, lon_b.val,
                  (metrical == "km" ? LocationTools::Km : LocationTools::Mi) );

  return DoubleVal(d);
}

IMPALA_UDF_EXPORT
StringVal udf_g_latlon_to_utm(FunctionContext* ctx,
                    const DoubleVal& lat, const DoubleVal& lon,
                    const IntVal& zone )
{
  int zn = zone.val;
  zn = std::floor((lon.val + 180.0) / 6) + 1; // UTM zone

  std::string s = LocationTools::latlon2utm(lat.val, lon.val, zn);

  StringVal result(ctx, s.size());
  memcpy(result.ptr, s.c_str(), s.size() );
  return result;
}

IMPALA_UDF_EXPORT
StringVal udf_g_utm_to_latlon(FunctionContext* ctx,
                    const DoubleVal& x, const DoubleVal& y,
                    const BooleanVal& south_hemi, const IntVal& zone )
{
  std::string s = LocationTools::utm2latlon(x.val, y.val, south_hemi.val,
                                           zone.val);

  StringVal result(ctx, s.size());
  memcpy(result.ptr, s.c_str(), s.size() );
  return result;
}

IMPALA_UDF_EXPORT
BooleanVal udf_g_isvalidlat(FunctionContext* ctx, const DoubleVal& lat)
{
  return BooleanVal( LocationTools::isValidLat(lat.val) );
}

IMPALA_UDF_EXPORT
BooleanVal udf_g_isvalidlon(FunctionContext* ctx, const DoubleVal& lon)
{
  return BooleanVal( LocationTools::isValidLon(lon.val) );
}

IMPALA_UDF_EXPORT
DoubleVal udf_unit_convert(FunctionContext* ctx,
                    const StringVal& cnv_str, const DoubleVal& dval)
{
  std::stringstream ss;
  ss << "What kind of conversion you want.";
  ss << "kmh_to_mih|mih_to_kmh|mi_to_meters|meters_to_mi|";
  ss << "feet_to_meters|meters_to_feet";

  if( cnv_str.is_null ) {
    ctx->SetError(ss.str().c_str());
    return DoubleVal::null();
  }

  // not worth testing for values <= 0
  if( dval.val <=0 ) return DoubleVal::null();

  std::string cnv((const char*)cnv_str.ptr, cnv_str.len);
  boost::algorithm::to_lower(cnv);
  if( cnv == "kmh_to_mih") {
    return UnitConverter::Kmh2Mph(dval.val);
  } else if( cnv == "mih_to_kmh") {
    return UnitConverter::Mph2Kmh(dval.val);
  } else if( cnv == "mi_to_meters") {
    return UnitConverter::Mi2Meters(dval.val);
  } else if( cnv == "meters_to_mi") {
    return UnitConverter::Meters2Mi(dval.val);
  } else if( cnv == "feet_to_meters") {
    return UnitConverter::Feet2Meters(dval.val);
  } else if( cnv == "meters_to_feet") {
    return UnitConverter::Meters2Feet(dval.val);
  } else {
    ctx->SetError(ss.str().c_str());
    return DoubleVal::null();
  }
}

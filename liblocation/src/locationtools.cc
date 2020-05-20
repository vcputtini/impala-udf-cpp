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

#include "locationtools.h"

/*
 * NAME: azimuth
 * SUBJECT: Calculate the azimuth from latitude and longitude.
 * PARAMS:
 *  IN: Starting point: lat/lon and End point: lat/lon.
 *  OUT: azimuth in decimal degrees
 * COMMENTS: Format: DD (degree.decimal)
 * Adapted from orignal: <https://www.omnicalculator.com/other/azimuth>
 */
double LocationTools::azimuth(const double lat_a, const double lon_a,
                                const double lat_b, const double lon_b)
{
  std::setprecision(10);

  double phi1 = lat_a * 0.01745329252; // convert to radians
  double lambda1 = lon_a * 0.01745329252;
  double phi2 = lat_b * 0.01745329252;
  double lambda2 = lon_b * 0.01745329252;

  double d_phi = phi2 - phi1;
  double d_lambda = lambda2 - lambda1;

  double part1 = std::sin(d_lambda) * std::cos(phi2);
  double part2 = (std::cos(phi1) * std::sin(phi2)) - (std::sin(phi1) * std::cos(phi2) * std::cos(d_lambda));
  double theta = std::atan2( part1 , part2 );

  double theta_deg = theta / M_PI * 180; // to degree
  if( theta_deg < 0 ) theta_deg += 360;

  return theta_deg;
}

/*
 * NAME: get_utmvalues
 * SUBJECT: Returns 'x' Easting or 'y' Northing value of UTM coordinate AND
 *          zone.
 * PARAMS:
 *  IN: String json-formated
 *  OUT: xE | yN | zone
 * COMMENTS:
 */
 std::string LocationTools::get_utmvalues(const std::string xy, const std::string s, double& val)
 {
   if(s.empty()) {
     val = 0.0;
     return "Parameter 2: JSON is empty.";
   }

   std::string opt;
   if( xy == "x" ) opt = "LatLonToUTMXY.xE";
   else if( xy == "y" ) opt = "LatLonToUTMXY.yN";
   else if( xy == "z" ) opt = "LatLonToUTMXY.zone";
   else
      return "Parameter 1: Must be one of these: 'x' or 'y' or 'z'.";

   ptree root;
   std::string s_err;

   try {
     std::stringstream ss;
     ss << s;  // we have to convert to stringstream
     read_json(ss, root);
   } catch(const ptree_error &e) {
      s_err = e.what();
   }

   std::string s_xyz;
   try {
     s_xyz = root.get<std::string>(opt);
   } catch(const ptree_error &e) {
     s_err = e.what();
   }
   if(s_err.empty()) {
     char* ptEnd;
     val = std::strtod(const_cast<char*>(s_xyz.c_str()), &ptEnd);
   } else {
     val = 0.0;
   }

   return s_err;
 }

/*
 * NOME: latlon2utm
 * SUBJECT: Works as a middleware between the UTM::LatLonToUTMXY function
 * and the function in the Impala API. As it is not possible to return a
 * complex variable to Impala, it returns the information from the calculation
 * in JSON format so that it can later be used in Impala, via the functions
 * g_getutm_xe () and g_getutm_yn ()
 * PARAMS:
 *  IN: Latitude, Longitude, e zone
 *  OUT: A JSON-formated string as shown:
 * {   "LatLonToUTMXY":{
 *     "xE":"508115.914296",
 *      "yN":"9434849.847182",
 *      "zone":"33"
 * }
 * COMMENTS:
 *  REMEMBER THAT: Latitude == UTM coordinate y in meters and
 *            Longitude == UTM coordinate x in meters
 */
std::string LocationTools::latlon2utm(const double lat, const double lon, const int zone)
{
  double xE, yN;
  UTM utm;
  int z = utm.LatLonToUTMXY(utm.DegToRad(lat), utm.DegToRad(lon), zone, xE, yN);

  // double to string convertions
  std::stringstream ss_xe;
  ss_xe << std::fixed << xE;
  std::stringstream ss_yn;
  ss_yn << std::fixed << yN;
  std::stringstream ss_z;
  ss_z << z;

  // Format to JSON
  std::stringstream ss;
  ss << "{\"LatLonToUTMXY\":"
        << "{\"xE\"" << ":\"" << ss_xe.str() << "\","
        << "\"yN\"" << ":\"" << ss_yn.str() << "\","
        << "\"zone\"" << ":\"" << ss_z.str() << "\""
        << "}}";

  return ss.str();
}

/*
 * NOME: utm2latlon
 * SUBJECT: Works as a middleware between the UTM::UTMXYToLatLon function
 * and the function in the Impala API. As it is not possible to return a
 * complex variable to Impala, it returns the information from the calculation
 * in JSON format so that it can later be used in Impala, via the functions
 * g_getutm_xe () and g_getutm_yn ()
 * PARAMS:
 *  IN: Latitude, Longitude, e zone
 *  OUT: A JSON-formated string as shown:
 * {   "UTMXYToLatLon":{
 *     "lat":"any value in decimal",
 *      "lat":"any value in decimal",
 *      "zone":"33"
 * }
 * COMMENTS:
 *  REMEMBER THAT: Latitude == UTM coordinate y in meters and
 *            Longitude == UTM coordinate x in meters
 */
std::string LocationTools::utm2latlon(const double x, const double y,
                              const int zone, const bool south_hemi)
{
  double d_lat, d_lon;
  UTM utm;
  utm.UTMXYToLatLon(x, y, zone, south_hemi, d_lat, d_lon );

  // double to string convertions
  std::stringstream ss_lat;
  ss_lat << std::setprecision(6) << std::fixed << utm.RadToDeg(d_lat);
  std::stringstream ss_lon;
  ss_lon << std::setprecision(6) << std::fixed << utm.RadToDeg(d_lon);
  std::stringstream ss_shemi;
  ss_shemi << south_hemi;

  // Format to JSON
  std::stringstream ss;
  ss << "{\"UTMXYToLatLon\":"
        << "{\"lat\"" << ":\"" << ss_lat.str() << "\","
        << "\"lon\"" << ":\"" << ss_lon.str() << "\","
        << "\"south_hemi\"" << ":\"" << ss_shemi.str() << "\""
        << "}}";

  return ss.str();
}

/*
 * NAME: isValidLat
 * SUBJECT: Check if latitude is in valid range: [-90,90]
 * PARAMS:
 *  IN: Latitude in sexagesimal
 *  OUT: true | false
 * COMMENTS: Use, Lat/Lon [+/-] in decimal degrees
 */
const bool LocationTools::isValidLat(const double coord)
{
  return ((int(coord) < -90) || (int(coord) > 90) ? false : true);
}

/*
 * NAME: isValidLon
 * SUBJECT: Check if longitude is in valid range: [-180,180]
 * PARAMS:
 *  IN: Longitude in sexagesimal
 *  OUT: true | false
 * COMMENTS: Use, Lat/Lon [+/-] in decimal degrees
 */
const bool LocationTools::isValidLon(const double coord)
{
  return ((int(coord) < -180) || (int(coord) > 180) ? false : true);
}

/*
 * NAME: todec
 * SUBJECT: sexagesimal coordinate in decimal
 * PARAMS:
 *  IN: Coordenate in sexagesimal
 *  OUT: Coordenate in decimal
 * COMMENTS: Use, GPS ISO 6709: LAT [-]ddmmss.ssss and
 *           LON [-]dddmmss.ssss in sexagesimal
 */
const double LocationTools::todec(const double coord)
{
  double intdeg;
  double fracmin = std::abs(std::modf(coord/10000, &intdeg)); // minute
  double intmin;
  double fracsec = std::abs(std::modf((fracmin)*100, &intmin));
  double min = intmin+(fracsec*100)/60;
  double d = std::abs(intdeg) + (min/60);

  return (intdeg < 0 ? d*(-1) : d);
}


/*
 * NAME: distance(lat_a, lat_b, lon_a, lon_b, MetricalType)
 * SUBJECT: Calculates the straight-line distance between two geographic
 * points,taking into account the curvature of the earth.
 * PARAMS:
 *  IN: lat_a & lon_b = Latitude and Longitude of A Point in decimal
 *      lat_a & lon_b = latitude and Longitude of B Point in decimal
 *  OUT: distance in Km or Mi

 * COMMENTS:
 */
double LocationTools::distance(const double lat_a, const double lon_a,
                 const double lat_b, const double lon_b,
                 MetricalType mt)
{
   double m;
   switch(mt) {
	 case MetricalType::Km:
             m = EARTH_RADIUS_IN_KM;
             break;
        case MetricalType::Mi:
             m = EARTH_RADIUS_IN_MILES;
             break;
        default:
             return 0.0;
   }

   double dist = m*std::acos(
        std::cos(M_PI*(90-lat_b)/180)*std::cos((90-lat_a)*M_PI/180)+
        std::sin((90-lat_b)*M_PI/180)*std::sin((90-lat_a)*M_PI/180)*
        std::cos((lon_a-lon_b)*M_PI/180)
	);

   return dist;
}

/*
 * KMH to MPH convertion
 */
const double UnitConverter::Kmh2Mph(const double km)
{
  return( km*0.6214 );
}

/*
 * MPH to KMH convertion
 */
const double UnitConverter::Mph2Kmh(const double mi)
{
  return( mi*1.60934 );
}

/*
 * land MI to Meters convertion
 */
const double UnitConverter::Mi2Meters(const double mi)
{
  return( mi*1609.34 );
}

/*
 * Meters to land miles convertion
 */
const double UnitConverter::Meters2Mi(const double m)
{
  /* 1meter == 6.2E-04 miles */
	return( m * 0.00062137119 );
}

/*
 * Meters to Nautical Miles convertion
 */
const double UnitConverter::Meters2Nm(const double m)
{
	return( m * 0.000547045);
}

/*
 * Feet to meters convertion
 */
const double UnitConverter::Feet2Meters(const double ft)
{
  return( ft * 0.3048 );
}

/*
 * Meters to feet convertion
 */
const double UnitConverter::Meters2Feet(const double mt)
{
	return( mt * 3.2808);
}

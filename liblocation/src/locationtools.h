/***************************************************************************
 * Copyright (c) 2020-21                                                      *
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

#ifndef LOCATIONTOOLS_H
#define LOCATIONTOOLS_H

#include <iostream>
#include <cstdlib>  // strtod ...
#include <string>
#include <sstream>  // stringstream
#include <cmath>
#include <iomanip>  // std::setprecision


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


#include "UTM.h"

using namespace boost::property_tree;

class LocationTools {
public:

  enum MetricalType {
      Km, // Kilometer
      Mi  // Miles
  };

  static const int EARTH_RADIUS_IN_KM    = 6371; // WGS 84: 6371.0088 Km
  static const int EARTH_RADIUS_IN_MILES = 3959;

  /* ************************************************************************
   * NOTE:
   *
   * Latitude == UTM coordinate y in meters
   * Longitude == UTM coordinate x in meters
   *
   ************************************************************************ */
  static double distance(const double lat_a, const double lon_a,
                  const double lat_b, const double lon_b,
                  MetricalType mt=MetricalType::Km);

  static double azimuth(const double lat_a, const double lon_a,
                  const double lat_b, const double lon_b);

  // UTM
  static std::string latlon2utm(const double lat, const double lon,
                  const int zone);
  static std::string utm2latlon(const double x, const double y,
                                const int zone, const bool south_hemi);

  static std::string get_utmvalues(const std::string xy,
                                   const std::string s, double& val);


  static const double todec(const double coord);

  static const bool isValidLat(const double coord);
  static const bool isValidLon(const double coord);

};

class UnitConverter {
public:

  static const double Kmh2Mph(const double km=0.0);
  static const double Mph2Kmh(const double  mi=0.0);
  static const double Mi2Meters(const double mi=0.0);
  static const double Meters2Mi(const double m=0.0);
  static const double Meters2Nm(const double m=0.0);
  static const double Feet2Meters(const double ft=0.0);
  static const double Meters2Feet(const double mt=0.0);
};

#endif // LOCATIONTOOLS_H

/*
 * Created  by rbh, 01/11/14
 * Modified by nas, 09/11/16
 * Modified by nas, 19/12/16
 */

#include <cassert>
#include <cmath>
#include <sstream>

#include "position.h"
#include "utilities.h"

namespace GPXP
{
  Position::Position(degrees lat, degrees lon, metres ele)
  {
      latitude  = lat;
      longitude = lon;
      elevation = ele;
  }

  Position::Position(const string & latStr,
                     const string & lonStr,
                     const string & eleStr)
  {
      latitude  = std::stod(latStr);
      longitude = std::stod(lonStr);
      elevation = std::stod(eleStr);
  }

  Position::Position(const string & ddmLatStr, char northing,
                     const string & ddmLonStr, char easting,
                     const string & eleStr)
  {
      assert(northing == 'N' || northing == 'S');
      assert(easting  == 'E' || easting  == 'W');

      degrees absoluteLat = ddmTodd(ddmLatStr);
      degrees absoluteLon = ddmTodd(ddmLonStr);

      // N/S and E/W are used with positive angles.
      assert(absoluteLat >= 0);
      assert(absoluteLon >= 0);

      latitude  = (northing == 'N') ? absoluteLat : -absoluteLat; // 'S' means negative angle
      longitude = (easting  == 'E') ? absoluteLon : -absoluteLon; // 'W' means negative angle
      elevation = std::stod(eleStr);
  }

  degrees Position::getLatitude() const
  {
      return latitude;
  }

  degrees Position::getLongitude() const
  {
      return longitude;
  }

  metres Position::getElevation() const
  {
      return elevation;
  }

  string Position::toString(bool includeElevation) const
  {
      std::ostringstream oss;

      oss << "lat=\""  << latitude  << "\"";
      oss << " lon=\"" << longitude << "\"";
      if (includeElevation) {
          oss << " ele=\"" << elevation << "\"";
      }

      return oss.str();
  }

  string Position::getElevationString() const
  {
      return std::to_string(elevation);
  }


  metres Position::distanceTo(const Position & target) const
  {
      return distanceBetween(*this, target);
  }

  /* Convert a DDM (degrees and decimal minutes) string representation of an angle to a
     DD (decimal degrees) value.
   */
  degrees Position::ddmTodd(const string & ddmStr)
  {
      double ddm  = std::stod(ddmStr);
      double degs = std::floor(ddm / 100);
      double mins = ddm - 100 * degs;
      return degs + mins / 60.0; // converts minutes (1/60th) to decimal fractions of a degree
  }

  /* Computes an approximation of the distance between two Positions.
   *
   * See: http://en.wikipedia.org/wiki/Law_of_haversines
   */
  metres distanceBetween(const Position & p1, const Position & p2)
  {
      const metres earth_mean_radius = 6371008.8;

      const radians lat1 = degToRad(p1.getLatitude());
      const radians lat2 = degToRad(p2.getLatitude());
      const radians lon1 = degToRad(p1.getLongitude());
      const radians lon2 = degToRad(p2.getLongitude());

      double h = sinSqr((lat2-lat1)/2) + cos(lat1)*cos(lat2)*sinSqr((lon2-lon1)/2);
      return 2 * earth_mean_radius * asin(sqrt(h));
  }
}

/*
 * Created  by rbh, 30/01/11
 * Modified by nas, 21/12/16
 */

#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
#include <cmath>

#include "utilities.h"
#include "xmlparser.h"
#include "track.h"

namespace GPXP
{

  Track::Track(string source, bool isFileName)
  {

      using namespace XML_Parser;
      using std::endl;

      std::ostringstream oss;

	  fileCheck(isFileName, source);

	  if (!constCheck(source, "gpx"))
	  {
		  return;
	  }

	  if (!constCheck(source, "trk"))
	  {
		  return;
	  }

	  if (!constCheck(source, "trkpt"))
	  {
		  return;
	  }

      source = getElementContent(getElement(source, "trk"));

	  if (constCheck(source, "name"))
      {
          routeName = getElementContent(getAndEraseElement(source, "name"));
          oss << endl << "track name is " << routeName;
      }

      string mergedTrkSegs;

      while (constCheck(source, "trkseg"))
      {
          string trkseg = getElementContent(getAndEraseElement(source, "trkseg"));
          string segname = getAndEraseElement(trkseg, "name");
          mergedTrkSegs += trkseg;
      }

	  if (!mergedTrkSegs.empty())
	  {
		  source = mergedTrkSegs;
	  }

      string temp = getAndEraseElement(source, "trkpt");
      string lat = getElementAttribute(getAndEraseElement(source, "trkpt"), "lat");
      string lon = getElementAttribute(getAndEraseElement(source, "trkpt"), "lon");
      string ele = getElementContent(getElement(getAndEraseElement(source, "trkpt"), "ele"));
      string time = getElementContent(getElement(getAndEraseElement(source, "trkpt"), "time"));
      string name;

      Position currentPos(lat, lon, ele);
	  addCurrPosition(currentPos, name);

      seconds startTime = timeFromString(time);
      arrived.push_back(0);
      departed.push_back(0);

      while (elementExists(source, "trkpt"))
      {
          temp = getAndEraseElement(source, "trkpt");
          lat = getElementAttribute(temp, "lat");
          lon = getElementAttribute(temp, "lon");
          ele = getElementContent(getElement(temp, "ele"));
          time = getElementContent(getElement(temp, "time"));

		  Position nextPos(lat, lon, ele);

          if (areSameLocation(currentPos, nextPos))
          {
              // If we're still at the same location, then we haven't departed yet.
              departed.back() = timeFromString(time) - startTime;
              oss << endl << "position ignored: " << nextPos.toString();
          }

          else
          {
			if (constCheck(temp,"name"))
              {
                  name = getElementContent(getElement(temp, "name"));
              }

              else
              {
                  name = "";
              }

			  addNextPosition(currentPos, nextPos, name);

              seconds timeElapsed = timeFromString(time) - startTime;
              arrived.push_back(timeElapsed);
              departed.push_back(timeElapsed);
              oss << " at time " << timeAsString(departed.back());

          }
      }

      oss << endl << positions.size() << " positions added";
	  calDistance();
      constStat(true);
      return;
  }

  seconds Track::totalTime() const
  {
      /* Computes total elapsed time between start and finish of the Track.
       * One of the 3 functions should exploit:
       * totalTime() == restingTime() + travellingTime()
       */
	  implementation(false);
      return (0);
  }

  seconds Track::restingTime() const
  {
      /* Computes total elapsed time between start and finish of the Track
       * that is spent not moving.
       * One of the 3 functions should exploit:
       * totalTime() == restingTime() + travellingTime()
       */
	  implementation(false);
      return (0);
  }

  seconds Track::travellingTime() const
  {
      /* Total elapsed time between start and finish of the Track that is
       * spent moving.
       * One of the 3 functions should exploit:
       * totalTime() == restingTime() + travellingTime()
       */
	  implementation(false);
      return (0);
  }

  speed Track::maxSpeed() const
  {
      /* Computes the greatest speed between any 2 Positions in the Track.
       */
	  implementation(false);
      return (0.0);
  }

  speed Track::averageSpeed(bool includeRests) const
  {
      /* Computes the average speed over the track.
       * The parameter determines whether the divisor is the totalTime()
       * or the travellingTime().
       */
	  implementation(false);
      return (0.0);
  }

  speed Track::maxRateOfClimb() const
  {
      /* Computes the greatest rate of climb between any 2 Positions.
       */
	  implementation(false);
      return (0.0);
  }

  speed Track::maxRateOfDescent() const
  {
      /* Computes the greatest rate of descent between any 2 Positions.
       */
	  implementation(false);
      return (0.0);
  }
}

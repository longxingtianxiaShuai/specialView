
#ifndef VB_CDT_H
#define VB_CDT_H

#include "geo_advancing.h"
#include "geo_sweep.h"
#include "geo_sweeper.h"

namespace tess {

class CDT
{
public:

  /**
   * Constructor - add polyline with non repeating points
   * 
   * @param polyline
   */
  CDT(std::vector<Point*> polyline);
  
   /**
   * Destructor - clean up memory
   */
  ~CDT();
  
  /**
   * Add a hole
   * 
   * @param polyline
   */
  void AddHole(std::vector<Point*> polyline);
  
  /**
   * Add a steiner point
   * 
   * @param point
   */
  void AddPoint(Point* point);
  
  /**
   * Triangulate - do this AFTER you've added the polyline, holes, and Steiner points
   */
  void Triangulate();
  
  /**
   * Get CDT triangles
   */
  std::vector<Triangle*> GetTriangles();
  
  /**
   * Get triangle map
   */
  std::list<Triangle*> GetMap();

  private:

  /**
   * Internals
   */
   
  SweepContext* sweep_context_;
  Sweep* sweep_;

};

}

#endif

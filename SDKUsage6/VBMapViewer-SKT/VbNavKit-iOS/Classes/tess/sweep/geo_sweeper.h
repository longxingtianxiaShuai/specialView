
#ifndef GEO_SWEEPER_H
#define GEO_SWEEPER_H

#include <vector>

namespace tess {

class SweepContext;
struct Node;
struct Point;
struct Edge;
class Triangle;

class Sweep 
{
public:

  /**
   * Triangulate
   * 
   * @param tcx
   */
  void Triangulate(SweepContext& tcx);
  
  /**
   * Destructor - clean up memory
   */
  ~Sweep();

private:

  /**
   * Start sweeping the Y-sorted point set from bottom to top
   * 
   * @param tcx
   */
  void SweepPoints(SweepContext& tcx);

  /**
   * Find closes node to the left of the new point and
   * create a new triangle. If needed new holes and basins
   * will be filled to.
   *
   * @param tcx
   * @param point
   * @return
   */
  Node& PointEvent(SweepContext& tcx, Point& point);

   /**
     * 
     * 
     * @param tcx
     * @param edge
     * @param node
     */
  void EdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

  void EdgeEvent(SweepContext& tcx, Point& ep, Point& eq, Triangle* triangle, Point& point);

  /**
   * Creates a new front triangle and legalize it
   * 
   * @param tcx
   * @param point
   * @param node
   * @return
   */
  Node& NewFrontTriangle(SweepContext& tcx, Point& point, Node& node);

  void Fill(SweepContext& tcx, Node& node);


  bool Legalize(SweepContext& tcx, Triangle& t);


  bool Incircle(Point& pa, Point& pb, Point& pc, Point& pd);


  void RotateTrianglePair(Triangle& t, Point& p, Triangle& ot, Point& op);

  void FillAdvancingFront(SweepContext& tcx, Node& n);
  
  // Decision-making about when to Fill hole. 
  // Contributed by ToolmakerSteve2
  bool LargeHole_DontFill(Node* node);
  bool AngleExceeds90Degrees(Point* origin, Point* pa, Point* pb);
  bool AngleExceedsPlus90DegreesOrIsNegative(Point* origin, Point* pa, Point* pb);
  double Angle(Point& origin, Point& pa, Point& pb);

  double HoleAngle(Node& node);


  double BasinAngle(Node& node);


  void FillBasin(SweepContext& tcx, Node& node);

  void FillBasinReq(SweepContext& tcx, Node* node);

  bool IsShallow(SweepContext& tcx, Node& node);

  bool IsEdgeSideOfTriangle(Triangle& triangle, Point& ep, Point& eq);

  void FillEdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

  void FillRightAboveEdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

  void FillRightBelowEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

  void FillRightConcaveEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

  void FillRightConvexEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

  void FillLeftAboveEdgeEvent(SweepContext& tcx, Edge* edge, Node* node);

  void FillLeftBelowEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

  void FillLeftConcaveEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

  void FillLeftConvexEdgeEvent(SweepContext& tcx, Edge* edge, Node& node);

  void FlipEdgeEvent(SweepContext& tcx, Point& ep, Point& eq, Triangle* t, Point& p);

  Triangle& NextFlipTriangle(SweepContext& tcx, int o, Triangle&  t, Triangle& ot, Point& p, Point& op);


  Point& NextFlipPoint(Point& ep, Point& eq, Triangle& ot, Point& op);


  void FlipScanEdgeEvent(SweepContext& tcx, Point& ep, Point& eq, Triangle& flip_triangle, Triangle& t, Point& p);

  void FinalizationPolygon(SweepContext& tcx);

  std::vector<Node*> nodes_;

};

}

#endif

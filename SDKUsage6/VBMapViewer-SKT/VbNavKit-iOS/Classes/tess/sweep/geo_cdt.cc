
#include "geo_cdt.h"

namespace tess {

CDT::CDT(std::vector<Point*> polyline)
{
  sweep_context_ = new SweepContext(polyline);
  sweep_ = new Sweep;
}

void CDT::AddHole(std::vector<Point*> polyline)
{
  sweep_context_->AddHole(polyline);
}

void CDT::AddPoint(Point* point) {
  sweep_context_->AddPoint(point);
}

void CDT::Triangulate()
{
  sweep_->Triangulate(*sweep_context_);
}

std::vector<tess::Triangle*> CDT::GetTriangles()
{
  return sweep_context_->GetTriangles();
}

std::list<tess::Triangle*> CDT::GetMap()
{
  return sweep_context_->GetMap();
}

CDT::~CDT()
{
  delete sweep_context_;
  delete sweep_;
}

}


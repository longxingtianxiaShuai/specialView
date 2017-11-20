
#include "geo_advancing.h"

namespace tess {

AdvancingFront::AdvancingFront(Node& head, Node& tail)
{
  head_ = &head;
  tail_ = &tail;
  search_node_ = &head;
}

Node* AdvancingFront::LocateNode(const double& x)
{
  Node* node = search_node_;

  if (x < node->value) {
    while ((node = node->prev) != NULL) {
      if (x >= node->value) {
        search_node_ = node;
        return node;
      }
    }
  } else {
    while ((node = node->next) != NULL) {
      if (x < node->value) {
        search_node_ = node->prev;
        return node->prev;
      }
    }
  }
  return NULL;
}

Node* AdvancingFront::FindSearchNode(const double& x)
{
  (void)x; // suppress compiler warnings "unused parameter 'x'"
  // TODO: implement BST index
  return search_node_;
}

Node* AdvancingFront::LocatePoint(const Point* point)
{
  const double px = point->x;
  Node* node = FindSearchNode(px);
  const double nx = node->point->x;

  if (px == nx) {
    if (point != node->point) {
      // We might have two nodes with same x value for a short time
      if (point == node->prev->point) {
        node = node->prev;
      } else if (point == node->next->point) {
        node = node->next;
      } else {
        assert(0);
      }
    }
  } else if (px < nx) {
    while ((node = node->prev) != NULL) {
      if (point == node->point) {
        break;
      }
    }
  } else {
    while ((node = node->next) != NULL) {
      if (point == node->point)
        break;
    }
  }
  if(node) search_node_ = node;
  return node;
}

AdvancingFront::~AdvancingFront()
{
}

}


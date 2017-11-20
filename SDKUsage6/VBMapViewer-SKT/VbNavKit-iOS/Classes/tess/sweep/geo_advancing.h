
#ifndef VB_ADVANCED_H
#define VB_ADVANCED_H

#include "../common/geometry.h"

namespace tess {

struct Node;

// Advancing front node
struct Node {
  Point* point;
  Triangle* triangle;

  Node* next;
  Node* prev;

  double value;

  Node(Point& p) : point(&p), triangle(NULL), next(NULL), prev(NULL), value(p.x)
  {
  }

  Node(Point& p, Triangle& t) : point(&p), triangle(&t), next(NULL), prev(NULL), value(p.x)
  {
  }

};

// Advancing front
class AdvancingFront {
public:

AdvancingFront(Node& head, Node& tail);
// Destructor
~AdvancingFront();

Node* head();
void set_head(Node* node);
Node* tail();
void set_tail(Node* node);
Node* search();
void set_search(Node* node);

/// Locate insertion point along advancing front
Node* LocateNode(const double& x);

Node* LocatePoint(const Point* point);

private:

Node* head_, *tail_, *search_node_;

Node* FindSearchNode(const double& x);
};

inline Node* AdvancingFront::head()
{
  return head_;
}
inline void AdvancingFront::set_head(Node* node)
{
  head_ = node;
}

inline Node* AdvancingFront::tail()
{
  return tail_;
}
inline void AdvancingFront::set_tail(Node* node)
{
  tail_ = node;
}

inline Node* AdvancingFront::search()
{
  return search_node_;
}

inline void AdvancingFront::set_search(Node* node)
{
  search_node_ = node;
}

}

#endif

#pragma once

#include "point2d.hpp"
#include <cmath>
#include <initializer_list>
#include <functional>

const Point2D DEFAULT_RAY_ORIGIN = Point2D(0.0f, 0.0f);
const Point2D DEFAULT_RAY_DIRECTION = Point2D(1.0f, 0.0f);

class Ray2D
{
public:
  Ray2D() = default;

  Ray2D(Ray2D const & obj)
    : m_origin(obj.m_origin), m_direction(obj.m_direction)
  {
    NormalizeDirection();
  }

  Ray2D(Point2D const & pt, Point2D const & vc)
  {
    m_origin = pt;
    m_direction = vc;
    NormalizeDirection();
  }

  Ray2D(float x1, float y1, float x2, float y2)
  {
    Point2D pt(x1, y1);
    Point2D vc(x2, y2);
    m_origin = pt;
    m_direction = vc;
    NormalizeDirection();
  }

  Ray2D(std::initializer_list<Point2D> const & lst)
  {
    Point2D * vals[] = { &m_origin, &m_direction };
    int const count = sizeof(vals) / sizeof(vals[0]);
    auto it = lst.begin();
    for (int i = 0; i < count && it != lst.end(); i++, ++it)
      *vals[i] = *it;
    NormalizeDirection();
  }
  
  // constructor move
  Ray2D(Ray2D && obj)
  {
    std::swap(m_origin, obj.m_origin );
    std::swap(m_direction, obj.m_direction );
  }
  
  // operator move
  Ray2D & operator = (Ray2D && obj)
  {
    if (this == &obj) return *this;
    std::swap(m_origin , obj.m_origin );
    std::swap(m_direction, obj.m_direction);
    return *this;
  }
  
  // operator logical equality
  bool operator == (Ray2D const & obj) const
  {
    return ((m_origin == obj.m_origin) && (m_direction == obj.m_direction));
  }

  bool operator != (Ray2D const & obj) const
  {
    return !operator==(obj);
  }

  Ray2D & operator = (Ray2D const & obj)
  {
    if (this == &obj) return *this;
    m_origin = obj.m_origin;
    m_direction = obj.m_direction;
    return *this;
  }
  Point2D operator [] (unsigned int index) const
  {
    if (index >= 2) return Point2D(0.0f,0.0f);
    return index == 0 ? m_origin : m_direction;
  }
  Point2D const & origin () const
  {
    return m_origin;
  }

  Point2D const & direction () const
  {
    return m_direction;
  }
  friend std::ostream & operator << (std::ostream & os, Ray2D const & obj)
  {
    os << "Ray2D {" << obj.origin() << ", " << obj.direction() << "}";
    return os;
  }
private:

  void NormalizeDirection()
  {
    try
    {
      m_direction.Normalize();
    }
    catch (IncorrectDataExceptions const & ex)
    {
      std::cerr << ex.what() << std::endl;
      throw;
    }
  }

  Point2D m_origin = DEFAULT_RAY_ORIGIN;
  Point2D m_direction = DEFAULT_RAY_DIRECTION;
};

#pragma once

#include "point2d.hpp"
#include "ray2d.hpp"
#include <cmath>
#include <initializer_list>
#include <functional>
#include <ostream>

const Point2D DEFAULT_BOX_MIN = Point2D(0.0f, 0.0f);
const Point2D DEFAULT_BOX_MAX = Point2D(1.0f, 1.0f);

class Box2D
{
public:
  // Разрешаем конструирование по умолчанию.
  Box2D() = default;

  // Конструктор копирования.
  Box2D(Box2D const & obj)
    : m_min(obj.m_min), m_max(obj.m_max)
  {
    BoxCorrect();
  }

  // Конструктор с параметрами.
  Box2D(Point2D const & min, Point2D const & max)
    : m_min(min), m_max(max)
  {
    BoxCorrect();
  }

  // Оператор логического равенства.
  bool operator == (Box2D const & obj) const
  {
    return (m_min == obj.m_min) && (m_max == obj.m_max);
  }

  Point2D const & boxMin() const { return m_min; }
  Point2D const & boxMax() const { return m_max; }

  // Конструктор со списком инициализации.
  Box2D(std::initializer_list<Point2D> const & lst)
  {
    Point2D * vals[] = { &m_min, &m_max };
    int const count = sizeof(vals) / sizeof(vals[0]);
    auto it = lst.begin();
    for (int i = 0; i < count && it != lst.end(); i++, ++it)
      *vals[i] = *it;
    BoxCorrect();
  }
  
  // Конструктор перемещения
  Box2D(Box2D && obj)
  {
    std::swap(m_min, obj.m_min);
    std::swap(m_max, obj.m_max);
  }
  
  //Оператор перемещения
  Box2D & operator = (Box2D && obj)
  {
    if (this == &obj) return *this;
    std::swap(m_min, obj.m_min);
    std::swap(m_max, obj.m_max);
    return *this;
  }

  // Оператор присваивания.
  Box2D & operator = (Box2D const & obj)
  {
    if (this == &obj) return *this;
    m_min = obj.m_min;
    m_max = obj.m_max;
    return *this;
  }

  // Оператор логического неравенства.
  bool operator != (Box2D const & obj) const
  {
    return !operator==(obj);
  }

  // Переопределение оператора [].
  Point2D operator [] (unsigned int index) const
  {
    if (index >= 2) return Point2D(0.0f,0.0f);
    return index == 0 ? m_min : m_max;
  }

  Box2D operator += (const Point2D & p)
  {
    m_min += p;
    m_max += p;
    return *this;
  }

  Box2D operator + (const Point2D & p) const
  {
    return Box2D(m_min + p, m_max + p);
  }

  Point2D BoxCenter()
  {
    return Point2D( (boxMin().x() + boxMax().x()) / 2, (boxMin().y() + boxMax().y()) / 2 );
  }

  bool BoxesIntersect(const Box2D &obj)
  {
    if (m_max.x() < obj.boxMin().x()) return false; // a is left of b
    if (m_min.x() > obj.boxMax().x()) return false; // a is right of b
    if (m_max.y() < obj.boxMin().y()) return false; // a is above b
    if (m_min.y() > obj.boxMax().y()) return false; // a is below b
    return true;
  }

  bool Inside(const Point2D &obj)
  {
    return (obj.x() <= m_max.x()) && (obj.y() <= m_max.y()) && (obj.x() >= m_min.x()) && (obj.y() >= m_min.y());
  }

  bool BoxRayIntersect(const Ray2D &obj)
  {
    if (Inside(obj.origin()))
    {
      return true;
    }
    else
    {
      float kRay = obj.direction().y() / obj.direction().x();
      float bRay = obj.origin().y() - kRay * obj.origin().x();
      float kDia1 = (m_max.y()- m_min.y()) / (m_max.x()- m_min.x());
      float kDia2 = -kDia1;
      float bDia1 = m_max.y() - kDia1 * m_max.x();
      float bDia2 = m_max.y() - kDia2 * m_min.x();
      float x;
      float y;
      if (!EqualWithEps(kRay, kDia1))
      {
        x = - (bRay - bDia1) / (kRay - kDia1);
        y = kRay * x + bRay;
        if (!Inside(Point2D(x,y)))
        {
          if (!EqualWithEps(kRay, kDia2))
          {
            x = - (bRay - bDia2) / (kRay - kDia2);
            y = kRay * x + bRay;
          }
          else
          {
            return false;
          }
        }
      }
      else
      {
        x = - (bRay - bDia2) / (kRay - kDia2);
        y = kRay * x + bRay;
      }
      if (!Inside(Point2D(x,y)))
      {
        return false;
      }
      if (!obj.origin().EqualWithEpsX(0.0f))
      {
        if ((x - obj.origin().x()) * obj.direction().x() > 0)
          return true;
      }
      else
      {
        if ((y - obj.origin().y()) * obj.direction().y() > 0)
          return true;
      }
      return false;
    }
  }
  friend std::ostream & operator << (std::ostream & os, Box2D const & obj)
  {
    os << "Box2D {" << obj.boxMin() << ", " << obj.boxMax() << "}";
    return os;
  }
private:
  bool EqualWithEps(float v1, float v2) const
  {
    return fabs(v1 - v2) < kEps;
  }
  void BoxCorrect()
  {
    try
    {
      if (m_min.EqualWithEpsX(m_max.x())) throw IncorrectDataExceptions("box have 0 width");
      if (m_min.EqualWithEpsY(m_max.y())) throw IncorrectDataExceptions("box have 0 length");
      Point2D p1 = Point2D(std::min(m_min.x(), m_max.x()), std::min(m_min.y(), m_max.y()));
      Point2D p2 = Point2D(std::max(m_min.x(), m_max.x()), std::max(m_min.y(), m_max.y()));
      m_min = p1;
      m_max = p2;
    }
    catch (IncorrectDataExceptions const & ex)
    {
      std::cerr << ex.what() << std::endl;
      m_min = DEFAULT_BOX_MIN;
      m_max = DEFAULT_BOX_MAX;
      throw;
    }
  }

  Point2D m_min = DEFAULT_BOX_MIN, m_max = DEFAULT_BOX_MAX;
};

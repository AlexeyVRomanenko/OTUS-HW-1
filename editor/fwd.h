#pragma once
#include <editor/fwd.h>

#include <memory>

using AppPtr = std::shared_ptr<class App>;
using DocumentPtr = std::shared_ptr<class Document>;
using LinePtr = std::shared_ptr<class Line>;
using PointPtr = std::shared_ptr<class Point>;
using CirclePtr = std::shared_ptr<class Circle>;
using RectanglePtr = std::shared_ptr<class Rectangle>;
using ShapePtr = std::shared_ptr<class Shape>;
using ShapesPtr = std::shared_ptr<class Shapes>;
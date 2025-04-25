#ifndef GEOMETRY
#define GEOMETRY

/*!
\brief The geometry of the mesh defines the coordinates
*/
template<int G>
class geometry
{
  public:
  std::vector<std::array<double,G>> coords;
};

#endif
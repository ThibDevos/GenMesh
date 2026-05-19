#ifndef CORE_MATH_H
#define CORE_MATH_H

#include <iostream>

template<size_t D, typename T1, typename T2>
double inner(T1 a, T2 b)
{
  double res = 0.;
  for(int i=0; i<D; ++i){res+= a[i]*b[i];}
  return res;
}

#endif
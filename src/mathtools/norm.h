/****************************************************************************
**  FougTools
**  Copyright Fougue (30 Mar. 2015)
**  contact@fougue.pro
**
** This software is a computer program whose purpose is to provide utility
** tools for the C++ language and the Qt toolkit.
**
** This software is governed by the CeCILL-C license under French law and
** abiding by the rules of distribution of free software.  You can  use,
** modify and/ or redistribute the software under the terms of the CeCILL-C
** license as circulated by CEA, CNRS and INRIA at the following URL
** "http://www.cecill.info".
****************************************************************************/

#ifndef MATHTOOLS_NORM_H
#define MATHTOOLS_NORM_H

#include "norm_traits.h"
#include "vec_traits.h"

namespace math {

namespace internal {

template<typename VEC> struct VecTraitsHelper
{
  typedef typename NumTraits< typename VecTraits<VEC>::CoordType >::Real Real;
};

} // namespace internal

/*! \brief Computation of norms in K-vector space
 *
 *  \tparam FUNC Class providing the concrete computation functions
 *
 *  Example:
 *  \code
 *  #include <mathtools/manhattan_norm.h>
 *  #include <mathtools/euclidean_norm.h>
 *  #include <mathtools/maximum_norm.h>
 *  #include <mathtools/sqr_euclidean_norm.h>
 *  #include <iostream>
 *
 *  int main()
 *  {
 *    const double vec3[] = { 0, 1.5, 15 };
 *    std::cout << "Euclidean norm: " << math::EuclideanNorm::fromArray(vec3) << std::endl;
 *    std::cout << "Manhattan norm: " << math::ManhattanNorm::fromArray(vec3) << std::endl;
 *    std::cout << "Maximum   norm: " << math::MaximumNorm::fromArray(vec3)   << std::endl;
 *    std::cout << "Squared euclidenan norm: " << math::SqrEuclideanNorm::fromArray(vec3) << std::endl;
 *    return 0;
 *  }
 *  \endcode
 *
 *  \headerfile norm.h <mathtools/norm.h>
 *  \ingroup mathtools
 */
template<typename FUNC>
struct Norm
{
  /*! \brief Returns the norm of the vector with coordinates in iterator range [ \p begin , \p end ]
   *  \tparam COORD_ITERATOR Type of the iterator over the vector's coordinates
   */
  template<typename COORD_ITERATOR>
#ifndef DOXYGEN
  static auto fromRange(COORD_ITERATOR begin, COORD_ITERATOR end) -> decltype(internal::typeHelper(*begin))
  { return FUNC::fromRange(begin, end); }
#else
  static CompatibleRealType fromRange(COORD_ITERATOR begin, COORD_ITERATOR end);
#endif

  template<typename COORD_ITERATOR>
  static bool isNullRange(COORD_ITERATOR begin, COORD_ITERATOR end)
  {
    typedef decltype(*begin) CoordType;
    return Norm::fromRange(begin, end) < NumTraits<CoordType>::precision();
  }


  /*! \brief Returns the norm of the vector with its N coordinates stored in memory at \p coordPtr
   *  \tparam N Size of the vector (count of coordinates)
   *  \tparam COORD_TYPE Type of the vector's corrdinates
   */
  template<std::size_t N, typename COORD_TYPE>
#ifndef DOXYGEN
  static typename NumTraits<COORD_TYPE>::Real fromPtr(const COORD_TYPE* coordPtr)
  {
    typename internal::NormTraits<FUNC>::NormCategory normCategory;
    return Norm::fromPtrDispatch<N, COORD_TYPE>(coordPtr, normCategory);
  }
#else
  static CompatibleRealType fromPtr(const COORD_TYPE* coordPtr);
#endif

  template<std::size_t N, typename COORD_TYPE>
  static bool isNullPtr(const COORD_TYPE* coordPtr)
  { return Norm::fromPtr(coordPtr) < NumTraits<COORD_TYPE>::precision(); }


  /*! \brief Returns the norm of the vector with its N coordinates stored in array \p coordArray
   */
  template<std::size_t N, typename COORD_TYPE>
#ifndef DOXYGEN
  static typename NumTraits<COORD_TYPE>::Real fromArray(const COORD_TYPE (&coordArray)[N])
  { return Norm::fromPtr<N, COORD_TYPE>(&coordArray[0]); }
#else
  static CompatibleRealType fromArray(const COORD_TYPE (&coordArray)[N]);
#endif

  template<std::size_t N, typename COORD_TYPE>
  static bool isNullArray(const COORD_TYPE (&coordArray)[N])
  { return Norm::fromArray(coordArray) < NumTraits<COORD_TYPE>::precision(); }


  /*! \brief Returns the norm of the vector object \p vec
   */
  template<typename VEC>
#ifndef DOXYGEN
  static typename internal::VecTraitsHelper<VEC>::Real fromObject(const VEC& vec)
  {
    typename VecTraits<VEC>::AccessCategory accessCategory;
    return Norm::fromObjectDispatch(vec, accessCategory);
  }
#else
  static CompatibleRealType fromObject(const VEC& vec);
#endif

  template<typename VEC>
  static bool isNullObject(const VEC& vec)
  {
    typedef typename VecTraits<VEC>::CoordType CoordType;
    return Norm::fromObject(vec) < NumTraits<CoordType>::precision();
  }

#ifndef DOXYGEN
  // -- Implementation

private:
  template<std::size_t N, typename COORD_TYPE>
  static typename NumTraits<COORD_TYPE>::Real fromPtrDispatch(const COORD_TYPE* coordPtr,
                                                              internal::DefaultNormSpecializationTag)
  { return Norm::fromRange(coordPtr, coordPtr + N); }


  template<std::size_t N, typename COORD_TYPE>
  static typename NumTraits<COORD_TYPE>::Real fromPtrDispatch(const COORD_TYPE* coordPtr,
                                                              internal::ArityNormSpecializationTag)
  { return FUNC::template fromPtr<N, COORD_TYPE>(coordPtr); }


  template<typename VEC>
  static typename internal::VecTraitsHelper<VEC>::Real fromObjectDispatch(const VEC& vec,
                                                                          StlIteratorVecAccessTag)
  { return Norm::fromRange(vec.begin(), vec.end()); }


  template<typename VEC>
  static typename internal::VecTraitsHelper<VEC>::Real fromObjectDispatch(const VEC& vec,
                                                                          FuncIteratorVecAccessTag)
  { return Norm::fromRange(VecAccess<VEC>::begin(vec), VecAccess<VEC>::end(vec)); }


  template<typename VEC>
  static typename internal::VecTraitsHelper<VEC>::Real fromObjectDispatch(const VEC& vec,
                                                                          PointerVecAccessTag)
  {
    typedef typename VecTraits<VEC>::CoordType CoordType;
    return Norm::fromPtr<VecTraits<VEC>::Arity, CoordType>(VecAccess<VEC>::pointer(vec));
  }


  template<typename VEC>
  static typename internal::VecTraitsHelper<VEC>::Real fromObjectDispatch(const VEC& vec,
                                                                          StdArrayVecAccessTag)
  {
    const auto&& array = VecAccess<VEC>::stdarray(vec);
    return Norm::fromRange(array.cbegin(), array.cend());
  }

#endif // !DOXYGEN
};

} // namespace math

#endif // MATHTOOLS_NORM_H

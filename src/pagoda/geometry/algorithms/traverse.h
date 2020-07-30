#pragma once

#include <pagoda/geometry/algorithms/traverse.h>
#include <pagoda/geometry/core/geometry.h>

#include <functional>

namespace pagoda::geometry::algorithms
{
/**
 * Calls function \p f with each edge in geometry \p geometry.
 */
template<class G, class F>
void EachEdge(G* geometry, const F& f)
{
	auto edgesEnd = geometry->EdgesEnd();
	for (auto eIter = geometry->EdgesBegin(); eIter != edgesEnd; ++eIter)
	{
		f(geometry, *eIter);
	}
}

/**
 * Calls function \p f with each split point in \p geometry.
 */
template<class G, class F>
void EachSplitPoint(G* geometry, const F& f)
{
	auto splitPointsEnd = geometry->SplitPointsEnd();
	for (auto spIter = geometry->SplitPointsBegin(); spIter != splitPointsEnd; ++spIter)
	{
		f(geometry, *spIter);
	}
}

/**
 * Calls function \p f with each face in \p geometry.
 */
template<class G, class F>
void EachFace(G* geometry, const F& f)
{
	auto facesEnd = geometry->FacesEnd();
	for (auto fIter = geometry->FacesBegin(); fIter != facesEnd; ++fIter)
	{
		f(geometry, *fIter);
	}
}

/**
 * Calls function \p f with each point in \p geometry.
 */
template<class G, class F>
void EachPoint(G* geometry, const F& f)
{
	auto pointsEnd = geometry->PointsEnd();
	for (auto pIter = geometry->PointsBegin(); pIter != pointsEnd; ++pIter)
	{
		f(geometry, *pIter);
	}
}

/**
 * Calls function \p f with each point around face \p face in \p geometry.
 */
template<class G, class F>
void EachPointAroundFace(G* geometry, const typename G::FaceHandle& face, const F& f)
{
	for (auto fpCirc = geometry->FacePointCirculatorBegin(face); fpCirc.IsValid(); ++fpCirc)
	{
		f(geometry, *fpCirc);
	}
}

/**
 * Calls function \p f with each point around each face in \p geometry.
 */
template<class G, class F>
void EachPointAroundEachFace(G* geometry, const F& f)
{
	auto facesEnd = geometry->FacesEnd();
	for (auto fIter = geometry->FacesBegin(); fIter != facesEnd; ++fIter)
	{
		for (auto fpCirc = geometry->FacePointCirculatorBegin(*fIter); fpCirc.IsValid(); ++fpCirc)
		{
			f(geometry, *fIter, *fpCirc);
		}
	}
}

/**
 * Calls function \p f with each split point around face \p face in \p geometry.
 */
template<class G, class F>
void EachSplitPointAroundFace(G* geometry, const typename G::FaceHandle& face, const F& f)
{
	for (auto fspCirc = geometry->FaceSplitPointCirculatorBegin(face); fspCirc.IsValid(); ++fspCirc)
	{
		f(geometry, *fspCirc);
	}
}
}  // namespace pagoda::geometry::algorithms

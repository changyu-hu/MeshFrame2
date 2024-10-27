/*!
*      \file vertex.h
*      \brief Base vertex Class for all types of Tetrahedron Mesh Classes
*
*		This is the fundamental class for vertex
*	   \date 10/10/2022
*
*/

#ifndef _TMESHLIB_VERTEX_STATIC_H_
#define _TMESHLIB_VERTEX_STATIC_H_

#include <list>
#include <vector>
#include "../Memory/Array.h"
#include "Vertex.h"
#include "../Types/TypeDefs.h"

namespace MF
{
	namespace TetMesh
	{
		template <typename DType>
		class CVertexStatic: public CVertexBase
		{
		public:
			CVertexStatic() { };
			~CVertexStatic(){ }

			void setPVertPos(TVerticesMat<DType>* pVertPos) {
				p = pVertPos;
			}

			TVec3<DType> position() {
				return p->col(m_id).head(3);
			}

			 
		protected:
			
			TVerticesMat<DType>* p;
		};
	};
};

#endif
/*
 * Copyright (c) 2013, Hernan Saez
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "NavigationMeshContainer.hpp"

#include "Debug/DebugRenderHelper.hpp"

#include "Primitives/Primitive.hpp"

#include "SceneGraph/Node.hpp"

using namespace crimild;
using namespace crimild::navigation;

NavigationMeshContainer::NavigationMeshContainer( NavigationMeshPtr const &mesh )
	: _navigationMesh( mesh )
{

}

NavigationMeshContainer::~NavigationMeshContainer( void )
{

}

void NavigationMeshContainer::renderDebugInfo( Renderer *renderer, Camera *camera )
{
	std::vector< Vector3f > exteriorEdges;
	std::vector< Vector3f > interiorEdges;
	std::vector< Vector3f > normals;
	std::vector< Vector3f > cells;

	static const auto OFFSET = Vector3f( 0.0f, 0.0f, 0.0f );

	getNavigationMesh()->foreachCell( [ &exteriorEdges, &interiorEdges, &cells, &normals ]( NavigationCellPtr const &cell ) {
		for ( int i = 0; i < 3; i++ ) {
			cells.push_back( cell->getVertex( i ) );
		}

		cell->foreachEdge( [&exteriorEdges, &interiorEdges]( NavigationCellEdgePtr const &e ) {
			if ( e->getNeighbor() != nullptr ) {
				interiorEdges.push_back( OFFSET + e->getPointAt( 0 ) );
				interiorEdges.push_back( OFFSET + e->getPointAt( 1 ) );
			}
			else {
				exteriorEdges.push_back( OFFSET + e->getPointAt( 0 ) );
				exteriorEdges.push_back( OFFSET + e->getPointAt( 1 ) );
			}
		});

		auto n = cell->getNormal();
		auto c = cell->getCenter();
		n *= 0.5f;
		normals.push_back( c );
		normals.push_back( c + n );
	});

	if ( exteriorEdges.size() > 0 ) {
		DebugRenderHelper::renderLines( renderer, camera, &exteriorEdges[ 0 ], exteriorEdges.size(), RGBAColorf( 0.0f, 0.0f, 0.5f, 1.0f ) );
	}

	if ( interiorEdges.size() > 0 ) {
		DebugRenderHelper::renderLines( renderer, camera, &interiorEdges[ 0 ], interiorEdges.size(), RGBAColorf( 0.75f, 0.75f, 1.0f, 1.0f ) );
	}

	if ( normals.size() > 0 ) {
		DebugRenderHelper::renderLines( renderer, camera, &normals[ 0 ], normals.size(), RGBAColorf( 0.0f, 1.0f, 0.0f, 1.0f ) );
	}		

	if ( cells.size() > 0 ) {
		auto primitive = crimild::alloc< Primitive >();
		primitive->setVertexBuffer( crimild::alloc< VertexBufferObject >( VertexFormat::VF_P3, cells.size(), ( float * ) &cells[ 0 ] ) );

		auto ibo = crimild::alloc< IndexBufferObject >( cells.size(), nullptr );
		for ( int i = 0; i < cells.size(); i++ ) {
			ibo->setIndexAt( i, i );
		}
		primitive->setIndexBuffer( ibo );

		Transformation t = getNode()->getWorld();
		t.translate() += OFFSET;
		DebugRenderHelper::render( renderer, camera, crimild::get_ptr( primitive ), t, RGBAColorf( 0.5f, 0.5f, 1.0f, 0.5f ) );
	}
}


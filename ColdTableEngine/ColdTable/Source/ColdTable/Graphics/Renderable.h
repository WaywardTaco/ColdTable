#pragma once

#include <intsafe.h>
#include <ColdTable/Core/Base.h>
#include <ColdTable/Core/Common.h>
#include <ColdTable/Math/Vertex.h>
#include <ColdTable/Graphics/GraphicsEnums.h>

namespace ColdTable
{
	class Renderable : public Base
	{
	public:
		explicit Renderable(const RenderableDesc& desc);
		virtual ~Renderable() override;

		void LoadVertices(const Vertex* vertexList, UINT listSize);
		void SetShader(ShaderPtr shader);

	private:
		VertexBufferPtr _vertexBuffer;
		EGeometryDrawmode _drawMode;
		ShaderPtr _shader;

	private:
		friend class DeviceContext;

	};
}


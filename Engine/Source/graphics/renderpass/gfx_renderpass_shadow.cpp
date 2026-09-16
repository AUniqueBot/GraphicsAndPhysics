#include <graphics/renderpass/gfx_renderpass_shadow.h>
#include <arch/resources/res_scene.h>

void ShadowPass::Execute(
	GraphicsContext& _context, 
	const PreparedSceneRenderData& _currentScene
) {

	for (const LightRenderable& data: _currentScene.lights) {
		Transform& lightTrs = *data.transform;
		Light& lightData = *data.light;
		

		// apply light ubo
		_context.BindResource();


		for (const MeshRenderable& mData : _currentScene.meshes) {
			Transform& meshTrs = *mData.transform;
			MeshRenderer& meshData = *mData.mesh;

			// apply UBO.
			_context.BindResource();

			// apply mesh.
			_context.BindResource();

			// render mesh.
			_context.DrawIndexed({
				.indexOffset = 0,
				.indexCount = 1,
				.instanceStart = 0,
				.instanceCount = 0
			});
		}

	}
	
	
	_context.BindResource();


	
}
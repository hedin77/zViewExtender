// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	zCViewItemRenderer::zCViewItemRenderer() : zCViewInteractive() {
		InitProperties();
	}


	zCViewItemRenderer::zCViewItemRenderer(int a0, int a1, int a2, int a3, zTviewID a4) : zCViewInteractive(a0, a1, a2, a3, a4) {
		InitProperties();
	}


	void zCViewItemRenderer::InsertItem(oCItem* item) {
		renderItem = item; 
	}

	void zCViewItemRenderer::RemoveItem() {
		renderItem = NULL;
	}


	zCViewItemRenderer::~zCViewItemRenderer() {
		renderWorld->DisposeWorld();
		renderWorld->Release();
	}


	void zCViewItemRenderer::InitProperties() {
		renderWorld = zfactory->CreateWorld();
		renderWorld->GetBspTree()->bspTreeMode = zBSP_MODE_INDOOR;
		renderWorld->GetActiveSkyControler()->SetFillBackground(False);
		renderWorld->m_bIsInventoryWorld = true;
	}


	void zCViewItemRenderer::Blit() {		
		zCViewInteractive::Blit();

		if (renderItem != NULL && renderItem) {
			if (!renderItem->GetVisual()) {
				renderItem->CreateVisual();
			}

			if (renderItem->GetVisual()) {
				renderItem->RenderItem(renderWorld, this, -2.0f);
			}
		}
	}
}
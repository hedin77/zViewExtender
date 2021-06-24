// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	zCViewRenderer::zCViewRenderer() : zCViewInteractive(){
		InitProperties();
	}


	zCViewRenderer::zCViewRenderer( int a0, int a1, int a2, int a3, zTviewID a4 ) : zCViewInteractive( a0, a1, a2, a3, a4 ){
		InitProperties();
	}


	void zCViewRenderer::PlaceCamToVob() {
		// Вычисление отношения основания
		// треугльника к катетам
		float fovH, fovV;
		zCCamera::activeCam->GetFOV( fovH, fovV );

    float fov   = fovV * RAD;
    float alpha = (RAD180 - fov) * 0.5f;
    float edgeA = 2.0f * cos( alpha );

		zVEC3 camPosition = Properties.Camera.Position;
		zMAT4& camTrafo		= zCCamera::activeCam->connectedVob->trafoObjToWorld;
		zTBBox3D bbox			=
			Properties.BBox.Auto && Properties.BBox.RealTimeUpdate ?
			Properties.Scene.Vob->visual->GetBBox3D() :
			Properties.BBox.CustomBBox;

    float z  = bbox.maxs[VZ];                 // Ближайшая к камере точка модели
    float w  = bbox.maxs[VX] - bbox.mins[VX]; // Ширина модели
    float h  = bbox.maxs[VY] - bbox.mins[VY]; // Высота модели
    float wh = max( w, max(h, bbox.maxs[VZ] - bbox.mins[VZ])); // Наилучшая длина для вписания модели во вьюпорт
    float d  = SafeDiv( wh, edgeA );          // Наилучшая дистанция камеры от модели
    float x  = bbox.mins[VX] + w * 0.5f;      // Наилучшая позиция камеры по X
    float y  = bbox.mins[VY] + h * 0.5f;      // Наилучшая позиция камеры по Y

		// 
		camPosition[VX] =  x;
		camPosition[VY] =  y;
		camPosition[VZ] = (z + d) * Properties.Camera.Distance;
		
    // Пересчитываем наилучшую матрицу
    // камеры с учетом ее поворота относительно
    // модели на угол angle
		camTrafo.MakeIdentity();
		camTrafo.SetTranslation( camPosition );
		camTrafo.PostRotateY( 180.0f );
		camTrafo =
      Alg_Rotation3D( zVEC3( 0.0f, 0.0f, 1.0f ), -Properties.Camera.Rotation[VZ] ) *
      Alg_Rotation3D( zVEC3( 0.0f, 1.0f, 0.0f ), -Properties.Camera.Rotation[VY] ) *
      Alg_Rotation3D( zVEC3( 1.0f, 0.0f, 0.0f ), -Properties.Camera.Rotation[VX] ) *
			camTrafo;
	}


	void zCViewRenderer::InsertVisual( zCVob* vob ) {
		InsertVisual( vob->GetVisual() );
	}


	void zCViewRenderer::InsertVisual( zCVisual* visual ) {
		Properties.Scene.Vob->SetVisual( visual );
		if( Properties.BBox.Auto )
			Properties.BBox.CustomBBox = visual->GetBBox3D();
	}


	zCViewRenderer::~zCViewRenderer() {
		Properties.Scene.World->DisposeWorld();
		Properties.Scene.World->Release();
	}


	void zCViewRenderer::InitProperties() {
    zCCamera* camera		 = new zCCamera();
		camera->connectedVob = new zCVob();
		camera->targetView	 = this;
		camera->farClipZ		 = 999999.0f;
		camera->vobFarClipZ	 = 999999.0f;

		zCVob* vob = new zCVob();
#if ENGINE >= Engine_G2
		vob->m_fVobFarClipZScale = 999999.0f;
#endif

		zCWorld* world = zfactory->CreateWorld();
		world->GetBspTree()->bspTreeMode = zBSP_MODE_INDOOR;
		world->GetActiveSkyControler()->SetFillBackground( False );
		world->AddVob( camera->connectedVob );
		world->AddVob( vob );
		camera->connectedVob->Release();
		vob->Release();

		Properties.Camera.Position		 = 0.0f;
		Properties.Camera.Rotation		 = 0.0f;
		Properties.Camera.FOV					 = 25.0f;
		Properties.Camera.Distance		 = 1.4f;
		Properties.BBox.Auto					 = True;
		Properties.BBox.RealTimeUpdate = False;
		Properties.Scene.World				 = world;
		Properties.Scene.Camera				 = camera;
		Properties.Scene.Vob					 = vob;
		Properties.Scene.Light				 = GFX_WHITE;
	}


	void zCViewRenderer::Blit() {
		zCViewInteractive::Blit();
		
		zCCamera* activeCam = zCCamera::activeCam;
		zCCamera::activeCam = Properties.Scene.Camera;
		zCEventManager::disableEventManagers = True;
		zrenderer->SetAlphaBlendFunc( zRND_ALPHA_FUNC_BLEND );
		zCCamera::activeCam->SetFOV( Properties.Camera.FOV );

		Properties.Scene.Vob->lightColorStat = Properties.Scene.Light;
		PlaceCamToVob();
		Properties.Scene.World->Render( *zCCamera::activeCam );

		zCEventManager::disableEventManagers = False;
		zCCamera::activeCam = activeCam;
	}
}
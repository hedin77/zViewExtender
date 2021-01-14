// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class VIEWAPI zCViewRenderer : public zCViewInteractive {
		void InitProperties();

	public:
		zCViewRenderer();
		zCViewRenderer( int a0, int a1, int a2, int a3, zTviewID a4 = zTviewID::VIEW_ITEM );

		void PlaceCamToVob();
		void InsertVisual( zCVob* vob );
		void InsertVisual( zCVisual* visual );
		virtual void Blit();
		virtual ~zCViewRenderer();


		// Render properties
		struct {
			struct {
				zVEC3 Position;
				zVEC3 Rotation;
				float FOV;
				float Distance;
			} Camera;

			struct {
				bool_t Auto;
				bool_t RealTimeUpdate;
				zTBBox3D CustomBBox;
			} BBox;

			struct {
				zCWorld* World;
				zCCamera* Camera;
				zCVob* Vob;
				zCOLOR Light;
			} Scene;
		}
		Properties;
	};
}
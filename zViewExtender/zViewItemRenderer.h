// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	class VIEWAPI zCViewItemRenderer : public zCViewInteractive {
		void InitProperties();

	public:
		zCViewItemRenderer();
		zCViewItemRenderer(int a0, int a1, int a2, int a3, zTviewID a4 = zTviewID::VIEW_ITEM);

		void InsertItem(oCItem* item);
		void RemoveItem();
		virtual void Blit();
		virtual ~zCViewItemRenderer();

		oCItem* renderItem;
		zCWorld* renderWorld;
	};
}
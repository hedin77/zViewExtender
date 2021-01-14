// Supported with union (c) 2020 Union team
// Union HEADER file

namespace GOTHIC_ENGINE {
	struct zTItemPanel : public zCView {
		zCViewInteractive* button;
		zCView* panel;
		zCView* highlight;
		zTItemPanel();
		void Init();
		void SetSelected(bool selected);
		void SetColor( zCOLOR color );
		virtual ~zTItemPanel();
	};

	struct zTInventoryDrawFlag {
		zTInventoryDrawFlag();
		string flagName;
		uint mainFlag;
		uint flag;
		zCViewInteractive* view;
		void InitViewSize();
	};

	struct zTInventoryCategory {
		zTInventoryCategory();
		string categoryName;
		Array<zTInventoryDrawFlag> flags;
		zCViewInteractive* view;
		void InitViewSize();
	};
	
	class zCInventory {
		zTInventoryCategory& GetOrCreateCategoty( string categoryName );
		zTInventoryDrawFlag& GetOrCreateFlag( string categoryName, string flagName );

	public:
		zCView* background;
		zCView* vseparator;
		zCView* category;
		zCView* column;
		zCView* hseparator;
		uint selectedCategoty;
		uint selectedFlag;
		Array<zTInventoryCategory> categories;

		zCInventory();
		void Init();
		void Open();
		void Close();
		void DrawCategories();
		void AddCategoty( string catName, string flagName, uint mainFlag, uint flag );
	};
}
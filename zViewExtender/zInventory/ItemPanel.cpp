// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	zTItemPanel::zTItemPanel() : zCView(0, 0, 10, 10) {
		screen->InsertItem( this );
		Init();
		screen->RemoveItem( this );
	}



	void zTItemPanel::Init() {
		button		= new zCViewInteractive( 0, 0, 8192, 8192 );
		panel			= new zCView( 0, 0, 8192, 8192 );
		highlight = new zCView( 0, 0, 8192, 8192 );

		InsertItem( button );
		InsertItem( panel );
		InsertItem( highlight );

		button->InsertBack( "Inventory_Cell" );
		panel->InsertBack( "Inventory_Cell_Typebar" );
		highlight->InsertBack( "Inventory_Cell_Highlight" );

		button->SetSize( 8192, 8192 );
		panel->SetSize( 8192, 8192 );
		highlight->SetSize( 8192, 8192 );

		//panel->SetAlphaBlendFunc( zRND_ALPHA_FUNC_ADD );
	}



	void zTItemPanel::SetSelected( bool selected ) {
		highlight->alpha = selected ? 255 : 0;
	}



	void zTItemPanel::SetColor( zCOLOR color ) {
		panel->color     = color;
		highlight->color = color;
	}



	zTItemPanel::~zTItemPanel() {
		delete button;
		delete panel;
		delete highlight;
	}
}
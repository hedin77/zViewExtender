// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
	static zCViewShaped* s_TestShape = Null;


	bool TurnLeft( zCViewInteractive* button, zCViewCursor* cursor, zEMouseButton mouse ) {
		s_TestShape->RotateShape( -5.0f );
		return true;
	}

	bool TurnRight( zCViewInteractive* button, zCViewCursor* cursor, zEMouseButton mouse ) {
		s_TestShape->RotateShape( +5.0f );
		return true;
	}




	uint hand1;
	uint hand2;
	uint hand3;
	uint skull1;
	uint skull2;

	bool ShapeDrag( zCViewInteractive* button, zCViewCursor* cursor ) {
		return true;
	}

	bool ShapeMouseDown( zCViewInteractive* button, zCViewCursor* cursor, zEMouseButton mouse ) {
		button->CursorVisualIndex = hand2;
		return true;
	}

	bool ShapeMouseUp( zCViewInteractive* button, zCViewCursor* cursor, zEMouseButton mouse ) {
		button->CursorVisualIndex = hand1;
		return true;
	}




	bool ChangeSkullTo2( zCViewInteractive* button, zCViewCursor* cursor, zEMouseButton mouse ) {
		button->CursorVisualIndex = skull2;
		return true;
	}

	bool ChangeSkullTo1( zCViewInteractive* button, zCViewCursor* cursor, zEMouseButton mouse ) {
		button->CursorVisualIndex = skull1;
		return true;
	}





	void InitTest() {
		static bool isInit = false;
		if( isInit )
			return;

		hand1 = zCViewCursor::GetCursor()->CreateVisual( "UnionCursorHand.tga", 1500, 1500 );
		hand2 = zCViewCursor::GetCursor()->CreateVisual( "UnionCursorFist.tga", 1500, 1500 );
		hand3 = zCViewCursor::GetCursor()->CreateVisual( "UnionCursorAction.tga", 100, 350 );
		skull1 = zCViewCursor::GetCursor()->CreateVisual( "UnionCursorSkull1.tga", 1500, 1500 );
		skull2 = zCViewCursor::GetCursor()->CreateVisual( "UnionCursorSkull2.tga", 1500, 1500 );

		zCViewInteractive* viewRotateLeft  = new zCViewInteractive();
		screen->InsertItem( viewRotateLeft );
		viewRotateLeft->SetSize( zPixelX( 100 ), zPixelY( 100 ) );
		viewRotateLeft->InsertBack( "RED" );
		viewRotateLeft->SetPos( 300, 4096 );
		viewRotateLeft->EventDown = TurnLeft;
		viewRotateLeft->CursorVisualIndex = hand3;


		zCViewInteractive* viewRotateRight = new zCViewInteractive();
		screen->InsertItem( viewRotateRight );
		viewRotateRight->SetSize( zPixelX( 100 ), zPixelY( 100 ) );
		viewRotateRight->InsertBack( "BLUE" );
		viewRotateRight->SetPos( 8192 - 300 - zPixelX( 100 ), 4096 );
		viewRotateRight->EventDown = TurnRight;
		viewRotateRight->CursorVisualIndex = hand3;


		zCViewInteractive* viewRotateMid = new zCViewInteractive();
		screen->InsertItem( viewRotateMid );
		viewRotateMid->SetSize( zPixelX( 100 ), zPixelY( 100 ) );
		viewRotateMid->InsertBack( "GREEN" );
		viewRotateMid->SetPos( 4192 - zPixelX( 100 ), 100 );
		viewRotateMid->EventDown = TurnRight;
		viewRotateMid->CursorVisualIndex = skull1;
		viewRotateMid->EventDown = ChangeSkullTo2;
		viewRotateMid->EventUp = ChangeSkullTo1;


		s_TestShape = new zCViewShaped();
		screen->InsertItem( s_TestShape );
		s_TestShape->SetSize( zPixelX( 380 ), zPixelY( 300 ) );
		s_TestShape->CreateRoundShape( 4096, 4096, 10 );
		s_TestShape->InsertBack( "LOADING" );
		s_TestShape->SetPos( 4096 - zPixelX( 190 ), 4096 - zPixelY( 150 ) );
		s_TestShape->EventDrag = ShapeDrag;
		s_TestShape->EventDown = ShapeMouseDown;
		s_TestShape->EventUp = ShapeMouseUp;
		s_TestShape->CursorVisualIndex = hand1;


		isInit = true;
	}
}
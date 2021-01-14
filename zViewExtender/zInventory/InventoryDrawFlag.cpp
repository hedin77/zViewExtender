// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  zCOLOR colorDefault = zCOLOR( 194, 184, 179 );
  zCOLOR colorActive  = GFX_WHITE;



  bool categoryOnEnter( zCViewInteractive* view, zCViewCursor* cursor ) {
    view->SetFontColor( colorActive );
    return true;
  }

  bool categoryOnLeave( zCViewInteractive* view, zCViewCursor* cursor ) {
    view->SetFontColor( colorDefault );
    //view->ClrPrintwin();
    //view->Print( 0, 0,  );
    return true;
  }

  bool categoryOnDown( zCViewInteractive* view, zCViewCursor* cursor, zEMouseButton button ) {
    return true;
  }

  bool categoryOnLoop( zCViewInteractive* view, zCViewCursor* cursor ) {
    return true;
  }



  zTInventoryDrawFlag::zTInventoryDrawFlag() {
    zCPar_Symbol* sym = parser->GetSymbol( "FONT_ScreenSmall" );
    zSTRING fontName = "FONT_OLD_10_WHITE_HI.TGA";
    //sym->GetValue( fontName, 0 );

    view = new zCViewInteractive( 0, 0, 8192, 8192 );
    view->InitFunctions( categoryOnEnter, categoryOnLeave, categoryOnDown, Null, Null, categoryOnLoop );
    view->SetFont( fontName );

    static bool x = 0;
    if( x )
      return;
    x = 1;

    cmd << AHEX32( view->font ) << endl;
    view->font->height *= 2;
    for( uint k = 0; k < 256; k++ ) {
      view->font->width[k] *= 2;
      //view->font->fontuv1[k] *= 2.0f;
      //view->font->fontuv2[k] *= 2.0f;
    }
  }

  void zTInventoryDrawFlag::InitViewSize() {
    screen->InsertItem( view );
    int w = view->FontSize( (zSTRING&)flagName );
    int h = view->FontY();
    view->SetSize( w, h );
    view->Print( 0, 0, flagName );
    screen->RemoveItem( view );
  }

  zTInventoryCategory::zTInventoryCategory() {
    zCPar_Symbol* sym = parser->GetSymbol( "FONT_Screen" );
    zSTRING fontName = "FONT_OLD_20_WHITE_HI.TGA";
    //sym->GetValue( fontName, 0 );

    view = new zCViewInteractive( 0, 0, 8192, 8192 );
    view->InitFunctions( categoryOnEnter, categoryOnLeave, categoryOnDown, Null, Null, categoryOnLoop );
    view->SetFont( fontName );

    static bool x = 0;
    if( x )
      return;
    x = 1;

    cmd << AHEX32( view->font ) << endl;
    view->font->height *= 2;
    for( uint k = 0; k < 256; k++ ) {
      view->font->width[k] *= 2;
      //view->font->fontuv1[k] *= 0.5f;
      //view->font->fontuv2[k] *= 0.5f;
    }
  }

  void zTInventoryCategory::InitViewSize() {
    screen->InsertItem( view );
    int w = view->FontSize( (zSTRING&)categoryName );
    int h = view->FontY();
    view->SetSize( w, h );
    view->Print( 0, 0, categoryName );
    screen->RemoveItem( view );
  }
}
// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static uint g_uCaret = 0;
  static uint g_uCaretTimerIndex = 0;
  static bool g_bCareVisible = false;
  static Timer g_tCaretTimer;

  uint zCViewNote::DefaultCodePage = ANSI_CODEPAGE_DEFAULT;

  void ResetCaretBlinking() {
    g_tCaretTimer[g_uCaretTimerIndex].Delete();
    g_bCareVisible = true;
  }




  bool SetClipboardText( string text ) {
    text.Replace( "\r", "\r\n" );
    uint textLength = (text.Length() + 1) * 2;
    HGLOBAL memory = GlobalAlloc( GMEM_MOVEABLE, textLength );
    memcpy( GlobalLock( memory ), text.AToW( zCViewNote::DefaultCodePage ), textLength );
    GlobalUnlock( memory );

    if( !OpenClipboard( Null ) ) {
      cmd << "cb error: " << AHEX32( GetLastError() ) << endl;
      return false;
    }

    EmptyClipboard();
    SetClipboardData( CF_UNICODETEXT, memory );
    CloseClipboard();
    return true;
  }



  bool GetClipboardText( string& text ) {
    if( !OpenClipboard( Null ) )
      return false;

    HANDLE memory = GetClipboardData( CF_UNICODETEXT );
    wchar* buffer = (wchar*)GlobalLock( memory );
    wstring data = buffer;
    text = data.WToA( zCViewNote::DefaultCodePage );
    
    text.Replace( "\r\n", "\n" );
    text.Replace( "\r", "\n" );
    
    GlobalUnlock( memory );
    CloseClipboard();
    return true;
  }



  zCViewNote::zCViewNote() : zCView() {
    Reset();
  }


  
  zCViewNote::zCViewNote( int a0, int a1, int a2, int a3, zTviewID a4 ) : zCView( a0, a1, a2, a3, a4 ) {
    Reset();
  }



  void zCViewNote::Reset() {
    Caret = 0;
    Selection = 0;
    RealText = "";
    Text = "";
    UpdateCaretX = true;
    Editable = true;
    Multiline = true;
    AutoHeight = false;
    EventKeyHandle = Null;
    CursorVisualIndex = Invalid;
    AutoSize = 0;
    DefPrintwin( 0, 0, 8192, 8192 );
  }



  inline bool IsOnSelectionrange( const int& index, const int& caret, const int& selection ) {
    int secondCaret = caret + selection;
    int min = min( caret, secondCaret );
    int max = max( caret, secondCaret );
    return index >= min && index < max;
  }



  void zCViewNote::DrawSelection( int x, int y, const zSTRING& s ) {
    if( inputList[0] != this )
      return;

    if( !Selection )
      return;
    
    zrenderer->SetAlphaBlendFunc( zTRnd_AlphaBlendFunc::zRND_ALPHA_FUNC_ADD );

    float farZ     = zCCamera::GetNearClipZ();
    int fontWidth  = 0;
    int fontHeight = font->GetFontY();
    int letterDist = font->GetLetterDistance();
    int spaceWidth = font->GetWidth( ' ' );
    zVEC2 pos1, pos2, texuv1, texuv2;

    for( uint i = 0; i < (uint)s.Length(); i++ ) {
      font->GetFontData( s[i], fontWidth, texuv1, texuv2 );

           int charWidth;
           if( byte( s[i] ) >  32 ) charWidth = fontWidth + letterDist;
      else if( byte( s[i] ) == 32 ) charWidth = spaceWidth;

      if( IsOnSelectionrange( i, Caret - g_uCaret, Selection ) ) {
        static zCTexture* ftex = zCTexture::Load( Z "BLUE.TGA", 0 );
        zrenderer->DrawTile(
          ftex,
          zVEC2( (float)( x ), (float)( y ) ),
          zVEC2( (float)( x + charWidth ), (float)( y + fontHeight ) ),
          farZ,
          0,
          1,
          fontColor
        );
      }

      x += charWidth;
    }
  }



  void zCViewNote::PrintCaret( int x, int y, const zSTRING& s ) {
    if( inputList[0] != this )
      return;

    if( g_tCaretTimer[g_uCaretTimerIndex].Await( 500 ) )
      g_bCareVisible = !g_bCareVisible;
    
    if( !g_bCareVisible )
      return;

    float farZ     = zCCamera::GetNearClipZ();
    int fontWidth  = 0;
    int fontHeight = font->GetFontY();
    int letterDist = font->GetLetterDistance();
    int spaceWidth = font->GetWidth( ' ' );
    zVEC2 pos1, pos2, texuv1, texuv2;

    for( uint i = 0; i <= (uint)s.Length(); i++ ) {
      
      if( i == Caret - g_uCaret ) {
        zCTexture* ftex = font->GetFontTexture();
        font->GetFontData( '|', fontWidth, texuv1, texuv2 );

        zrenderer->DrawTile(
          ftex,
          zVEC2( (float)(x - fontWidth / 2 + 1), (float)(y) ),
          zVEC2( (float)(x + fontWidth / 2 + 1), (float)(y + fontHeight) ),
          farZ,
          texuv1,
          texuv2,
          fontColor
          );

        CaretPosition.y = y;
        if( UpdateCaretX ) {
          CaretPosition.x = x;
          UpdateCaretX = False;
        }

        return;
      }

      if( i < (uint)s.Length() ) {
        if( byte( s[i] ) > 32 ) {
          font->GetFontData( s[i], fontWidth, texuv1, texuv2 );
          x += fontWidth + letterDist;
        }
        else if( s[i] == 32 )
          x += spaceWidth;
      }
    }
  }



  uint zCViewNote::FindCaretPositionInLine( int x, int y, zCViewText* viewtext ) {
    if( !font )
      return Invalid;
    
    int px         = VirtualToPixelX( x ) - pposx;
    int fontWidth  = 0;
    int fontHeight = font->GetFontY();
    int letterDist = font->GetLetterDistance();
    zVEC2 texuv1, texuv2;
    
    zSTRING& text    = viewtext->text;
    uint totalLength = 0;
    uint index       = 0;

    for( index = 0; index <= (uint)text.Length(); index++ ) {
      char letter = text[index];
      font->GetFontData( letter, fontWidth, texuv1, texuv2 );

      if( px + fontWidth / 2 < totalLength )
        return index;

      totalLength += letter != 32 ?
        fontWidth + letterDist :
        fontWidth;
    }

    return text.Length() + 1;
  }



  inline uint GetLeftBound( const uint& caret, const zSTRING& text, const string& symbols ) {
    uint leftCaret = caret;
    while( true ) {
      if( leftCaret == 0 )
        return leftCaret;

      for( uint i = 0; i < symbols.Length(); i++ )
        if( text[leftCaret - 1] == symbols[i] )
          return leftCaret;

      leftCaret--;
    }
  }



  inline uint GetRightBound( const uint& caret, const zSTRING& text, const string& symbols ) {
    uint rightCaret = caret;
    while( true ) {
      if( rightCaret >= text.Length() )
        return rightCaret;

      for( uint i = 0; i < symbols.Length(); i++ )
        if( text[rightCaret] == symbols[i] )
          return rightCaret;

      rightCaret++;
    }
  }



  void zCViewNote::PickWord() {
    PickRange( " \n" );
  }



  void zCViewNote::PickLine() {
    PickRange( "\n" );
  }



  uint zCViewNote::GetClicksCount() {
    static uint clicks = 0;
    static uint lastClickTime = 0;

    if( zCViewCursor::GetCursor()->IsLeftToggled() ) {
      uint time = Timer::GetTime();
      if( time - lastClickTime < 200 )
        clicks = (clicks + 1) % 3;
      else
        clicks = 0;

      lastClickTime = time;
      return clicks;
    }
    else if( zCViewCursor::GetCursor()->IsLeftPressed() )
      return clicks;

    return 0;
  }



  bool zCViewNote::SetCaretPosition( int x, int y, bool expandSelection, bool updateCaretX ) {
    if( !font )
      return false;

    if( !expandSelection && (zKeyPressed( KEY_LSHIFT ) || zKeyPressed( KEY_RSHIFT )) )
      expandSelection = true;
    
    uint clicks      = GetClicksCount();
    bool_t control   = zKeyPressed( KEY_LCONTROL ) || zKeyPressed( KEY_RCONTROL );
    uint totalLength = Text.Length();

#if ENGINE >= Engine_G2
    auto list        = textLines.next;

    while( list ) {
      totalLength -= list->data->text.Length() + 1;

      // Caret belong to the current text line check
      if( y >= screen->any( pposy + nay( list->data->posy ) ) ) {
        uint position = FindCaretPositionInLine( x, y, list->data );
        if( position != Invalid ) {
          if( !expandSelection ) {
            // Move caret and reset selection
            Caret = totalLength + position;
            Selection = 0;
          }
          else {
            // Move caret with selection expanding
            int newCaret = totalLength + position;
            Selection -= newCaret - Caret;
            Caret = newCaret;
          }
               // Auto range alignment to the word or line bounds
               if( clicks == 1 || control ) PickWord();
          else if( clicks == 2 )            PickLine();

          ResetCaretBlinking();
          if(updateCaretX)
            UpdateCaretX = True;

          return true;
        }
      }

      list = list->next;
    }
#endif

    return false;
  }



  POINT zCViewNote::GetSelectionRange() {
    POINT range;

    if( Selection < 0 ) {
      range.x = Caret + Selection;
      range.y = -Selection;
    }
    else {
      range.x = Caret;
      range.y = Selection;
    }

    return range;
  }



  zSTRING zCViewNote::GetSelectionText() {
    POINT range = GetSelectionRange();
    return ( (string&)Text ).Copy( range.x, range.y );
  }


  
  uint zCViewNote::GetCaretLine() {
    int totalLength = Text.Length();
    int linesNum    = textLines.GetNumInList() - 1;

#if ENGINE >= Engine_G2   
    auto list = textLines.next;
    while( list ) {
      zSTRING& text = list->data->text;
      totalLength -= text.Length();

      if( Caret > totalLength )
        break;

      list = list->next;
    }
#endif
    return (uint)linesNum;
  }



  void zCViewNote::UpdateAutoHeight() {
    if( AutoSize != vsizey ) {
      if( AutoHeight && !Multiline && font ) {
        zVEC2 scale = owner->GetVirtualScale();
        SetSize( vsizex, owner->any( font->GetFontY() ) );
        AutoSize = vsizey;
      }
    }
  }



  void zCViewNote::CopyText() {
    SetClipboardText( GetSelectionText() );
  }



  void zCViewNote::PasteText() {
    string subStr;
    if( GetClipboardText( subStr ) ) {
      if( !Multiline )
        subStr.Replace( "\n", " " );

      DeleteSelection();
      Text.Put( Z subStr, Caret );
      Caret += subStr.Length();
      Selection = 0;
    }
  }



  void zCViewNote::CutText() {
    if( SetClipboardText( GetSelectionText() ) )
      DeleteSelection();
  }



  void zCViewNote::DeleteSelection() {
    if( Selection == 0 )
      return;

    POINT range = GetSelectionRange();
    Text.Cut( range.x, range.y );
    Caret = range.x;
    Selection = 0;
  }



  void zCViewNote::SelectAll() {
    Caret = 0;
    Selection = Text.Length();
  }



  void zCViewNote::SetPrevCaretPosition() {
    bool shift    = zKeyPressed( KEY_LSHIFT ) || zKeyPressed( KEY_RSHIFT ) ? true : false;
    bool control  = zKeyPressed( KEY_LCONTROL ) || zKeyPressed( KEY_RCONTROL );
    uint newCaret = Caret - 1;
    if( control )
      newCaret = GetLeftBound( newCaret, Text, " \n" );

    uint moveLength = newCaret - Caret;

    if( Caret > 0 ) {
      Caret = newCaret;
      UpdateCaretX = True;

      if( shift )
        Selection -= moveLength;
      else
        Selection = 0;
    }
    else if( !shift )
      Selection = 0;
  }



  void zCViewNote::SetNextCaretPosition() {
    bool shift    = zKeyPressed( KEY_LSHIFT ) || zKeyPressed( KEY_RSHIFT ) ? true : false;
    bool control  = zKeyPressed( KEY_LCONTROL ) || zKeyPressed( KEY_RCONTROL );
    uint newCaret = Caret + 1;
    if( control )
      newCaret = GetRightBound( newCaret, Text, " \n" );

    uint moveLength = newCaret - Caret;

    if( Caret < Text.Length() ) {
      Caret = newCaret;
      UpdateCaretX = True;

      if( shift )
        Selection -= moveLength;
      else
        Selection = 0;
    }
    else if( !shift )
      Selection = 0;
  }




  void zCViewNote::PickRange( const string& symbols ) {
    uint leftCaret = Caret;
    uint rightCaret = Caret + Selection;

    if( leftCaret < rightCaret ) {
      leftCaret = GetLeftBound( leftCaret, Text, symbols );
      rightCaret = GetRightBound( rightCaret, Text, symbols );
      Caret = leftCaret;
      Selection = rightCaret - leftCaret;
    }
    else {
      leftCaret = GetRightBound( leftCaret, Text, symbols );
      rightCaret = GetLeftBound( rightCaret, Text, symbols );
      Caret = leftCaret;
      Selection = rightCaret - leftCaret;
    }
  }




  HOOK Hook_zCViewNote_PrintChars AS( &zCView::PrintChars, &zCViewNote::PrintChars );

  void zCViewNote::PrintChars( int x, int y, const zSTRING& s ) {
    THISCALL( Hook_zCViewNote_PrintChars )( x, y, s );

    zCViewNote* note = dynamic_cast<zCViewNote*>( (zCView*)this );
    if( !note || !font )
      return;
    
    zTRnd_AlphaBlendFunc oldBlendFunc = zrenderer->GetAlphaBlendFunc();
    g_uCaret -= s.Length() + 1;
    DrawSelection( x, y, s );
    PrintCaret( x, y, s );
    zrenderer->SetAlphaBlendFunc( oldBlendFunc );
  }



  HOOK Hook_zCFont_GetFontData AS( &zCFont::GetFontData, &zCFont::GetFontData_Union );

  int zCFont::GetFontData_Union( const unsigned char letter, int& width, zVEC2& uv1, zVEC2& uv2 ) {
    int OK = THISCALL( Hook_zCFont_GetFontData )( letter, width, uv1, uv2 );
    if( !OK || width == 0 )
      THISCALL( Hook_zCFont_GetFontData )( '?', width, uv1, uv2 );
    
    return True;
  }



  void zCViewNote::EditBegin() {
    OldText = Text;
    SetHandleEventTop();
    ActiveViewNote = this;
  }



  void zCViewNote::EditEnd() {
    SetEnableHandleEvent( False );
    ActiveViewNote = Null;
  }



  void zCViewNote::EditCancel() {
    Text = OldText;
    SetEnableHandleEvent( False );
    ActiveViewNote = Null;
  }



  void zCViewNote::Blit() {
    g_uCaret = Text.Length() + 1;

    if( RealText != Text ) {
      RealText = Text;
      ClrPrintwin();
      Printwin( RealText );
    }
    
    UpdateAutoHeight();
    zCView::Blit();
  }



  char& GetInputCharacter() {
    static char inputCharacted;
    return inputCharacted;
  }


  
  int zCViewNote::HandleEvent( int key ) {
    if( !font )
      return True;

    if( EventKeyHandle )
      if( EventKeyHandle( this, key ) )
        return True;
    
    bool control = zKeyPressed( KEY_LCONTROL ) || zKeyPressed( KEY_RCONTROL );
    bool shift   = zKeyPressed( KEY_LSHIFT )   || zKeyPressed( KEY_RSHIFT );

    if( key == MOUSE_BUTTONLEFT && zCViewCursor::GetCursor()->GetTopSelectedView() != this ) {
      EditEnd();
      return True;
    }

    // Copy selected text
    if( key == KEY_C && control ) {
      CopyText();
      return True;
    }
    
    // Select all
    if( key == KEY_A && control ) {
      SelectAll();
      return True;
    }
    
    // Stop interaction
    if( key == KEY_ESCAPE ) {
      EditEnd();
      return True;
    }
    
    // Select all
    if( key == KEY_RETURN && control ) {
      EditEnd();
      return True;
    }
    
    // Move caret to left
    if( key == KEY_LEFT ) {
      SetPrevCaretPosition();
      ResetCaretBlinking();
      return True;
    }
    
    // Move caret to right
    if( key == KEY_RIGHT ) {
      SetNextCaretPosition();
      ResetCaretBlinking();
      return True;
    }

    // Lazy Caret up
    if( key == KEY_UP ) {
      int fontH = font->GetFontY() - 1;
      SetCaretPosition( PixelToVirtualX( CaretPosition.x ), PixelToVirtualY( CaretPosition.y - fontH ), false, false );
      ResetCaretBlinking();
      return True;
    }
    
    // Lazy Caret down
    if( key == KEY_DOWN ) {
      int fontH = font->GetFontY() + 1;
      SetCaretPosition( PixelToVirtualX( CaretPosition.x ), PixelToVirtualY( CaretPosition.y + fontH ), false, false );
      ResetCaretBlinking();
      return True;
    }

    // EDIT MODE
    if( !Editable )
      return True;

    if( key == KEY_U && control ) {
      POINT range = GetSelectionRange();
      uint index  = range.x;
      uint length = range.y;
      
      string subStr = (A Text).Copy( index, length );
      subStr = shift ? subStr.Upper() : subStr.Lower();
      
      for( uint i = 0; i < length; i++ )
        Text[index + i] = subStr[i];

      ResetCaretBlinking();
      return True;
    }

    // Cancel interaction
    if( key == KEY_ESCAPE && shift ) {
      EditCancel();
      return True;
    }
    
    // Paste copied text
    if( key == KEY_V && zKeyPressed( KEY_LCONTROL ) ) {
      PasteText();
      return True;
    }
    
    // Cut selected text (copy and delete)
    if( key == KEY_X && zKeyPressed( KEY_LCONTROL ) ) {
      CutText();
      return True;
    }

    // Delete left letter
    if( key == KEY_BACKSPACE ) {
      if( Selection != 0 )
        DeleteSelection();
      else if( Caret > 0 )
        Text.Cut( --Caret, 1 );

      ResetCaretBlinking();
      return True;
    }

    // Delete right letter
    if( key == KEY_DELETE ) {
      if( Selection != 0 )
        DeleteSelection();
      else if( Caret < Text.Length() )
        Text.Cut( Caret, 1 );

      ResetCaretBlinking();
      return True;
    }

    // Check letter symbol
    char character = GetInputCharacter();
    if( character == 0 )
      return True;

    // Normalize eol
    if( character == '\r' )
      character = '\n';

    // Check on multiline
    if( character == '\n' )
      if( !Multiline )
        return True;

    // Print letter
    UpdateCaretX = True;
    ResetCaretBlinking();
    DeleteSelection();
    Text.Put( zSTRING( character ), Caret++ );
    return True;
  }
}
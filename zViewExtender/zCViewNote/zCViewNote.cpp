// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
  static uint g_uCaret = 0;
  static uint g_uCaretTimerIndex = 0;
  static bool g_bCareVisible = false;
  static Timer g_tCaretTimer;

  void ResetCaret() {
    g_tCaretTimer[g_uCaretTimerIndex].Delete();
    g_bCareVisible = true;
  }



  zCViewNote::zCViewNote() {
    Caret    = 0;
    RealText = "";
    Text     = "";
  }



  void zCViewNote::PrintCaret( int x, int y, const zSTRING& s ) {
    if( inputList[0] != this )
      return;

    if( g_tCaretTimer[g_uCaretTimerIndex].Await( 500 ) )
      g_bCareVisible = !g_bCareVisible;
    
    if( !g_bCareVisible )
      return;

    zREAL farZ;
    if( zCCamera::activeCam ) farZ = zCCamera::activeCam->nearClipZ + 1;
    else                      farZ = 1;

    zCViewNote* note = dynamic_cast<zCViewNote*>( (zCView*)this );
    if( !note )
      return;

    if( !font )
      return;

    int fontWidth  = 0;
    int fontHeight = font->GetFontY();
    int letterDist = font->GetLetterDistance();
    int spaceWidth = font->GetWidth( ' ' );
    zVEC2 pos1, pos2, texuv1, texuv2;


    for( uint i = 0; i <= (uint)s.Length(); i++ ) {
      if( i == Caret - g_uCaret ) {
        zCTexture* ftex = font->GetFontTexture();
        font->GetFontData( '|', fontWidth, texuv1, texuv2 );
        pos1[0] = (float)x;
        pos1[1] = (float)y;
        pos2[0] = (float)(x + fontWidth  - 1);
        pos2[1] = (float)(y + fontHeight - 1);

        zrenderer->DrawTile(
          ftex,
          zVEC2( (float)(x - fontWidth / 2 + 1), (float)(y) ),
          zVEC2( (float)(x + fontWidth / 2 + 1), (float)(y + fontHeight) ),
          farZ,
          texuv1,
          texuv2,
          fontColor
          );
      }

      if( i < s.Length() ) {
        if( byte( s[i] ) > 32 ) {
          font->GetFontData( s[i], fontWidth, texuv1, texuv2 );
          pos1[0] = (float)x;
          pos1[1] = (float)y;
          pos2[0] = (float)x + fontWidth  - 1;
          pos2[1] = (float)y + fontHeight - 1;
          x += fontWidth + letterDist;
        }
        else if( s[i] == 32 )
          x += spaceWidth;
      }
    }
  }



  HOOK Hook_zCViewNote_PrintChars AS( &zCView::PrintChars, &zCViewNote::PrintChars );

  void zCViewNote::PrintChars( int x, int y, const zSTRING& s ) {
    THISCALL( Hook_zCViewNote_PrintChars )( x, y, s );
    g_uCaret -= s.Length() + 1;
    PrintCaret( x, y, s );
  }



  void zCViewNote::EditBegin() {
    HandleEvent( True );
    SetHandleEventTop();
    DefPrintwin( 0, 0, 8192, 8192 ); // TO DO
  }



  void zCViewNote::EditEnd() {
    Text = OldText;
    HandleEvent( False );
  }



  void zCViewNote::EditCancle() {
    Text = OldText;
  }



  void zCViewNote::Blit() {
    g_uCaret = Text.Length() + 1;

    if( RealText != Text ) {
      RealText = Text;
      ClrPrintwin();
      Printwin( RealText );
    }

    zCView::Blit();
  }



  char& GetInputCharacter() {
    static char inputCharacted;
    return inputCharacted;
  }



  // 0x004D2130 unsigned char __cdecl GetCharFromDIK(int,int,int)
  CInvoke<unsigned char( *)(int, int, int)> GetCharFromDIK( ZenDef( 0x004C7590, 0x004D7060, 0x004CFB70, 0x004D2130 ) );

  int zCViewNote::HandleEvent( int key ) {
    
    if( key == KEY_ESCAPE  ) {
      SetEnableHandleEvent( False );
      return True;
    }


    if( key == KEY_LEFT ) {
      if( Caret > 0 )
        Caret--;

      ResetCaret();
      return True;
    }


    if( key == KEY_UP ) {
      Array<string> lines = (A Text).Split( "\n" );
      uint totalSize   = 0;
      uint prevLinePos = 0;
      uint prevLineLen = 0;

      for( uint i = 0; i < lines.GetNum(); i++ ) {
        string& line = lines[i];
        if( Caret <= totalSize + line.Length() ) {
          if( i > 0 ) {
            uint posInLine      = Caret - totalSize;
            Caret               = prevLineLen < posInLine ?
                                  prevLinePos + prevLineLen :
                                  prevLinePos + posInLine;
          }
          else
            Caret = 0;
        }

        prevLinePos = totalSize;
        prevLineLen = line.Length();
        totalSize  += prevLineLen + 1;
      }

#if 0
      int totalLength = 0;
      int oldPosition = Invalid;
      int oldLength   = 0;

      zCList<zCViewText>* list = textLines.GetNextInList();
      while( list ) {
        zCViewText* data = list->data;
        zSTRING& text    = data->text;
        int curPosition  = totalLength;
        int curLength    = text.Length();

        if( Caret <= curPosition + curLength ) {
          if( oldPosition != Invalid ) {
            int posInLine = Caret - curPosition + 1;
            Caret = oldPosition + posInLine;
          }
          else
            Caret = 0;

          break;
        }

        oldPosition  = curPosition;
        oldLength    = curLength;
        totalLength += curLength + 1;
        list         = list->next;
      }


      if( Caret > 0 )
        Caret--;

      ResetCaret();
#endif

      return True;
    }


    if( key == KEY_RIGHT ) {
      if( Caret < Text.Length() )
        Caret++;

      ResetCaret();
      return True;
    }


    if( key == KEY_BACKSPACE ) {
      if( Caret > 0 )
        Text.Cut( --Caret, 1 );

      ResetCaret();
      return True;
    }


    if( key == KEY_DELETE ) {
      if( Caret < Text.Length() )
        Text.Cut( Caret, 1 );

      ResetCaret();
      return True;
    }


    char character = GetInputCharacter();
    if( character == 0 )
      return True;


    if( character == '\r' )
      character = '\n';


    ResetCaret();
    Text.Put( zSTRING( character ), Caret++ );
    return True;
  }
}
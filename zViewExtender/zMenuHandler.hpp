#pragma once

namespace NAMESPACE {
  // 0x004DDC20 public: virtual void __thiscall zCMenu::Render(void)
  HOOK Ivk_Menu_Render AS( ZMENU_RENDER, &zCMenu::Render_Union, HOOK_ENABLED );

  // 0x004DBB70 public: virtual int __thiscall zCMenu::HandleEvent(int)
  HOOK Ivk_Menu_HandleEvent AS( ZMENU_HANDLEEVENT, &zCMenu::HandleEvent_Union, HOOK_ENABLED );

  // 0x004DCDE0 protected: virtual void __thiscall zCMenu::ScreenInit(void)
  HOOK Ivk_Menu_Enter AS( ZMENU_ENTER, &zCMenu::ScreenInit_Union, HOOK_ENABLED );

  // 0x004DD9E0 protected: virtual void __thiscall zCMenu::ScreenDone(void)
  HOOK Ivk_Menu_Leave AS( ZMENU_LEAVE, &zCMenu::ScreenDone_Union, HOOK_ENABLED );

  // 0x0042C450 int __cdecl Apply_Options_Video(void)
  int Hook_Apply_Options_Video();
  HOOK Ivk_Apply_Options_Video AS( ZAPPLY_OPTIONS_VIDEO, &Hook_Apply_Options_Video, HOOK_ENABLED );

  // 0x006934A0
  CInvoke<void( __thiscall* )( zCViewDialogChoice* )> Ivk_zCViewDialogChoice_BlitText( ZCVIEWDIALOGCHOICE_BLITTEXT, &zCViewDialogChoice::BlitText_Union, HOOK_ENABLED );

  // 
  CInvoke<void( __fastcall* )( zCViewDialogChoice* )> Ivk_zCViewDialog_StartSelection( ZCVIEWDIALOG_STARTSELECTION, &zCViewDialogChoice::StartSelection_Union, HOOK_ENABLED );

  // 
  CInvoke<void( __fastcall* )( zCViewDialogChoice* )> Ivk_zCViewDialog_StopSelection( ZCVIEWDIALOG_STOPSELECTION, &zCViewDialogChoice::StopSelection_Union, HOOK_ENABLED );

  // 0x0068EBA0
  CInvoke<bool_t( __thiscall* )( zCViewDialogChoice*, int )> Ivk_zCViewDialogChoice_HandleEvent ( ZCVIEWDIALOGCHOICE_HANDLEEVENT, &zCViewDialogChoice::HandleEvent_Union, HOOK_ENABLED );

  // 
  HOOK Ivk_oCItemContainer_HandleEvent AS( OCITEMCONTAINER_HANDLEEVENT, &oCItemContainer::HandleEvent_Union, HOOK_ENABLED );
  
  //
  HOOK Ivk_oCStealContainer_HandleEvent AS( OCSTEALCONTAINER_HANDLEEVENT, &oCStealContainer::HandleEvent_Union, HOOK_ENABLED );
  
  //
  HOOK Ivk_oCNpcContainer_HandleEvent AS( OCNPCCONTAINER_HANDLEEVENT, &oCNpcContainer::HandleEvent_Union, HOOK_ENABLED );
  
  //
  HOOK Ivk_oCNpcInventory_HandleEvent AS( OCNPCINVENTORY_HANDLEEVENT, &oCNpcInventory::HandleEvent_Union, HOOK_ENABLED );
  
  //
  HOOK Ivk_oCNpcInventory_OpenPassive AS( OCNPCINVENTORY_OPENPASSIVE, &oCNpcInventory::OpenPassive_Union, HOOK_ENABLED );

  //
  HOOK Ivk_oCNpcInventory_Close AS( OCNPCINVENTORY_CLOSE, &oCNpcInventory::Close_Union, HOOK_ENABLED );
  
  // 0x007076B0
  HOOK Ivk_oCItemContainer_Draw AS( OCITEMCONTAINER_DRAW, &oCItemContainer::Draw_Union, HOOK_ENABLED );
  
  // 0x00713AC0
  HOOK Ivk_oCItem_RenderItem AS( OCITEM_RENDERITEM, &oCItem::RenderItem_Union, HOOK_ENABLED );








  void oCMenuSavegame::UpdateThumbPic_Union() {

    int oldSelSlot = m_selSlot;
    if( m_mainSel >= 0 && m_mainSel < m_listItems.GetNumInList() ) {
      m_selSlotItemIndex = m_mainSel;
      m_selSlot = GetMenuItemSlotNr( m_listItems[m_mainSel] );
    }
    if( oldSelSlot != m_selSlot )
      HandleSlotChange( oldSelSlot );
  }

  void zCMenu::Render_Union() {
    zCViewCursor* cursor = zCViewCursor::GetCursor();

    if( cursor ) {
      static int oldx, oldy, x, y;
      cursor->GetPixelPos( x, y );

      if( oldx != x || oldy != y ) {

        oldx = x;
        oldy = y;
        zCView* frontView = screen->GetTopView( x, y );

        if( frontView ) {
          for( int i = 0; i < m_listItems.GetNum(); i++ ) {
            if( m_listItems[i]->m_pInnerWindow == frontView ) {

              zCMenuItem* activeItem = GetActiveItem();
              if( activeItem != m_listItems[i] ) {
                SetActiveItem( m_listItems[i] );

                if( m_listItems[i] == GetActiveItem() ) {
                  zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SELECT.WAV" );
                  if( sfx ) {
                    zsound->PlaySound( sfx, 1 );
                    sfx->Release();
                  }
                }

                oCMenuSavegame* menuSavegame = dynamic_cast<oCMenuSavegame*>( this );
                if( menuSavegame )
                  menuSavegame->UpdateThumbPic_Union();
              }
            }
          }
        }
      }
    }

    return THISCALL( Ivk_Menu_Render )();
  }

  
  


  
  int zCMenu::HandleEvent_Union( int key ) {

    if( m_mainSel != -1 ) {
      zCViewCursor* cursor = zCViewCursor::GetCursor(); 

      if( cursor ) {
        if( key == MOUSE_WHEELDOWN ) {
          zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
          if( sfx ) {
            zsound->PlaySound( sfx, 1 );
            sfx->Release();
          }

          return THISCALL( Ivk_Menu_HandleEvent )( KEY_LEFT );
        }
        else if( key == MOUSE_WHEELUP ) {
          zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
          if( sfx ) {
            zsound->PlaySound( sfx, 1 );
            sfx->Release();
          }

          return THISCALL( Ivk_Menu_HandleEvent )( KEY_RIGHT );
        }
      
        if( key == MOUSE_BUTTONLEFT ) {

          zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_CLICK.WAV" );
          if( sfx ) {
            zsound->PlaySound( sfx, 1 );
            sfx->Release();
          }

          int x, y;
          cursor->GetPixelPos( x, y );
          zCView* menuItem = screen->GetTopView( x, y );
      
          if( menuItem ) {
            for( int i = 0; i < m_listItems.GetNum(); i++ ) {
            
              if( GetActiveItem()->m_pInnerWindow == menuItem ) {
                SetActiveItem( m_listItems[i] );
                return THISCALL( Ivk_Menu_HandleEvent )( key );
              }
            }
          }
          return True;
        }

        if( key == MOUSE_BUTTONRIGHT ) {
          zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_BACK.WAV" );
          if( sfx ) {
            zsound->PlaySound( sfx, 1 );
            sfx->Release();
          }
        }
      }
    }
    return THISCALL( Ivk_Menu_HandleEvent )( key );
  }

  


  static bool_t s_lastMenuIsMain = True;

  void zCMenu::ScreenInit_Union() {
    bool_t thisMenuIsMain = name == "MENU_MAIN";

    if( thisMenuIsMain && s_lastMenuIsMain )
      zCViewCursor::GetCursor()->ShowAtCenter();
    else
      zCViewCursor::GetCursor()->Show();

    THISCALL( Ivk_Menu_Enter )();
  }





  void zCMenu::ScreenDone_Union() {
    THISCALL( Ivk_Menu_Leave )();
    zCViewCursor::GetCursor()->Hide();
    
    s_lastMenuIsMain = name == "MENU_MAIN";
  }




  
  int Hook_Apply_Options_Video() {
    int Ok = Ivk_Apply_Options_Video();
    ShowCursor( False );
    return Ok;
  }








  
  void zCView::ShowBounds( zCOLOR color ) {
    screen->Line(
      zPixelX( pposx ),
      zPixelY( pposy ),
      zPixelX( pposx + psizex ),
      zPixelY( pposy ),
      color
    );

    screen->Line(
      zPixelX( pposx + psizex ),
      zPixelY( pposy ),
      zPixelX( pposx + psizex ),
      zPixelY( pposy + psizey ),
      color
    );

    screen->Line(
      zPixelX( pposx + psizex ),
      zPixelY( pposy + psizey ),
      zPixelX( pposx ),
      zPixelY( pposy + psizey ),
      color
    );

    screen->Line(
      zPixelX( pposx ),
      zPixelY( pposy + psizey ),
      zPixelX( pposx ),
      zPixelY( pposy ),
      color
    );
  }

  zCView* zCView::GetTopView( int x, int y ) {

    zCView* child = childs.root;
    zCView* frontView = IsOnBounds( x, y ) ? this : Null;
    while( child ) {

      zCView* view = child->GetTopView( x, y );
      if( view )
        frontView = view;
      
      child = child->next;
    }

    return frontView;
  }










  static zCView* s_inventoryItemView = Null;
  static oCItemContainer* s_inventory = Null;
  static bool_t s_itemSelected = False;


  
  bool_t oCItemContainer::HandleEventBase_Union( int key ) {
    if( s_itemSelected ) {
      s_itemSelected = False;

      if( key == MOUSE_BUTTONLEFT ) {
        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_CLICK.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }
      }
      else if( key == MOUSE_BUTTONRIGHT ) {
        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_BACK.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }
      }

#if ENGINE == __G2 || ENGINE == __G2A
      if( zinput->IsBinded( GAME_UP, key ) || ( key == MOUSE_WHEELUP ) ) {

        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }

        offset -= maxSlotsCol;
        if( offset < 0 )
          offset = 0;
      }
      else if( zinput->IsBinded( GAME_DOWN, key ) || ( key == MOUSE_WHEELDOWN ) ) {

        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }

        if( offset < contents->GetNumInList() - maxSlotsCol * maxSlotsRow )
          offset += maxSlotsCol;
      }
#else
      if( zinput->IsBinded( GAME_UP, key ) || ( key == MOUSE_WHEELUP ) ) {
        offset--;
        if( offset < 0 )
          offset = 0;
      }
      else if( zinput->IsBinded( GAME_DOWN, key ) || ( key == MOUSE_WHEELDOWN ) ) {
        if( offset < contents->GetNumInList() - drawItemMax )
          offset++;
      }
#endif
    }

    return False;
  }

  bool_t oCItemContainer::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;

    return THISCALL( Ivk_oCItemContainer_HandleEvent )( key );
  }

  bool_t oCStealContainer::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;

    return THISCALL( Ivk_oCStealContainer_HandleEvent )( key );
  }
  
  bool_t oCNpcContainer::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;

    return THISCALL( Ivk_oCNpcContainer_HandleEvent )( key );
  }
  
  bool_t oCNpcInventory::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;

    return THISCALL( Ivk_oCNpcInventory_HandleEvent )( key );
  }

  void oCItemContainer::OpenPassive_Union( int a1, int a2, int a3 ) {
    if( !IsOpen() )
      zCViewCursor::GetCursor()->ShowAtCenter();

    THISCALL( Ivk_oCNpcInventory_OpenPassive )( a1, a2, a3 );
  }

  void oCItemContainer::Close_Union() {
    if( IsOpen() )
      zCViewCursor::GetCursor()->Hide();

    THISCALL( Ivk_oCNpcInventory_Close )();
  }


  


  void oCItemContainer::Draw_Union() {
    s_inventory = this;
    s_inventoryItemView = viewBack;
    THISCALL( Ivk_oCItemContainer_Draw )();
    s_inventoryItemView = Null;
    s_inventory = Null;
  }




  void oCItem::RenderItem_Union( zCWorld* world, zCViewBase* viewBase, float v3 ) {
    if( s_inventory ) {
      zCView* view = dynamic_cast<zCView*>( viewBase );
      if( view && view->owner && view->owner == s_inventoryItemView ) {
        int x, y;
        zCViewCursor::GetCursor()->GetActivePointPosition( x, y );

        if( view->IsOnBounds( screen->nax( x ), screen->nay( y ) ) ) {
          if( !s_inventory->IsActive() )
            s_inventory->Activate();

          auto* list = s_inventory->contents->next;
          int index = 0;

          while( list ) {
            oCItem* item = list->data;
            if( item == this ) {

              if( s_inventory->selectedItem != index ) {
                zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SELECT.WAV" );
                if( sfx ) {
                  zsound->PlaySound( sfx, 1 );
                  sfx->Release();
                }
              }

              s_inventory->selectedItem = index;
              s_itemSelected = True;
              break;
            }

            index++;
            list = list->next;
          }
        }
      }
    }

    THISCALL( Ivk_oCItem_RenderItem )( world, viewBase, v3 );
  }









  void zCViewDialogChoice::BlitText_Union() {
    int linesNum = ListTextLines.GetNum();
    if( linesNum && IsActivated ) {

      zCViewText2* viewText    = ListTextLines[0];
      zCViewFont&  viewFont    = viewText->ViewFont;
      zCFont*      font        = viewFont.Font;
      int          fontHeight  = zPixelY( font->GetFontY() );
      int          linesNumMax = VirtualSize.Y / fontHeight;

      int x, y;
      zCViewCursor::GetCursor()->GetActivePointPosition( x, y );
      int top = VirtualPosition.Y;

      if( zKeyToggled( MOUSE_WHEELUP ) ) {
        if( LineStart > 0 )
          ScrollUp();
      }
      else if( zKeyToggled( MOUSE_WHEELDOWN ) ) {
        if( LineStart + 1 < linesNum )
          ScrollDown();
      }

      for( int_t i = 0; i < linesNumMax; i++ ) {
        if( x >= VirtualPosition.X && x < VirtualPosition.X + VirtualSize.X )
          if( ( y - zPixelY( 5 ) ) >= ( top + fontHeight * i ) && ( y - zPixelY( 5 ) ) < ( top + fontHeight * i + fontHeight ) ) {
            Select( LineStart + i );
            break;
          }
      }
    }

    Ivk_zCViewDialogChoice_BlitText( this );
  }


  void __fastcall zCViewDialogChoice::StartSelection_Union() {
    if( !IsDone )
      zCViewCursor::GetCursor()->ShowAtCenter();

    Ivk_zCViewDialog_StartSelection( this );
  }


  void __fastcall zCViewDialogChoice::StopSelection_Union() {
    if( IsDone )
      zCViewCursor::GetCursor()->Hide();

    Ivk_zCViewDialog_StopSelection( this );
  }

  bool_t zCViewDialogChoice::HandleEvent_Union( int key ) {
    if( key == MOUSE_BUTTONLEFT ) {
      zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
      if( sfx ) {
        zsound->PlaySound( sfx, 1 );
        sfx->Release();
      }
    }

    if( key == MOUSE_WHEELUP || key == MOUSE_WHEELDOWN )
      return True;

    return Ivk_zCViewDialogChoice_HandleEvent( this, key );
  }
}
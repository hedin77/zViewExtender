
namespace NAMESPACE {


  static BOOL bUseCursorInInventory = NULL;
  
  bool needUseCursorInInventory() {
       if (bUseCursorInInventory == NULL) {
           Union.GetSysPackOption().Read(bUseCursorInInventory, "zViewExtender", "UseCursorInInventory", true);
     }
      return bUseCursorInInventory;
  }

  HOOK Ivk_oCItemContainer_HandleEvent  PATCH_IF( &oCItemContainer::HandleEvent,  &oCItemContainer::HandleEvent_Union,  needUseCursorInInventory());
  HOOK Ivk_oCStealContainer_HandleEvent PATCH_IF( &oCStealContainer::HandleEvent, &oCStealContainer::HandleEvent_Union, needUseCursorInInventory());
  HOOK Ivk_oCNpcContainer_HandleEvent   PATCH_IF( &oCNpcContainer::HandleEvent,   &oCNpcContainer::HandleEvent_Union,   needUseCursorInInventory());
  HOOK Ivk_oCNpcInventory_HandleEvent   PATCH_IF( &oCNpcInventory::HandleEvent,   &oCNpcInventory::HandleEvent_Union, needUseCursorInInventory());
  HOOK Ivk_oCNpcInventory_OpenPassive   PATCH_IF( &oCItemContainer::OpenPassive,  &oCItemContainer::OpenPassive_Union, needUseCursorInInventory());
  HOOK Ivk_oCNpcInventory_Close         PATCH_IF( &oCItemContainer::Close,        &oCItemContainer::Close_Union, needUseCursorInInventory());
  HOOK Ivk_oCItemContainer_Draw         PATCH_IF( &oCItemContainer::Draw,         &oCItemContainer::Draw_Union, needUseCursorInInventory());
  HOOK Ivk_oCItem_RenderItem            PATCH_IF( &oCItem::RenderItem,            &oCItem::RenderItem_Union, needUseCursorInInventory());


  static oCItemContainer* s_inventory = Null;
  static bool_t s_itemSelected = False;


#if 0
  // 0x004C3050 public: int __thiscall zCInput::IsBinded(unsigned short,unsigned short)
  int __fastcall zCInput_IsBinded( zCInput* _this, void* vt, DWORD g, DWORD k );
  CInvoke<int( __fastcall* )( zCInput* _this, void* vt, DWORD g, DWORD k )> Ivk_zCInput_IsBinded( 0x004C3050, &zCInput_IsBinded );
  int __fastcall zCInput_IsBinded( zCInput* _this, void* vt, DWORD g, DWORD k ) {

    if( g == GAME_UP && zMouseKeyPressed( Left ) ) {
      cmd << "ACTION 1!!" << endl;
      return True;
    }
    
    if( g == GAME_ACTION /*&& zMouseKeyPressed( Left )*/ ) {
      cmd << "ACTION 2!!" << endl;
      return True;
    }

    return Ivk_zCInput_IsBinded( _this, vt, g, k );
  }
#endif




  bool* keyevent = (bool*)ZenDef( 0x0086CCC8, 0x008B27C0, 0x008C3020, 0x008D1678 );

  int oCItemContainer::HandleEventBase_Union( int& key ) {
    //cmd << key << endl;

    /*if( key == MOUSE_WHEELUP_G1 ) {
      zCArray<WORD> keys;
      if( zinput->GetBinding( GAME_UP, keys ) )
        key = keys[0];
    }

    if( key == MOUSE_WHEELDOWN_G1 ) {
      zCArray<WORD> keys;
      if( zinput->GetBinding( GAME_DOWN, keys ) )
        key = keys[0];
    }*/




    if( s_itemSelected ) {
      s_itemSelected = False;

      if( key == MOUSE_BUTTONLEFT || key == MOUSE_KEY_LEFT_G1 ) {
        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_CLICK.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }
      }
      else if( key == MOUSE_BUTTONRIGHT || key == MOUSE_KEY_RIGHT_G1 ) {
        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_BACK.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }
      }

#if ENGINE >= Engine_G2
      if( zinput->IsBinded( GAME_UP, key ) || key == MOUSE_WHEELUP ) {

        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }

        offset -= maxSlotsCol;
        if( offset < 0 )
          offset = 0;
      }
      else if( zinput->IsBinded( GAME_DOWN, key ) || key == MOUSE_WHEELDOWN ) {

        zCSoundFX* sfx = zsound->LoadSoundFX( "MOUSE_SCROLL.WAV" );
        if( sfx ) {
          zsound->PlaySound( sfx, 1 );
          sfx->Release();
        }

        if( offset < contents->GetNumInList() - maxSlotsCol * maxSlotsRow )
          offset += maxSlotsCol;
      }
#else
      if( zinput->IsBinded( GAME_UP, key ) || key == MOUSE_WHEELUP_G1 ) {
        offset--;
        if( offset < 0 )
          offset = 0;
      }
      else if( zinput->IsBinded( GAME_DOWN, key ) || key == MOUSE_WHEELDOWN_G1 ) {
        if( offset < contents->GetNumInList() - drawItemMax )
          offset++;
      }
#endif
    }

    return False;
  }



  int oCItemContainer::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;

    //cmd << tab << key << endl;
    return THISCALL( Ivk_oCItemContainer_HandleEvent )( key );
  }



  int oCStealContainer::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;
    
    //cmd << tab << key << endl;
    return THISCALL( Ivk_oCStealContainer_HandleEvent )( key );
  }
  


  int oCNpcContainer::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;
    
    //cmd << tab << key << endl;
    return THISCALL( Ivk_oCNpcContainer_HandleEvent )( key );
  }
  


  int oCNpcInventory::HandleEvent_Union( int key ) {
    if( oCItemContainer::HandleEventBase_Union( key ) )
      return True;
    
    //cmd << tab << key << endl;
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
    THISCALL( Ivk_oCItemContainer_Draw )();
    s_inventory = Null;
  }




  void oCItem::RenderItem_Union( zCWorld* world, zCViewBase* viewBase, float v3 ) {
    if( s_inventory ) {
      zCView* view = dynamic_cast<zCView*>( viewBase );
      if( view && view->owner /*&& view->owner == s_inventoryItemView*/ ) {
        int x, y;
        zCViewCursor::GetCursor()->GetActivePointPosition( x, y );

        if( view->IsOnBounds( screen->nax( x ), screen->nay( y ) ) ) {
          if( !s_inventory->IsActive() ) {
            s_inventory->Activate();
            //s_inventory->
          }

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
              
#if ENGINE < Engine_G2
              s_inventory->SetCategoryOnRightContainer();
#endif
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
}
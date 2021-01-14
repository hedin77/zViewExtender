// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {

  






  







  zCInventory::zCInventory() {
    Init();
  }



  void zCInventory::Init() {
    selectedCategoty = 0;
    selectedFlag     = 0;

    background = new zCView( 0,    0, 8192, 8192 );
    vseparator = new zCView( 0,    0, 8192, 8192 );
    category   = new zCView( 0,    0, 8192, 8192 );
    column     = new zCView( 4160, 0, 4032, 8192 );
    hseparator = new zCView( 0,    0, 8192, 8192 );

    background->InsertItem( vseparator );
    background->InsertItem( category );
    background->InsertItem( column );
    background->InsertItem( hseparator );

    background->InsertBack( "Inventory_Back" );
    vseparator->InsertBack( "Inventory_Separator_V" );
    hseparator->InsertBack( "Inventory_Separator_H" );
  }



  void zCInventory::Open() {
    screen->InsertItem( background );
    
    int vertivalOffset = 0;
    int sx, sy;

    for( uint i = 0; i < categories.GetNum(); i++ ) {
      auto& cat = categories[i];
      category->InsertItem( cat.view );
      cat.view->GetSize( sx, sy );
      cat.view->SetPos( 4160 - sx, vertivalOffset );
      vertivalOffset += sy;

      for( uint j = 0; j < cat.flags.GetNum(); j++ ) {
        auto& flag = cat.flags[j];
        category->InsertItem( flag.view );
        flag.view->GetSize( sx, sy );
        flag.view->SetPos( 4160 - sx, vertivalOffset );
        vertivalOffset += sy;
      }

      vertivalOffset += background->anx( 50 );
    }
    
  }



  void zCInventory::Close() {
    screen->RemoveItem( background );
  }



  void zCInventory::DrawCategories() {

  }



  zTInventoryCategory& zCInventory::GetOrCreateCategoty( string categoryName ) {
    for( uint i = 0; i < categories.GetNum(); i++ )
      if( categories[i].categoryName == categoryName )
        return categories[i];
    
    zTInventoryCategory& category = categories.Create();
    category.categoryName = categoryName;
    category.InitViewSize();
    return category;
  }



  zTInventoryDrawFlag& zCInventory::GetOrCreateFlag( string categoryName, string flagName ) {
    zTInventoryCategory& category = GetOrCreateCategoty( categoryName );
    for( uint i = 0; i < category.flags.GetNum(); i++ )
      if( category.flags[i].flagName == flagName )
        return category.flags[i];

    zTInventoryDrawFlag& drawFlag = category.flags.Create();
    drawFlag.flagName = flagName;
    drawFlag.InitViewSize();
    return drawFlag;
  }



  void zCInventory::AddCategoty( string catName, string flagName, uint mainFlag, uint flag ) {
    zTInventoryDrawFlag& drawFlag = GetOrCreateFlag( catName, flagName );
    drawFlag.mainFlag = mainFlag;
    drawFlag.flag     = flag;
  }





  //HOOK Ivk_zCTextureExchange_GetRGBAAtPtr AS( &zCTextureExchange::GetRGBAAtPtr, &zCTextureExchange::GetRGBAAtPtr_Union );

  //zVEC4 zCTextureExchange::GetRGBAAtPtr_Union( zBYTE* ptr, zTTexPalette* palPtr, const zTRnd_TextureFormat texFormat ) {
  //  if( texFormat == zRND_TEX_FORMAT_RGBA_8888 ) {
  //    //cmd << "----" << endl;

  //    zQWORD a = *(zQWORD*)ptr;
  //    return zVEC4( ((a >> 16) & 31) << 8,
  //                  ((a >> 8) & 31) << 8,
  //                  ((a >> 0) & 31) << 8,
  //                  ((a >> 24) & 31) << 8 );
  //  }
  //  
  //  return THISCALL( Ivk_zCTextureExchange_GetRGBAAtPtr )(ptr, palPtr, texFormat);
  //}





  void InventoryTest() {
    if( zKeyToggled( KEY_U ) ) {
      zTItemPanel* panel = new zTItemPanel();
      panel->InsertBack( "black" );
      screen->InsertItem( panel );
      panel->SetSize( zPixelX( 800 ), zPixelY( 180 ) );
      panel->SetPos( 100, 100 );
      cmd << 1234 << endl;
      //panel->Init();
    }

    static zCInventory inventory;
    if( zKeyToggled( KEY_T ) ) {
      player->inventory2.UnpackAllItems();

      inventory.AddCategoty( "Оружия", "Одноручные", 0, ITM_FLAG_DAG | ITM_FLAG_SWD | ITM_FLAG_AXE );
      inventory.AddCategoty( "Оружия", "Двуручные",  0, ITM_FLAG_2HD_SWD | ITM_FLAG_2HD_AXE );
      inventory.AddCategoty( "Оружия", "Луки",       0, ITM_FLAG_BOW );
      inventory.AddCategoty( "Оружия", "Арбалеты",   0, ITM_FLAG_CROSSBOW );
      inventory.AddCategoty( "Оружия", "Боеприпасы", ITM_CAT_MUN, 0 );

      inventory.AddCategoty( "Защита", "Доспехи", ITM_CAT_ARMOR, 0 );
      inventory.AddCategoty( "Защита", "Щиты",    0, ITM_FLAG_SHIELD );
      inventory.AddCategoty( "Защита", "Амулеты", 0, ITM_FLAG_AMULET );
      inventory.AddCategoty( "Защита", "Пояса",   0, ITM_FLAG_BELT );
      inventory.AddCategoty( "Защита", "Кольца",  0, ITM_FLAG_RING );

      inventory.Open();
    }
  }
}
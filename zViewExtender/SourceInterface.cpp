#include "UnionAfx.h"


#ifdef __G1
#define NAMESPACE Gothic_I_Classic
//#include "zViewExtender.h"
#endif
#ifdef __G1A
#define NAMESPACE Gothic_I_Addon
#include "zViewExtender.h"
#endif
#ifdef __G2
#define NAMESPACE Gothic_II_Classic
#include "zViewExtender.h"
#endif
#ifdef __G2A
#define NAMESPACE Gothic_II_Addon
#include "zViewExtender.h"
#endif


#ifdef __G1
#define HOOK_ENABLED IVK_DISABLED //Union.GetEngineVersion() == Engine_G1 ? IVK_AUTO : IVK_DISABLED
#define ENGINE __G1
#define NAMESPACE Gothic_I_Classic
#define ZCVIEW_BLIT          0x006FC8C0
#define ZCRND_D3D_BEGINFRAME 0x00716F60
#define ZCRND_D3D_ENDFRAME   0x00717150
//#include "zViewExtender.hpp"
#define ZMENU_RENDER                   0x004D0DA0
#define ZMENU_HANDLEEVENT              0x004CEE10
#define ZMENU_ENTER                    0x004CFF30
#define ZMENU_LEAVE                    0x004D0B60
#define ZAPPLY_OPTIONS_VIDEO           0x0042A310
#define ZCVIEWDIALOGCHOICE_BLITTEXT    0x007536D0
#define ZCVIEWDIALOG_STARTSELECTION    0x00758E00
#define ZCVIEWDIALOG_STOPSELECTION     0x00758E40
#define ZCVIEWDIALOGCHOICE_HANDLEEVENT 0x00758A60
#define OCITEMCONTAINER_HANDLEEVENT    0x00669DD0
#define OCSTEALCONTAINER_HANDLEEVENT   0x0066A730
#define OCNPCCONTAINER_HANDLEEVENT     0x0066ACD0
#define OCNPCINVENTORY_HANDLEEVENT     0x0066E390
#define OCNPCINVENTORY_OPENPASSIVE     0x00668050
#define OCNPCINVENTORY_CLOSE           0x00668C10
#define OCITEMCONTAINER_DRAW           0x00667660
#define OCITEM_RENDERITEM              0x00672F70
//#include "zMenuHandler.hpp"
#endif

#ifdef __G1A
#define HOOK_ENABLED (Union.GetEngineVersion() == Engine_G1A ? IVK_AUTO : IVK_DISABLED)
#define ENGINE __G1A
#define NAMESPACE Gothic_I_Addon
#define ZCVIEW_BLIT          0x00736C80
#define ZCRND_D3D_BEGINFRAME 0x00752ED0
#define ZCRND_D3D_ENDFRAME   0x007530D0
#include "zViewExtender.hpp"
#define ZMENU_RENDER                   0x004E14E0
#define ZMENU_HANDLEEVENT              0x004DF6A0
#define ZMENU_ENTER                    0x004E08E0
#define ZMENU_LEAVE                    0x004E1280
#define ZAPPLY_OPTIONS_VIDEO           0x0042D130
#define ZCVIEWDIALOGCHOICE_BLITTEXT    0x00795B80
#define ZCVIEWDIALOG_STARTSELECTION    0x0079B3C0
#define ZCVIEWDIALOG_STOPSELECTION     0x0079B400
#define ZCVIEWDIALOGCHOICE_HANDLEEVENT 0x0079AFC0
#include "zMenuHandler.hpp"
#endif

#ifdef __G2
#define HOOK_ENABLED (Union.GetEngineVersion() == Engine_G2 ? IVK_AUTO : IVK_DISABLED)
#define ENGINE __G2
#define NAMESPACE Gothic_II_Classic
#define ZCVIEW_BLIT          0x007468C0
#define ZCRND_D3D_BEGINFRAME 0x00762780
#define ZCRND_D3D_ENDFRAME   0x00762980
#include "zViewExtender.hpp"
#define ZMENU_RENDER                   0x004DB270
#define ZMENU_HANDLEEVENT              0x004D94D0
#define ZMENU_ENTER                    0x004DA740
#define ZMENU_LEAVE                    0x004DB030
#define ZAPPLY_OPTIONS_VIDEO           0x0042C130
#define ZCVIEWDIALOGCHOICE_BLITTEXT    0x007A0440
#define ZCVIEWDIALOG_STARTSELECTION    0x007A5CD0
#define ZCVIEWDIALOG_STOPSELECTION     0x007A5D10
#define ZCVIEWDIALOGCHOICE_HANDLEEVENT 0x007A5910
#include "zMenuHandler.hpp"
#endif

#ifdef __G2A
#define HOOK_ENABLED (Union.GetEngineVersion() == Engine_G2A ? IVK_AUTO : IVK_DISABLED)
#define ENGINE               __G2A
#define NAMESPACE            Gothic_II_Addon
#define ZCVIEW_BLIT          0x007A63C0
#define ZCRND_D3D_BEGINFRAME 0x0064DD20
#define ZCRND_D3D_ENDFRAME   0x0064DF20
#include "zViewExtender.hpp"
#define ZMENU_RENDER                   0x004DDC20
#define ZMENU_HANDLEEVENT              0x004DBB70
#define ZMENU_ENTER                    0x004DCDE0
#define ZMENU_LEAVE                    0x004DD9E0
#define ZAPPLY_OPTIONS_VIDEO           0x0042C450
#define ZCVIEWDIALOGCHOICE_BLITTEXT    0x006901F0
#define ZCVIEWDIALOG_STARTSELECTION    0x0068EF70
#define ZCVIEWDIALOG_STOPSELECTION     0x0068EFB0
#define ZCVIEWDIALOGCHOICE_HANDLEEVENT 0x0068EBA0
#define OCITEMCONTAINER_HANDLEEVENT    0x0070A640
#define OCSTEALCONTAINER_HANDLEEVENT   0x0070B090
#define OCNPCCONTAINER_HANDLEEVENT     0x0070B6F0
#define OCNPCINVENTORY_HANDLEEVENT     0x0070E040
#define OCNPCINVENTORY_OPENPASSIVE     0x007086D0
#define OCNPCINVENTORY_CLOSE           0x00708F30
#define OCITEMCONTAINER_DRAW           0x007076B0
#define OCITEM_RENDERITEM              0x00713AC0
#include "zMenuHandler.hpp"
#endif
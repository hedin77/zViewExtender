// Interface queue
#include "UnionAfx.h"
#define VIEWAPI __declspec(dllexport)


// Check executed engine with current source code
#define CHECK_THIS_ENGINE (Union.GetEngineVersion() == ENGINE)
#define GOTHIC_ENGINE NAMESPACE
#define Engine_G1  1
#define Engine_G1A 2
#define Engine_G2  3
#define Engine_G2A 4



// Include headers
#ifdef __G1
#define NAMESPACE Gothic_I_Classic
#define ENGINE Engine_G1
#include "Headers.h"
#endif
#ifdef __G1A
#define NAMESPACE Gothic_I_Addon
#define ENGINE Engine_G1A
#include "Headers.h"
#endif
#ifdef __G2
#define NAMESPACE Gothic_II_Classic
#define ENGINE Engine_G2
#include "Headers.h"
#endif
#ifdef __G2A
#define NAMESPACE Gothic_II_Addon
#define ENGINE Engine_G2A
#include "Headers.h"
#endif



// Include source files (with same as header parameters)
#ifdef __G1
#define NAMESPACE Gothic_I_Classic
#define ENGINE Engine_G1
#include "Sources.h"
#endif
#ifdef __G1A
#define NAMESPACE Gothic_I_Addon
#define ENGINE Engine_G1A
#include "Sources.h"
#endif
#ifdef __G2
#define NAMESPACE Gothic_II_Classic
#define ENGINE Engine_G2
#include "Sources.h"
#endif
#ifdef __G2A
#define NAMESPACE Gothic_II_Addon
#define ENGINE Engine_G2A
#include "Sources.h"
#endif
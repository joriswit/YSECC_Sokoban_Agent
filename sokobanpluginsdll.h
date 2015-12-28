#ifndef SOKOBANPLUGIN_H
#define SOKOBANPLUGIN_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

///////////////////////////////////////////////////////////////////////////////
// Constants
///////////////////////////////////////////////////////////////////////////////

// Return codes (for all functions returning an integer)
#define SOKOBAN_PLUGIN_RESULT_OK 0
#define SOKOBAN_PLUGIN_RESULT_SUCCESS SOKOBAN_PLUGIN_RESULT_OK
#define SOKOBAN_PLUGIN_RESULT_CONSTRAINTSVIOLATION 1
#define SOKOBAN_PLUGIN_RESULT_INVALID 2
#define SOKOBAN_PLUGIN_RESULT_IMPOSSIBLE 3
#define SOKOBAN_PLUGIN_RESULT_GIVEUP 4
#define SOKOBAN_PLUGIN_RESULT_GAMETOOLONG 5
#define SOKOBAN_PLUGIN_RESULT_INVALIDCONFIGURATION 6
#define SOKOBAN_PLUGIN_RESULT_FAILURE 7
#define SOKOBAN_PLUGIN_RESULT_TIMEOUT 8
#define SOKOBAN_PLUGIN_RESULT_TERMINATED_BY_USER 9

// Flags for PluginStatus.uiFlags
#define SOKOBAN_PLUGIN_FLAG_UNSUCCESSFUL 0
#define SOKOBAN_PLUGIN_FLAG_SOLUTION 1
#define SOKOBAN_PLUGIN_FLAG_MOVES 2
#define SOKOBAN_PLUGIN_FLAG_PUSHES 4
#define SOKOBAN_PLUGIN_FLAG_BOX_LINES 8
#define SOKOBAN_PLUGIN_FLAG_BOX_CHANGES 16
#define SOKOBAN_PLUGIN_FLAG_PUSHING_SESSIONS 32
#define SOKOBAN_PLUGIN_FLAG_SECONDARY_MOVES 64
#define SOKOBAN_PLUGIN_FLAG_SECONDARY_PUSHES 128
#define SOKOBAN_PLUGIN_FLAG_SECONDARY_BOX_LINES 256
#define SOKOBAN_PLUGIN_FLAG_SECONDARY_BOX_CHANGES 512
#define SOKOBAN_PLUGIN_FLAG_SECONDARY_PUSHING_SESSIONS 1024

///////////////////////////////////////////////////////////////////////////////
// Structures
///////////////////////////////////////////////////////////////////////////////

// PluginStatus (SolveEx() and Optimize() parameter)
struct PluginStatus
{
    unsigned int uiSize;
    unsigned int uiFlags;
    __int64 i64MovesGenerated;
    __int64 i64PushesGenerated;
    __int64 i64StatesGenerated;
    char szStatusText[256];
    unsigned int uiPluginTimeMS;      // [out] plugin running time, not necessarily identical to clock time
};

///////////////////////////////////////////////////////////////////////////////
// Function Prototypes
///////////////////////////////////////////////////////////////////////////////

typedef void __stdcall CONFIGURE
(
    HWND hwndParent                   // [in]  parent window handle
);

typedef void __stdcall GETCONSTRAINTS
(
    unsigned int* puiMaxWidth,        // [out] maximum level width
    unsigned int* puiMaxHeight,       // [out] maxumum level height
    unsigned int* puiMaxBoxes         // [out] maximum objects
);

typedef void __stdcall GETPLUGINNAME
(
    char* pcString,                   // [out] plugin name string
    unsigned int uiStringBufferSize   // [in]  maximum length of the plugin name, including string terminator
);

typedef int __stdcall  ISSUPPORTEDOPTIMIZATION
(
    unsigned int uiOptimization       // [in]  optimization mode to test
);

typedef int __stdcall PLUGINCALLBACK(void); // a non-zero return-value terminates the plugin

typedef int __stdcall OPTIMIZE
(
    unsigned int uiWidth,             // [in]  level width
    unsigned int uiHeight,            // [in]  level height
    char* pcBoard,                    // [in]  the level as a vector of characters
    char* pcMoves,                    // [in,out] the sets of moves, e.g., solutions, separated by a null-terminator and ending with an extra null-terminator
    unsigned int uiMovesBufferSize,   // [in]  maximum length of the sets of moves, including string terminators
    struct PluginStatus* psStatus,    // [in,out] plugin status information
    PLUGINCALLBACK* pc                // [in]  address of callback function
);

typedef void __stdcall SHOWABOUT
(
    HWND hwndParent                   // [in]  parent window handle
);

typedef int __stdcall TERMINATE(void);

typedef int __stdcall SOLVEEX
(
    unsigned int uiWidth,             // [in]  level width
    unsigned int uiHeight,            // [in]  level height
    char* pcBoard,                    // [in]  the level as a vector of characters
    char* pcSolution,                 // [out] solution
    unsigned int uiSolutionBufferSize,// [in]  maximum length of the solution string, including string terminator
    struct PluginStatus* psStatus,    // [out] plugin status information
    PLUGINCALLBACK* pc                // [in]  address of callback function
);

///////////////////////////////////////////////////////////////////////////////
// Backwards Compatibility Section
///////////////////////////////////////////////////////////////////////////////

#ifdef SOKOBAN_PLUGIN_ENABLE_BACKWARDS_COMPATIBILITY

#define SolverStatus PluginStatus
#define GETSOLVERNAME GETPLUGINNAME
#define SOLVERCALLBACK PLUGINCALLBACK

#define SOLVER_SUCCESS SOKOBAN_PLUGIN_RESULT_SUCCESS
#define SOLVER_OK SOKOBAN_PLUGIN_RESULT_OK
#define SOLVER_CONSTRAINTSNOTMET SOKOBAN_PLUGIN_RESULT_CONSTRAINTSVIOLATION
#define SOLVER_INVALID SOKOBAN_PLUGIN_RESULT_INVALID
#define SOLVER_IMPOSSIBLE SOKOBAN_PLUGIN_RESULT_IMPOSSIBLE
#define SOLVER_GIVEUP SOKOBAN_PLUGIN_RESULT_GIVEUP
#define SOLVER_SOLUTIONTOOLONG SOKOBAN_PLUGIN_RESULT_GAMETOOLONG
#define SOLVER_INVALIDCONFIGURATION SOKOBAN_PLUGIN_RESULT_INVALIDCONFIGURATION
#define SOLVER_FAILURE SOKOBAN_PLUGIN_RESULT_FAILURE
#define SOLVER_TIMEOUT SOKOBAN_PLUGIN_RESULT_TIMEOUT
#define SOLVER_TERMINATED_BY_USER SOKOBAN_PLUGIN_RESULT_TERMINATED_BY_USER

#define SOKOBAN_SOLVER_STATUS_FLAG_UNSOLVED SOKOBAN_PLUGIN_FLAG_UNSUCCESSFUL
#define SOKOBAN_SOLVER_STATUS_FLAG_SOLUTION SOKOBAN_PLUGIN_FLAG_SOLUTION
#define SOKOBAN_SOLVER_STATUS_FLAG_OPTIMAL_SOLUTION_MOVES SOKOBAN_PLUGIN_FLAG_MOVES
#define SOKOBAN_SOLVER_STATUS_FLAG_OPTIMAL_SOLUTION_PUSHES SOKOBAN_PLUGIN_FLAG_PUSHES
#define SOKOBAN_SOLVER_STATUS_FLAG_OPTIMAL_SOLUTION_LINEAR_PUSHES SOKOBAN_PLUGIN_FLAG_BOX_LINES
#define SOKOBAN_SOLVER_STATUS_FLAG_OPTIMAL_SOLUTION_BOX_CHANGES SOKOBAN_PLUGIN_FLAG_BOX_CHANGES
#define SOKOBAN_SOLVER_STATUS_FLAG_OPTIMAL_SOLUTION_PUSHING_SESSIONS SOKOBAN_PLUGIN_FLAG_PUSHING_SESSIONS
#define SOKOBAN_SOLVER_STATUS_FLAG_SECONDARY_OPTIMAL_SOLUTION_MOVES SOKOBAN_PLUGIN_FLAG_SECONDARY_MOVES
#define SOKOBAN_SOLVER_STATUS_FLAG_SECONDARY_OPTIMAL_SOLUTION_PUSHES SOKOBAN_PLUGIN_FLAG_SECONDARY_PUSHES
#define SOKOBAN_SOLVER_STATUS_FLAG_SECONDARY_OPTIMAL_SOLUTION_LINEAR_PUSHES SOKOBAN_PLUGIN_FLAG_SECONDARY_BOX_LINES
#define SOKOBAN_SOLVER_STATUS_FLAG_SECONDARY_OPTIMAL_SOLUTION_BOX_CHANGES SOKOBAN_PLUGIN_FLAG_SECONDARY_BOX_CHANGES
#define SOKOBAN_SOLVER_STATUS_FLAG_SECONDARY_OPTIMAL_SOLUTION_PUSHING_SESSIONS SOKOBAN_PLUGIN_FLAG_SECONDARY_PUSHING_SESSIONS

#define uiInternalCalculatingTimeMS uiPluginTimeMS

typedef int __stdcall SOLVE
(
    unsigned int uiWidth,             // [in]  level width
    unsigned int uiHeight,            // [in]  level height
    unsigned char** ppucBoard,        // [in]  the level as a vector of characters
    char* pcSolution,                 // [out] solution
    unsigned int uiSolutionBufferSize // [in]  maximum length of the solution string, including string terminator
);

#endif // ifdef SOKOBAN_PLUGIN_ENABLE_BACKWARDS_COMPATIBILITY

#ifdef __cplusplus
}
#endif

#endif // ifndef SOKOBANPLUGIN_H

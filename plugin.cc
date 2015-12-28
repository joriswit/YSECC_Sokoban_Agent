// A sokoban agent written in C++
//
// Developers
// ----------
//
// - Christian Magnerfelt
// - Yoann Nicod
// - Christopher Norman
// - Sebastian Östlund
// - Etienne-Nicholas Nyaiesh
//
// Converted to plugin by Joris Wit

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "tile.h"
#include "maze.h"
#include "node.h"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstring>
#pragma pack(push, 1)
#include "sokobanpluginsdll.h"
#pragma pack(pop)
#include "ysecc_sokoban_agent_export.h"

std::string path_to_string(std::vector<Maze::position> const& path);

void reverse_best_first_search(Maze const& maze,
                               std::vector<Node> const& root_nodes,
                               std::vector<Node>& steps);

void forward_best_first_search(Maze const& maze,
                               std::vector<Node> const& root_nodes,
                               std::vector<Node>& steps);

void bidirectional_search(Maze const& maze,
                          std::vector<Node> const& initial_nodes,
                          std::vector<Node> const& terminal_nodes,
                          std::vector<Node>& steps);
void threaded_bidirectional_search(Maze const& maze,
                          std::vector<Node> const& initial_nodes,
                          std::vector<Node> const& terminal_nodes,
                          std::vector<Node>& steps);

extern "C" {

int YSECC_SOKOBAN_AGENT_EXPORT __stdcall SolveEx(unsigned int width,
                                                 unsigned int height,
                                                 char* board,
                                                 char* pcSolution,
                                                 unsigned int uiSolutionBufferSize,
                                                 struct PluginStatus *psStatus,
                                                 PLUGINCALLBACK *pc) {
    std::vector<Maze::position> player_ending_pos;
    bool player_in_maze = false; // Safety measure
    
    Maze maze;
    
    for (unsigned y = 0; y < height; y++) {
        maze.add_row(width);
        for (unsigned x = 0; x < width; x++) {
            if (board[y * width + x] == '@') {
                maze.set_player_starting_pos(Maze::position(x, y));
                player_in_maze = true;
                player_ending_pos.push_back(Maze::position(x, y));
            }
            if (board[y * width + x] == '+') {
                maze.set_player_starting_pos(Maze::position(x, y));
                player_in_maze = true;
                maze(x, y).setType(Tile::Dest);
                maze.add_crates_ending_pos(Maze::position(x, y));
            }
            if (board[y * width + x] == '.') {
                maze(x, y).setType(Tile::Dest);
                maze.add_crates_ending_pos(Maze::position(x, y));
            }
            if (board[y * width + x] == '*') {
                maze(x, y).setType(Tile::Dest);
                maze.add_crates_ending_pos(Maze::position(x, y));
                maze.add_crates_starting_pos(Maze::position(x, y));
            }
            if (board[y * width + x] == '$') {
                maze.add_crates_starting_pos(Maze::position(x, y));
                player_ending_pos.push_back(Maze::position(x, y));
            }
            if (board[y * width + x] == '#') {
                maze(x, y).setType(Tile::Obstacle);
            }
            if (board[y * width + x] == ' ') {
                maze(x, y).setType(Tile::Floor);
                player_ending_pos.push_back(Maze::position(x, y));
            }
        }
    }
    maze.calculate_displacement_mapping();
    
    if (!player_in_maze) {
        // Assume it never happens
    }
    
//    if (crates_starting_pos.size() != dest_pos.size()) {
//        // Assume it never happens
//    }
    
    /* Create root nodes for the reverse problem */
    std::vector<Node> terminal_nodes;
    for (Maze::position const& pos : player_ending_pos) {
            terminal_nodes.push_back(Node(maze, pos, maze.get_crates_ending_pos()));
    }
    
    /* Create root nodes for the forward problem */
    std::vector<Node> initial_nodes;
    initial_nodes.push_back(Node(maze, maze.get_player_starting_pos(),
                                       maze.get_crates_starting_pos()));
    
    /* Solve the problem */
    std::vector<Node> steps;
    //reverse_best_first_search(maze, terminal_nodes, steps);
	//forward_best_first_search(maze, initial_nodes, steps);
	//threaded_bidirectional_search(maze, initial_nodes, terminal_nodes, steps);
	bidirectional_search(maze, initial_nodes, terminal_nodes, steps);
    
    /* We now have a sequence of nodes that leads to the goal 
     * Translate to player movements.
     */
    
    std::string solution_string = "";
    Maze::position current_pos = maze.get_player_starting_pos();
    for (Node const& node : steps) {
//        std::cout << node << std::endl;
//        std::cout << node.get_player_pos() << " to " << current_pos << std::endl;
        std::vector<Maze::position> path;
        auto const& dest = node.get_player_starting_pos();
        auto const& crates = node.get_crates_starting_pos();
        maze.find_path(dest, current_pos, crates, path);
        
        solution_string += path_to_string(path);
        current_pos = node.get_player_ending_pos();
        solution_string += node.get_path();
    }

    psStatus->uiFlags = SOKOBAN_PLUGIN_FLAG_SOLUTION;

    if(!strcpy_s(pcSolution, uiSolutionBufferSize, solution_string.data())) {
        return SOKOBAN_PLUGIN_RESULT_SUCCESS;
    } else {
        return SOKOBAN_PLUGIN_RESULT_GAMETOOLONG;
    }
}

void YSECC_SOKOBAN_AGENT_EXPORT __stdcall GetConstraints(unsigned int* puiMaxWidth, unsigned int* puiMaxHeight, unsigned int* puiMaxBoxes) {
    *puiMaxWidth = 0;
    *puiMaxHeight = 0;
    *puiMaxBoxes = 0;
}

void YSECC_SOKOBAN_AGENT_EXPORT __stdcall GetPluginName(char* pcString, unsigned int uiStringBufferSize) {
    strcpy_s(pcString, uiStringBufferSize, "YSECC Sokoban agent");
}

void YSECC_SOKOBAN_AGENT_EXPORT __stdcall Configure(HWND hwndParent) {
    MessageBox(hwndParent, "No configuration options", "Sokoban agent", MB_OK);
}

void YSECC_SOKOBAN_AGENT_EXPORT __stdcall ShowAbout(HWND hwndParent) {
    MessageBox(hwndParent, "Sokoban\n=======\n\nA sokoban agent written in C++\n\nDevelopers\n----------\n\n- Christian Magnerfelt\n- Yoann Nicod\n- Christopher Norman\n- Sebastian Östlund\n- Etienne-Nicholas Nyaiesh", "Sokoban agent", MB_OK);
}

}
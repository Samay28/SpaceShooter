#pragma once
#include <unordered_map>
#include <string>
#include <Data/EnemyDefinition.h>

//for identification of gameplay consequence 
enum class NarrativeAction
{
    None,
    AcceptAlliance,
    RejectAlliance
};

//an edge in our graph
// It represents one choice leading from the current
// dialogue node to another dialogue node.
struct DialogueChoice
{
    std::string text; // The text of the choice
    int NextNodeID;
    NarrativeAction action; // The action that this choice represents
};

//a vertex in our graph, which represents a point in the narrative
struct DialogueNode
{
    int id;
    std::string speaker; // The character speaking
    std::string dialogue; // The dialogue text

    //all possible outgoing edges from this node
    std::vector<DialogueChoice> choices;
};


class GameBranchController
{
    //will store enemy types killed
    //setup a graph with nodes to decide where game will go next based on player choice 
    //the decision will be made and thrown away in an event based system that concerned class will listen to and act on the decision
    //player inputs on choices will be collected here as well and will be used to make the decision

public:

    GameBranchController();

    // a map storing the number of enemies killed for each type
    std::unordered_map<EnemyType, int> enemyKillCount;
    void recordEnemyKill(const EnemyType & enemyType);
    void resetEnemyKillCount(); // Resets the enemy kill count for all enemy types

    EnemyType& findImposter();

    //setup graph with nodes to decide where game will go next based on player choice
    void AddNode(const DialogueNode& node);
    const DialogueNode* GetNode(int nodeId) const;
    bool HasNode(int id) const;

    void BuildGraph(); // This function will build the dialogue graph with nodes and choices
    void SelectChoice(size_t choiceIndex); // This function will handle the selection of a choice and transition to the next node

    const DialogueNode* GetCurrentNode() const;// This function will retrieve the current dialogue node based on m_currentNodeID

private:
    //Node ID -> DialogueNode
    // This map will store all the dialogue nodes in the game, allowing for quick access to any node by its ID.
    std::unordered_map<int, DialogueNode> m_nodes;

    int m_startingNodeID = 0; // The ID of the starting node in the dialogue graph
    int m_currentNodeID = 0; // The ID of the current node in the dialogue graph
};


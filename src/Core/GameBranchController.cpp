#include "GameBranchController.h"
#include <iostream>

using namespace std;
GameBranchController::GameBranchController()
{
    BuildGraph();
}
void GameBranchController::recordEnemyKill(const EnemyType& enemyType)
{
    enemyKillCount[enemyType]++;
    //debug print the enemy type and the number of kills

}

void GameBranchController::resetEnemyKillCount()
{
    enemyKillCount.clear();
}

//returns the enemy type with the highest kill count
EnemyType& GameBranchController::findImposter()
{
    //just traverse the map and find the enemy type with the highest kill count
    EnemyType mostKilledEnemy;
    int maxKills = 0;
    for (const auto& pair : enemyKillCount)
    {
        if (pair.second > maxKills)
        {
            maxKills = pair.second;
            mostKilledEnemy = pair.first;
        }
    }
    cout << "Most killed enemy type: " << static_cast<int>(mostKilledEnemy) << endl;
    return mostKilledEnemy;
}

void GameBranchController::AddNode(const DialogueNode& node)
{
    //add the node to the map
    m_nodes[node.id] = node;

    
}

const DialogueNode* GameBranchController::GetNode(int nodeId) const
{
    return m_nodes.find(nodeId) != m_nodes.end() ? &m_nodes.at(nodeId) : nullptr;
}

bool GameBranchController::HasNode(int id) const
{
    return m_nodes.find(id) != m_nodes.end();
}

void GameBranchController::BuildGraph()
{
    m_nodes.clear(); //just in case we are rebuilding the graph, clear any existing nodes

    //------------Node 0 ------------

    DialogueNode transmission{};
    transmission.id = 0;
    transmission.speaker = "Unkown Pilot";
    transmission.dialogue = "I can help you destroy this fleet, trust me, I have seen enough destruction of my own kind. Accept my Offer!";

    transmission.choices =
    {
        {
            "Accept the Alliance",
            1,
            NarrativeAction::AcceptAlliance
        },
        {
            "Reject the Alliance",
            2,
            NarrativeAction::RejectAlliance
        }

    };

    AddNode(transmission);

    //------------Node 1 ------------
    DialogueNode allianceAccepted{};
    allianceAccepted.id = 1;
    allianceAccepted.speaker = "Unkown Pilot";
    allianceAccepted.dialogue = "Good. Stay close and I am sending reinforcements to your location. We will destroy this fleet together!";

    allianceAccepted.choices =
    {
        {
            "Let's roll!",
            3,
            NarrativeAction::None
        }
    };
    AddNode(allianceAccepted);

    //------------Node 2 ------------
    DialogueNode allianceRejected{};
    allianceRejected.id = 2;
    allianceRejected.speaker = "Unkown Pilot";
    allianceRejected.dialogue = "Then you leave me no choice. Incoming reinforcements.";

    allianceRejected.choices =
    {
        {
            "Prepare for battle!",
            4,
            NarrativeAction::None
        }
    };
    AddNode(allianceRejected);

    // ---------------- NODE 3 ----------------
    // Gameplay continuation after accepting.

    DialogueNode allyContinuation{};

    allyContinuation.id = 3;
    allyContinuation.speaker = "Unknown Pilot";
    allyContinuation.dialogue =
        "Target the ships ahead. I'll take the ones behind.";

    AddNode(allyContinuation);


    // ---------------- NODE 4 ----------------
    // Gameplay continuation after rejecting.

    DialogueNode enemyContinuation{};

    enemyContinuation.id = 4;
    enemyContinuation.speaker = "Unknown Pilot";
    enemyContinuation.dialogue =
        "You should have listened. Here they come.";

    AddNode(enemyContinuation);


    // Set the initial dialogue node.
    m_startingNodeID = 0;
    m_currentNodeID = m_startingNodeID;

}

void GameBranchController::SelectChoice(size_t choiceIndex)
{
    const DialogueNode* currNode = GetNode(m_currentNodeID);

    if (!currNode)
        return;

    //to check if selected choice exists
    if (choiceIndex >= currNode->choices.size())
        return;

    const DialogueChoice& choice = currNode->choices[choiceIndex];
    m_currentNodeID = choice.NextNodeID;
}

const DialogueNode* GameBranchController::GetCurrentNode() const
{
    // Search for the node using the current node ID.
    auto it = m_nodes.find(m_currentNodeID);

    // Return nullptr if the current node doesn't exist.
    if (it == m_nodes.end())
        return nullptr;

    // Return a pointer to the node stored inside the map.
    return &it->second;
}
#pragma once
#include "CvorLokacije.h"
#include "CvorPuta.h"
#include "CvorRaskrsnice.h"
#include <vector>

constexpr double INF = std::numeric_limits<double>::infinity();
struct NodeInfo
{
    int id;
    double udaljenost;
    int prethodni;
    bool posjecen;

    NodeInfo() : id(-1), udaljenost(INF), prethodni(-1), posjecen(false) {}
    NodeInfo(int _id) : id(_id), udaljenost(INF), prethodni(-1), posjecen(false) {}
    NodeInfo(int _id, double _distance, int _previousNode, bool _visited)
        : id(_id), udaljenost(_distance), prethodni(_previousNode), posjecen(_visited) {}
};

class Graf
{
public:
    void dodajCvor(int id, std::unique_ptr<Cvor> node)
    {
        cvorovi[id] = std::move(node);
    }
    void serijalizuj(const std::string &ime) const
    {
        std::ofstream file(ime);
        if (!file)
        {
            std::cerr << "Ne mogu otvoriti fajl za serijalizaciju: " << ime << std::endl;
            return;
        }

        for (const auto &[id, node] : cvorovi)
        {
            switch (node->getTipCvora())
            {
            case Cvor::TipCvora::LOKACIJA:
                file << "Node: Lokacija, ID: " << node->getID() << std::endl;
                break;
            case Cvor::TipCvora::PUT:
            {
                const CvorPuta *put = dynamic_cast<const CvorPuta *>(node.get());
                file << "Node: Put, ID: " << put->getID()
                     << ", Duzina: " << put->getLength()
                     << ", MaksBrzina: " << put->getMaxSpeed()
                     << ", MaksVozila: " << put->getMaxVehicles() << std::endl;
            }
            break;
            case Cvor::TipCvora::RASKRSNICA:
            {
                const CvorRaskrsnice *raskrsnica = dynamic_cast<const CvorRaskrsnice *>(node.get());
                file << "Node: Raskrsnica, ID: " << raskrsnica->getID()
                     << ", MaksVozila: " << raskrsnica->getMaxVehicles() << std::endl;
            }
            break;
            }
        }

        file.close();
    }

    void deserijalizuj(const std::string &ime)
    {
        std::ifstream file(ime);
        if (!file)
        {
            std::cerr << "Ne mogu otvoriti fajl za deserijalizaciju: " << ime << std::endl;
            return;
        }

        clear();

        size_t numNodes;
        file >> numNodes;

        for (size_t i = 0; i < numNodes; ++i)
        {
            int id, tipInt;
            file >> id >> tipInt;
            Cvor::TipCvora tip = static_cast<Cvor::TipCvora>(tipInt);

            if (tip == Cvor::TipCvora::RASKRSNICA)
            {
                int maxVozila;
                file >> maxVozila;
                cvorovi[id] = std::make_unique<CvorRaskrsnice>(id, maxVozila);
            }
            else if (tip == Cvor::TipCvora::PUT)
            {
                int odLokacije, doLokacije, maxVozila;
                double duzina, maxBrzina;
                file >> odLokacije >> doLokacije >> duzina >> maxBrzina >> maxVozila;
                cvorovi[id] = std::make_unique<CvorPuta>(id, odLokacije, doLokacije, duzina, maxBrzina, maxVozila);
            }
        }

        file.close();
    }

    void addIntersectionConnection(int odLokacijeID, int doLokacijeID, int raskrsnicaID, double udaljenost, double ogranicenje, bool canMove)
    {
        if (cvorovi.find(odLokacijeID) != cvorovi.end() && cvorovi.find(doLokacijeID) != cvorovi.end())
        {
            if (auto intersection = dynamic_cast<CvorRaskrsnice *>(cvorovi[raskrsnicaID].get()))
            {
                intersection->addConnectionWithConstraint(odLokacijeID, doLokacijeID, udaljenost, ogranicenje, canMove);
            }
        }
    }
    const std::unordered_map<int, std::unique_ptr<Cvor>> &getNodes() const
    {
        return cvorovi;
    }

    void addEdge(int fromNodeID, int odLokacije, double udaljenost, double ogranicenje, bool canMove)
    {
        if (auto intersection = dynamic_cast<CvorRaskrsnice *>(cvorovi[fromNodeID].get()))
        {
            intersection->addConnection(fromNodeID, odLokacije, udaljenost, ogranicenje, canMove);
        }
    }

    std::vector<int> dijkstra(int startNodeID, int endNodeID, bool minimizeDistance, bool minimizeTime)
    {
        std::unordered_map<int, NodeInfo> nodeInfo;
        for (const auto &[id, node] : cvorovi)
        {
            nodeInfo[id] = NodeInfo(id);
        }

        nodeInfo[startNodeID].udaljenost = 0;

        auto compare = [&](const std::pair<double, int> &a, const std::pair<double, int> &b)
        {
            if (minimizeDistance)
                return a.first > b.first;
            else if (minimizeTime)
            {
                auto nodeA = dynamic_cast<CvorPuta *>(cvorovi[a.second].get());
                auto nodeB = dynamic_cast<CvorPuta *>(cvorovi[b.second].get());
                return a.first / nodeA->getMaxSpeed() > b.first / nodeB->getMaxSpeed();
            }
            else
                return false;
        };

        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, decltype(compare)> queue(compare);
        queue.push({0, startNodeID});

        while (!queue.empty())
        {
            double currentDistance = queue.top().first;
            int currentNodeID = queue.top().second;
            queue.pop();

            if (currentNodeID == endNodeID)
            {
                break;
            }

            nodeInfo[currentNodeID].posjecen = true;

            auto &currentNode = cvorovi[currentNodeID];
            if (currentNode->getTipCvora() == Cvor::TipCvora::RASKRSNICA)
            {
                CvorRaskrsnice *intersection = dynamic_cast<CvorRaskrsnice *>(currentNode.get());
                for (const auto &[connection, info] : intersection->getConnections())
                {
                    int neighborNodeID = connection.second;
                    double distanceToNeighbor = std::get<0>(info);
                    double ogranicenje = std::get<1>(info);
                    bool canMove = std::get<2>(info);

                    if (canMove && !nodeInfo[neighborNodeID].posjecen)
                    {
                        // Relaxation step
                        double newDistance = currentDistance + (minimizeDistance ? distanceToNeighbor : distanceToNeighbor / ogranicenje);
                        if (newDistance < nodeInfo[neighborNodeID].udaljenost)
                        {
                            nodeInfo[neighborNodeID].udaljenost = newDistance;
                            nodeInfo[neighborNodeID].prethodni = currentNodeID;
                            queue.push({newDistance, neighborNodeID});
                        }
                    }
                }
            }
        }

        std::vector<int> shortestPath;
        int currentNode = endNodeID;
        while (currentNode != -1)
        {
            shortestPath.push_back(currentNode);
            currentNode = nodeInfo[currentNode].prethodni;
        }
        std::reverse(shortestPath.begin(), shortestPath.end());

        return shortestPath;
    }

private:
    std::unordered_map<int, std::unique_ptr<Cvor>> cvorovi;
    void clear()
    {
        cvorovi.clear();
    }
};
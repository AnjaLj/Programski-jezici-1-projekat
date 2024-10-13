#pragma once
#include "Cvor.h"
#include "CvorLokacije.h"
#include "CvorRaskrsnice.h"
#include "CvorPuta.h"
#include "Graf.h"
#include <sstream>

class Vozilo
{
public:
    Vozilo(int id, int startNodeID, int endNodeID, double maxSpeed)
        : id(id), startNodeID(startNodeID), endNodeID(endNodeID), maxSpeed(maxSpeed), currentNodeID(startNodeID) {}

    int getID() const { return id; }
    int getCurrentNodeID() const { return currentNodeID; }
    void setCurrentNodeID(int nodeID) { currentNodeID = nodeID; }
    int getStartNodeID() const { return startNodeID; }
    int getEndNodeID() const { return endNodeID; }
    double getMaxSpeed() const { return maxSpeed; }
    std::string route(const std::vector<int> &shortestPath) const
    {
        std::stringstream ss;
        for (size_t i = 0; i < shortestPath.size(); ++i)
        {
            if (i == 0)
            {
                ss << "Od " << shortestPath[i];
            }
            else if (i == shortestPath.size() - 1)
            {
                ss << " Do " << shortestPath[i];
            }
            else
            {
                ss << "->" << shortestPath[i];
            }
        }
        return ss.str();
    }

    double distance(const Graf &network, const std::vector<int> &shortestPath) const
    {
        double totalDistance = 0.0;
        for (size_t i = 1; i < shortestPath.size(); ++i)
        {
            int fromNodeID = shortestPath[i - 1];
            int odLokacije = shortestPath[i];
            auto currentNode = network.getNodes().at(fromNodeID).get();

            if (currentNode->getTipCvora() == Cvor::TipCvora::PUT)
            {
                CvorPuta *road = dynamic_cast<CvorPuta *>(currentNode);
                totalDistance += road->getLength();
            }
        }
        return totalDistance;
    }

    double time(const Graf &network, const std::vector<int> &shortestPath) const
    {
        double totalTime = 0.0;
        for (size_t i = 1; i < shortestPath.size(); ++i)
        {
            int fromNodeID = shortestPath[i - 1];
            int odLokacije = shortestPath[i];
            auto currentNode = network.getNodes().at(fromNodeID).get();

            if (currentNode->getTipCvora() == Cvor::TipCvora::PUT)
            {
                CvorPuta *road = dynamic_cast<CvorPuta *>(currentNode);
                double roadLength = road->getLength();
                double maxSpeed = road->getMaxSpeed();
                double slowdownFactor = road->getSlowdownFactor();
                double effectiveSpeed = std::min(getMaxSpeed(), maxSpeed / slowdownFactor);
                totalTime += roadLength / effectiveSpeed;
            }
        }
        return totalTime;
    }

    void setPutanja(const std::vector<int> &novaPutanja)
    {
        this->putanja = novaPutanja;
    }
    int getNextNodeId()
    {
        if (trenutniIndexPutanje + 1 < putanja.size())
        {
            return putanja[++trenutniIndexPutanje];
        }
        return -1; // Ili neki drugi mehanizam za signalizaciju kraja putanje
    }

    int getCurrentNodeIDpath() const
    {
        if (!putanja.empty() && trenutniIndexPutanje < putanja.size())
        {
            return putanja[trenutniIndexPutanje];
        }
        return -1; // Ili neki drugi mehanizam za signalizaciju greške
    }

private:
    int id;
    int startNodeID;
    int endNodeID;
    double maxSpeed;
    int currentNodeID;
    std::vector<int> putanja;
    int trenutniIndexPutanje = 0;
};
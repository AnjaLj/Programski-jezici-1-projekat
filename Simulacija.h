#pragma once
#include "Graf.h"
#include "Vozilo.h"
#include <iostream>
#include <memory>
#include <unordered_map>
#include <queue>
#include <vector>
#include <limits>
#include <algorithm>
#include <random>
#include <string>
#include <sstream>
#include <fstream>
class Simulacija
{
public:
    Simulacija(Graf &network) : network(network) {}

    void addVehicle(const Vozilo &vehicle)
    {
        vozila.push_back(vehicle);
    }

    void simulate()
    {
        for (auto &vehicle : vozila)
        {
            moveVehicle(vehicle);
            ispisiPozicijuVozila(vehicle);
        }
    }

    const std::vector<Vozilo> &getVehicles() const
    {
        return vozila;
    }

    bool isSimulationEnded() const
    {
        for (const auto &vehicle : vozila)
        {
            if (vehicle.getCurrentNodeID() != vehicle.getEndNodeID())
            {
                return false;
            }
        }
        return true;
    }

    double calculateAverageTravelTime(int startNodeID, int endNodeID) const
    {
        double totalTravelTime = 0.0;
        int vehicleCount = 0;

        for (const auto &vehicle : vozila)
        {
            if (vehicle.getStartNodeID() == startNodeID && vehicle.getEndNodeID() == endNodeID)
            {
                totalTravelTime += calculateTravelTime(vehicle);
                vehicleCount++;
            }
        }

        if (vehicleCount > 0)
        {
            return totalTravelTime / vehicleCount;
        }
        else
        {
            return INF;
        }
    }

    void ispisiPozicijuVozila(const Vozilo &vehicle) const
    {
        std::cout << "Vozilo ID: " << vehicle.getID() << " se nalazi na cvoru: " << vehicle.getCurrentNodeID() << std::endl;
    }

private:
    void moveVehicle(Vozilo &vehicle)
    {
        int nextNodeID = vehicle.getNextNodeId(); // Dobijanje sledećeg čvora na putanji
        if (nextNodeID != -1)
        {                                         // Ako postoji sledeći čvor
            vehicle.setCurrentNodeID(nextNodeID); // Ažuriranje trenutne pozicije vozila
        }
        else
        {
            // Logika za obradu situacije kada vozilo dođe do kraja putanje
            std::cout << "Vozilo ID: " << vehicle.getID() << " je stiglo na odredište." << std::endl;
        }
    }

    double calculateTravelTime(const Vozilo &vehicle) const
    {
        int startNodeID = vehicle.getStartNodeID();
        int endNodeID = vehicle.getEndNodeID();

        std::vector<int> shortestPath = network.dijkstra(startNodeID, endNodeID, true, false);

        double travelTime = 0.0;
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
                double effectiveSpeed = std::min(vehicle.getMaxSpeed(), maxSpeed / slowdownFactor);
                travelTime += roadLength / effectiveSpeed;
            }
        }

        return travelTime;
    }

    Graf &network;
    std::vector<Vozilo> vozila;
};
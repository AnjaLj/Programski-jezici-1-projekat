#pragma once
#include "Cvor.h"
#include <iostream>
#include <unordered_map>
#include <map>
#include <tuple>

struct PairHash
{
    template <typename T1, typename T2>
    std::size_t operator()(const std::pair<T1, T2> &pair) const
    {
        return std::hash<T1>{}(pair.first) ^ std::hash<T2>{}(pair.second);
    }
};

class CvorRaskrsnice : public Cvor
{
public:
    CvorRaskrsnice(int id, int maxVozila) : Cvor(TipCvora::RASKRSNICA, id), maxVozila(maxVozila) {}

    void addConnection(int odLokacije, int doLokacije, double duzina, double brzina, bool moze)
    {
        connections[std::make_pair(odLokacije, doLokacije)] = std::make_tuple(duzina, brzina, moze);
    }

    void addConnectionWithConstraint(int odLokacije, int doLokacije, double duzina, double brzina, bool moze)
    {
        if (moze)
        {
            addConnection(odLokacije, doLokacije, duzina, brzina, moze);
        }
    }

    const std::unordered_map<std::pair<int, int>, std::tuple<double, double, bool>, PairHash> &getConnections() const
    {
        return connections;
    }

    int getMaxVehicles() const
    {
        return maxVozila;
    }

    void ispisiPodatke() const override
    {
        std::cout << "Cvor raksrsnice ID: " << id << std::endl
                  << "Maksimalan broj vozila: " << maxVozila << std::endl
                  << "Povezanosti na raksrsnici:" << std::endl;
        for (const auto &[cvorovi, informacije] : connections)
        {
            std::cout << "Od lokacije " << cvorovi.first << " do lokacije " << cvorovi.second
                      << ", Udaljenost: " << std::get<0>(informacije)
                      << ", Brzina: " << std::get<1>(informacije)
                      << ", Da li moze preci?: " << (std::get<2>(informacije) ? "Da" : "Ne") << std::endl;
        }
    }

private:
    int maxVozila;
    std::unordered_map<std::pair<int, int>, std::tuple<double, double, bool>, PairHash> connections;
};

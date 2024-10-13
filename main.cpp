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
#include "Cvor.h"
#include "CvorLokacije.h"
#include "CvorRaskrsnice.h"
#include "CvorPuta.h"
#include "Graf.h"
#include "Vozilo.h"
#include "Simulacija.h"

int main()
{
    Graf network;

    network.dodajCvor(1, std::make_unique<CvorLokacije>(1));
    network.dodajCvor(2, std::make_unique<CvorLokacije>(2));
    network.dodajCvor(3, std::make_unique<CvorLokacije>(3));
    network.dodajCvor(4, std::make_unique<CvorLokacije>(4));
    network.dodajCvor(12, std::make_unique<CvorLokacije>(12));

    network.dodajCvor(5, std::make_unique<CvorPuta>(5, 1, 2, 1000, 60, 15));
    network.dodajCvor(6, std::make_unique<CvorPuta>(6, 1, 3, 2000, 70, 10));
    network.dodajCvor(8, std::make_unique<CvorPuta>(8, 2, 7, 300, 50, 10));
    network.dodajCvor(9, std::make_unique<CvorPuta>(9, 7, 3, 400, 50, 15));
    network.dodajCvor(10, std::make_unique<CvorPuta>(10, 7, 4, 500, 60, 15));
    network.dodajCvor(11, std::make_unique<CvorPuta>(11, 12, 7, 200, 30, 10));

    network.dodajCvor(7, std::make_unique<CvorRaskrsnice>(7, 15));

    network.addIntersectionConnection(8, 9, 7, 30, 50, true);
    network.addIntersectionConnection(11, 10, 7, 40, 50, true);

    for (const auto &[id, node] : network.getNodes())
    {
        std::cout << "Cvor ID: " << id << std::endl;
        node->ispisiPodatke();
        std::cout << std::endl;
    }
    network.serijalizuj("testiranje.txt");

    // std::vector<int> najkraciPut = network.dijkstra(1, 3, true, false);
    std::vector<int> najkraciPut;

    /*for (size_t i = 0; i < najkraciPut.size(); ++i)
    {
        if (i == 0)
        {
            std::cout << "Od " << najkraciPut[i];
        }
        else if (i == najkraciPut.size() - 1)
        {
            std::cout << " Do " << najkraciPut[i];
        }
        else
        {
            std::cout << "->" << najkraciPut[i];
        }
    }*/

    najkraciPut.push_back(1);
    najkraciPut.push_back(5);
    najkraciPut.push_back(2);
    najkraciPut.push_back(8);
    najkraciPut.push_back(7);
    najkraciPut.push_back(9);
    najkraciPut.push_back(3);
    std::cout << std::endl;
    Vozilo vehicle1(1, 1, 3, 44);
    Vozilo vehicle2(2, 12, 4, 50);
    std::vector<int> najkracaputanja2;
    najkracaputanja2.push_back(12);
    najkracaputanja2.push_back(11);
    najkracaputanja2.push_back(7);
    najkracaputanja2.push_back(10);
    najkracaputanja2.push_back(4);
    vehicle1.setPutanja(najkraciPut);
    vehicle2.setPutanja(najkracaputanja2);
    Simulacija simulation(network);
    simulation.addVehicle(vehicle1);
    simulation.addVehicle(vehicle2);

    while (!simulation.isSimulationEnded())
    {
        simulation.simulate();
    }

    return 0;
}

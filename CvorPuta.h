#pragma once
#include "Cvor.h"
#include <iostream>

class CvorPuta : public Cvor
{
public:
    CvorPuta(int id, int odLokacije, int doLokacije, double duzina, double maxBrzina, int maxVozila)
        : Cvor(TipCvora::PUT, id), odLokacije(odLokacije), doLokacije(doLokacije), duzina(duzina), maxBrzina(maxBrzina), maxVozila(maxVozila), currentVehicles(0) {}

    double getLength() const
    {
        return duzina;
    }

    double getMaxSpeed() const
    {
        return maxBrzina;
    }

    int getMaxVehicles() const
    {
        return maxVozila;
    }

    void incrementVehicleCount()
    {
        currentVehicles++;
    }

    void decrementVehicleCount()
    {
        currentVehicles--;
    }

    double getSlowdownFactor() const
    {
        return 1.0 + (currentVehicles / static_cast<double>(maxVozila));
    }

    void ispisiPodatke() const override
    {
        std::cout << "Put ID: " << id << std::endl
                  << "Od lokacije ID: " << odLokacije << std::endl
                  << "Do lokacije ID: " << doLokacije << std::endl
                  << "Duzina: " << duzina << "\n"
                  << "Maksimalna brzina: " << maxBrzina << " m/s\n"
                  << "Maksimalan broj vozila: " << maxVozila << std::endl;
    }

    int getodLokacije() const
    {
        return odLokacije;
    }

    int getdoLokacije() const
    {
        return doLokacije;
    }

private:
    double duzina;
    double maxBrzina;
    int maxVozila;
    int currentVehicles;
    int odLokacije;
    int doLokacije;
};
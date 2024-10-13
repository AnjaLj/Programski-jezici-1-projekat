#pragma once
#include "Cvor.h"

class CvorLokacije : public Cvor
{
public:
    CvorLokacije(int id) : Cvor(TipCvora::LOKACIJA, id) {}

    void ispisiPodatke() const override
    {
        std::cout << "Cvor lokacije ID: " << id << std::endl;
    }
};
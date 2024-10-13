#pragma once
#include <string>
#include <vector>

class Cvor
{
public:
    enum class TipCvora
    {
        PUT,
        RASKRSNICA,
        LOKACIJA
    };

    Cvor(TipCvora tip, int id) : tip(tip), id(id) {}
    virtual ~Cvor() = default;

    TipCvora getTipCvora() const
    {
        return tip;
    }

    int getID() const
    {
        return id;
    }

    virtual void ispisiPodatke() const {}

protected:
    TipCvora tip;
    int id;
};

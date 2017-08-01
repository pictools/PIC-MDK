#ifndef PICMDK_CONSTANTS_H
#define PICMDK_CONSTANTS_H


namespace picmdk {

// Mathematical and physical constants in CGS
namespace constants
{
    const double lightVelocity = 29979245800.0;
    const double electronCharge = -4.80320427e-10;
    const double electronMass = 9.10938215e-28;
    const double protonMass = 1.672622964e-24;
    const double pi = 3.14159265358;
    const double reducedPlanck = 1.0545716818e-27;
    const double electronVolt = 1.60217656535e-12;
    const double eV = electronVolt;
    const double MeV = 1e6 * eV;
}

} // namespace picmdk

#endif

#include <cmath>

#include <stdexcept>
#include <iostream>

#include "avalanche.h"
#include "products.h"
#include "critical_field.h"

using namespace std;

double avalanche_generation_rate(double electron_density, double electron_temperature, double effective_charge, double electric_field, double magnetic_field, module_struct modules) {

	double agr, avalanche_threshold_field;

	double coulomb_log = calculate_coulomb_log(electron_density, electron_temperature);
	double critical_field = calculate_critical_field(electron_density, electron_temperature);
	double runaway_collision_time = calculate_runaway_collision_time(electron_density, electron_temperature);
	
	if ( (modules.avalanche_formula.compare("rosenbluth_putvinski")) || (modules.avalanche_formula.compare("rosenbluth_putvinski_with_threshold")) ){

		agr = (electric_field/critical_field - 1) / (2*runaway_collision_time*coulomb_log);

		//! threshold field: avalanche_thresold_field
		if (modules.avalanche_formula.compare("rosenbluth_putvinski_with_threshold"))
			avalanche_threshold_field = calculate_avalanche_threshold_field(electron_density, electron_temperature,	effective_charge, critical_field, magnetic_field);
		else avalanche_threshold_field = 0;

		if (electric_field < avalanche_threshold_field) agr = 0;

		//! Avalanche rate must be non-negative
		if(isnan(agr) || (agr<0)) agr = 0;
	}
	
	return agr;
	
}

double calculate_avalanche_threshold_field(double electron_density, double electron_temperature, double effective_charge, double critical_field, double magnetic_field){

	//! \a REQ-3: electron collision time
	/*!
	\f[
		\tau = 4 \pi \epsilon_0^2 \cdot \frac{m_\mathrm{e}^2 \cdot c^3 }{e^4} \cdot \frac{1}{n_\mathrm{e} \ln \Lambda}
	\f]
	*/

	double runaway_collision_time = calculate_runaway_collision_time(electron_density, electron_temperature);
	double synchrotron_loss_time = calculate_synchrotron_loss_time(magnetic_field);
	double norm_synchrotron_loss_time = synchrotron_loss_time/runaway_collision_time;

	/*!
	\f[
		E_\mathrm{a} \approx E_0 = 1 + \frac{ \frac {1+Z}{sqrt(\tau_\mathrm{rad}}}{(\frac{1}{8} + \frac{(Z+1)^2}{\tau_\mathrm{rad}})^{1/6}}
	\f]
	*/
	return critical_field*(1.0 + (1.0+effective_charge) / sqrt(norm_synchrotron_loss_time) / pow( 1.0/8.0 + (1.0+effective_charge) * (1.0+effective_charge) / norm_synchrotron_loss_time , 1.0/6.0));

}

double calculate_toroidicity_avalanche(double inv_asp_ratio, double electric_field, double electron_density, double electron_temperature, double rho_tor_norm){

    double inv_asp_ratio_coord = inv_asp_ratio*rho_tor_norm;
    
	double critical_field = calculate_critical_field(electron_density, electron_temperature);
	double rel_electric_field = electric_field/critical_field;
	
	double toroidicity_avalanche = pow(1.0-inv_asp_ratio_coord,2)/(ITM_PI*sqrt(inv_asp_ratio_coord*rel_electric_field));
	
	if (rel_electric_field < 1){
		toroidicity_avalanche = 1;
	}
	return toroidicity_avalanche;

}


/*
double calculate_flow_Ap(double p, double E, double Z){
	return 2*E/(Z+1)*p*p/sqrt(p*p+1);
}

double calculate_flow_costheta(double p, double E, double Z){
	double Ap = calculate_flow_Ap(p,E,Z);
	return 1/(tanh(Ap))- 1/Ap;
}

double calculate_flow_velocity(double electron_density, double electron_temperature, double effective_charge, double electric_field, double magnetic_field){

	double critical_field = calculate_critical_field(electron_density, electron_temperature);
	double E = electric_field/critical_field;
	double Z = effective_charge;
	double p = 0; // temporary definition

	double costheta = calculate_flow_costheta(p,E,Z);
	return 0;
}
*/

#ifndef RUNAFLUID_H_
#define RUNAFLUID_H_

void fire(ItmNs::Itm::coreprof &coreprof, ItmNs::Itm::coreimpur &coreimpur,
		ItmNs::Itm::equilibrium &equilibrium, ItmNs::Itm::distribution &distribution_in, ItmNs::Itm::distribution &distribution_out, double &timestep, int &runafluid_switch, double &critical_fraction, int &runaway_warning, int &not_suitable_warning, int &critical_fraction_warning, ItmNs::Itm::temporary &runaway_rates);
		
int init_rates(ItmNs::Itm::temporary & runaway_rates, int N_rates, int N_rho);


#endif /* RUNAFLUID_H_ */
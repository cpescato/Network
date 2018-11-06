#include "network.h"
#include "random.h"
#include <iostream>
//#include <algorithm>
//inclusion traitement d'erreur?
#include <tclap/CmdLine.h>


void Network::resize(const size_t& n) {
	if (n>0) {
		values.resize(n);
		for (size_t i(0);i<n;++i) 
			values[i]=RNG.normal(); 
	}
	//else std::cerr<<"Impossible de resize"<<'\n';
	else throw TCLAP::ArgException("Impossible de resize, taille <= 0");
	
}

bool Network::add_link(const size_t& a, const size_t& b) {	
	if (a==b) return false;
	if ((size()<=a) or (size()<=b)) {
		std::cerr<<"Size_t pas acceptables"<<'\n';
		return false;
	}
	if (links.find(a)!=links.end()) {
		std::multimap<size_t,size_t>::iterator it1(links.find(a));
		
		if ((it1->first ==a) and (it1->second==b)) {
			std::cerr<<"Le lien existe deja!"<<'\n';
			return false;
		}
	}
	
	if (links.find(b)!=links.end()) {
		std::multimap<size_t,size_t>::iterator it2(links.find(b));
		if ((it2->first ==b) and (it2->second==a)) {
			std::cerr<<"Le lien existe deja!"<<'\n';
			return false;
		}
	}	
			
		links.insert(std::make_pair(a,b));
		links.insert(std::make_pair(b,a));
		return true;
	
}


size_t Network::random_connect(const double& mean_deg) {
	
	size_t deg(0), nbreliens(0);
	std::multimap<size_t,size_t>::iterator it(links.begin());
	
	links.clear();
		
	std::vector<size_t> indices;
	for (size_t i(0);i<size();++i)	
		indices.push_back(i);
		
	for (size_t j(0);j<size();++j) {
		//RandomNumbers rn;
		deg=RNG.poisson(mean_deg);
		if (deg>(size()-1)) deg = (size()-1);
		
		RNG.shuffle(indices);
		size_t compteur(0);
		//while (compteur<deg) {
		for(size_t n(0);n<indices.size();++n) {
			//RNG.shuffle(indices);
			if (compteur<deg) {
				if (add_link(j, indices[n])) { //[0]
					++compteur;
					++nbreliens;
				}
			}
		}
	}
	
	return nbreliens; //links.size()/2 
}

size_t Network::set_values(const std::vector<double>& vec) {
	for (size_t i(0);i<vec.size();++i)
		values[i]=vec[i];
	return vec.size();
}

size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	return links.count(_n);
}

double Network::value(const size_t &_n) const {
	return values[_n]; 
	
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> resultat(values);
	
	for (size_t k(resultat.size()/2); k >= 1; k /= 2)
        for (size_t i(k+1); i <= resultat.size(); ++i) {
            int j(i-k);
            while (j > 0) {
                if (resultat[j-1] < resultat[j+k-1]) { 
                    std::swap(resultat[j-1], resultat[j+k-1]);
                    j -= k;
                } else {
                    j = 0;
                }
            }
        }
	return resultat;
}
    
    
std::vector<size_t> Network::neighbors(const size_t& n) const {
	
	std::vector<size_t> resultat;
	
	for (std::multimap<size_t,size_t>::const_iterator it(links.begin());it!=links.end();++it) 
	
		if (it->first == n) {
			resultat.push_back(it->second);	
		}
	return resultat;
	
}

void Network::affichevalues() {
	for (auto element:values)
		std::cout<<element<<" "<<'\n';	
}


void Network::affichemap() {
	
}

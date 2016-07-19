/**
 * This file is part of MetaBoFlux (http://www.cbib.u-bordeaux2.fr/metaboflux/)
 * Copyright (C) 2010 Amine Ghozlane from LaBRI and University of Bordeaux 1
 *
 * MetaBoFlux is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MetaBoFlux is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * \file gsl_recuit.h
 * \brief Compute the simulated annealing
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#ifndef GSL_RECUIT_H_
#define GSL_RECUIT_H_

/* Gsl */

/* Fonction d'energie */
double Recuit_energyFunction(void *);

/* Distance entre les variables*/
double Recuit_metricDistance(void *, void *);

/* Prend un pas a travers l'espace des solutions TSP */
void Recuit_takeStep(const gsl_rng*, void *, double);

/* Affichage des valeurs prises */
void Recuit_printPosition(void *);

/* Redirection du flux stdout */
FILE * Recuit_redirectionFlux(xmlConfig_t *, FILE *,char **, int);

/* Choix des parametres et verification de leur valeur */
void Recuit_verifParameters(double *, const gsl_rng*, int, int);

/* Choix des parametres et verification de leur valeur */
void Recuit_verifParameters_2(double *, double *, const gsl_rng*, int, int, double);

/* Definition des parametres initiaux de la simulation */
void Recuit_defParametre(double *,const gsl_rng*);

/* Affichage des parametres initiaux */
void Recuit_printParametre(double *);

/* Realise le recuit simule */
extern void Recuit_compute_recuit(char **, int, int, pListParameters, gsl_siman_params_t);

#endif /* GSL_RECUIT_H_ */

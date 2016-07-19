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
 * \file gsl_min.h
 * \brief Compute minimization of scenarii
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#ifndef GSL_MIN_H_
#define GSL_MIN_H_

/***************************/
/* Prototype des fonctions */
/***************************/
/* Copie de tableau */
int Min_copieTab2(double *, int, int, int);

/* Copie de tableau */
int Min_copieTab3( gsl_multimin_fminimizer *, int, int, int);

/* Verification des valeurs des parametres */
void Min_verifValue(gsl_vector *, double *, int, int);

/* Fonction de calcul d'energie */
double Min_my_f (const gsl_vector *, void *);

/* Met en tampon la meilleure energie */
void Min_getTampon(double *, double);

/* Sortie du resultat des simulations */
void Min_score_print_mean(double *);

/* Minimize les solutions */
extern void Min_compute_minimization(pListParameters, double *, double *, char **, int, int);

#endif /* GSL_MIN_H_ */

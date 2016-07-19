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
 * \file gsl_mod.h
 * \brief Compute the modeling of scenarii
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#ifndef GSL_MOD_H_
#define GSL_MOD_H_

/* Sortie du resultat des simulations */
void Mod_score_print_mean(/*FILE *, FILE *,*/ pListParameters, pSimParameters, double *, double, int, int);

/* Modelisation des solutions */
extern void Mod_compute_modeling(pListParameters, double *, double *, char **, int, int, int);

#endif /* GSL_MOD_H_ */

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
 * \file gsl_mod.c
 * \brief Compute the modeling of scenarii
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_siman.h>
#include <gsl/gsl_ieee_utils.h>
#include <sbml/SBMLTypes.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "especes.h"
#include "equations.h"
#include "simulation.h"
#include "xml_parameter.h"
#include "data_parameters.h"
#include "gsl_mod.h"

/**
 * \fn void Mod_score_print_mean(pListParameters a, pSimParameters simu, double *result_tab, double energie, int number_group, int group)
 * \author Amine Ghozlane
 * \brief  Print the mean score
 * \param  a Global parameters
 * \param  simu Simulation parameters
 * \param  result_tab Result table
 * \param  energie Energy of the simulation
 * \param  number_group Number of simulation
 * \param  group Number of the group (if a group is simulated)
 */
void Mod_score_print_mean(pListParameters a, pSimParameters simu, double *result_tab, double energie, int number_group, int group)
{
  /* Sortie du resultat des simulations */
  double simulated_quantite=0.0;
  int i,j=0;

  /* Affichage du numero de groupe */
  if(group){
      result_tab[j]=(double)number_group;
      j++;
  }

  /* Affiche l'energie du systeme */
  result_tab[j]=energie;
  j++;

  /* Affiche la quantite des especes etudiees */
  for(i=0;i<simu->out->nb_species;i++){
    if( simu->out->species_amount[i]!=(-1) ){
      simulated_quantite=Equations_findSpecies(simu->out->species[i],simu->out->name,simu->out->quantite,simu->out->taille);
      result_tab[j]=simulated_quantite;
      j++;
    }
    simulated_quantite=0.0;
  }

  /* Impression des quantites de toutes les especes */
  for(i=0;i<simu->out->tailleSpecies;i++){
    result_tab[j]=simu->out->quantite[i];
    j++;
  }

  /* Affiche les ratios identifies pour les reactions du systeme */
  for (i =0; i < a->nb_parameters; i++){
    result_tab[j]=simu->y[i];
    j++;
  }
}

/**
 * \fn void Mod_compute_modeling(pListParameters a, double *fluxRatio, double *result_tab, char **files_path, int number_group, int group, int debug)
 * \author Amine Ghozlane
 * \brief  Compute the modeling
 * \param  a struct ListParameters
 * \param  fluxRatio Ratio parameters
 * \param  result_tab Result table
 * \param  files_path List of paths
 * \param  number_group Number of the group
 * \param  group Group flag
 * \param  debug Debug flag
 */
void Mod_compute_modeling(pListParameters a, double *fluxRatio, double *result_tab, char **files_path, int number_group, int group, int debug)
{
  /* Modelisation des solutions */
  pSimParameters simu=NULL;
  pScore result_temp=NULL;
  double energie=0.0;
  int i;

  /* Allocation de la memoire aux parametres simules */
  simu=(pSimParameters)malloc(sizeof(SimParameters));
  assert(simu!=NULL);

  /* Reajustement pour le numero des groupes */
  number_group=(number_group+1);

  /* Intialisation des parametres de simulation */
  if(group) Data_simParameters(a,simu,files_path[2],"modeling_group", number_group, debug);
  else Data_simParameters(a,simu,files_path[2],"modeling", number_group, debug);

  /* Initialisation des parametres des reactions */
  for(i=0;i<a->nb_parameters;i++){
    simu->y[i]=fluxRatio[i];
  }

  /* Initialisation de la structure de score temporaire */
  result_temp=Data_scoreAlloc(a);

  /* Initialisation des equations */
  simu->pile=Data_equationsAlloc(a,simu->pile);
  Data_equationsInit(a,simu->pile);

  /* Simulation */
  SBML_compute_simulation_mean(simu->debugFile,simu->out, result_temp, a->model, simu->y, simu->r, a->banned, a->nb_banned, Xml_getNbTriesMod(a->conf));

  /* Calcul de l'energie */
  for(i=0;i<a->nb_equations;i++){
    energie+=Equations_calcul(simu->pile[i], simu->out->name, simu->out->quantite, simu->out->taille);
    /*if(simu->pile[i]!=NULL) free(simu->pile[i]);*/
  }

  /* Energie des molecules */
  energie+=Equations_finalQuantite(simu->out->nb_species,simu->out->species, simu->out->species_amount, simu->out->species_weight, simu->out->name, simu->out->quantite,
      simu->out->taille);

  /* Racine de l'energie */
  energie=sqrt(energie);

  /* Debug file */
  if(simu->debugFile!=NULL) SBML_debugPrint(simu->debugFile, simu->out->tailleSpecies, simu->out->taille, simu->out->quantite, energie);

  /* Affichage des resultats */
  Mod_score_print_mean(a, simu, result_tab, energie, number_group, group);

  /* Desallocation memoire */
  Data_desallocSim(simu);

  /*  Liberation du score */
  Data_scoreFree(result_temp);
}


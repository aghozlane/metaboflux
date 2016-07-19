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
 * \file gsl_sd.c
 * \brief Compute the standard deviation analysis of the simulations
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
#include <gsl/gsl_statistics_double.h>
#include <sbml/SBMLTypes.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "especes.h"
#include "equations.h"
#include "simulation.h"
#include "xml_parameter.h"
#include "data_parameters.h"
#include "gsl_sd.h"

/**
 * \fn double Sd_compute_simulation(pListParameters all, pSimParameters sim)
 * \author Amine Ghozlane
 * \brief  Enter in the program for standard deviation
 * \param  all Global parameters : struct ListParameters
 * \param  sim Simulation parameters : struct SimParameters
 * \return Energy value
 */
double Sd_compute_simulation(pListParameters all, pSimParameters sim)
{
  /* Simulations unitaires */
  double energy =0.0;
  int i, choice=0;

  /* Determine si l'entete doit etre imprime */
  if(sim->out->name[0]==NULL) choice=1;

  /* Initialisation du tableau */
  Data_scoreInit(sim->out);

  /* Initialisation des equations */
  sim->pile=Data_equationsAlloc(all, sim->pile);
  Data_equationsInit(all, sim->pile);

  /* Simulation */
  SBML_compute_simulation(sim->out, all->model, sim->y, sim->r, all->banned, all->nb_banned);

  /* Calcul du score */
  for(i=0;i<all->nb_equations;i++){
    energy+=Equations_calcul(sim->pile[i], sim->out->name, sim->out->quantite, sim->out->taille);
    /*if(sim->pile[i]!=NULL) free(sim->pile[i]);*/
  }
  energy+=Equations_finalQuantite(sim->out->nb_species,sim->out->species, sim->out->species_amount, sim->out->species_weight,
      sim->out->name, sim->out->quantite,sim->out->taille);
  energy=sqrt(energy);

  /* Debug file */
  if(sim->debugFile!=NULL){
    if(choice) SBML_debugPrintHead(sim->debugFile, sim->out->taille, sim->out->name);
    SBML_debugPrint(sim->debugFile, sim->out->tailleSpecies, sim->out->taille, sim->out->quantite, energy);
  }

  /* Reinitialisation des equations */
  if(sim->pile!=NULL) free(sim->pile);
  return energy;
}

/**
 * \fn void Sd_compute_standard_deviation(pListParameters a, double *fluxRatio, double *result_tab, char **files_path, int number_arg, int debug)
 * \author Amine Ghozlane
 * \brief  Compute the standard deviation
 * \param  a struct ListParameters
 * \param  fluxRatio Ratio parameters
 * \param  result_tab Result table
 * \param  files_path List of paths
 * \param  number_arg Number of simulation
 * \param  debug Debug flag
 */
void Sd_compute_standard_deviation(pListParameters a, double *fluxRatio, double *result_tab, char **files_path, int number_arg, int debug)
{
  /* Calcul la variance */
  pSimParameters simu=NULL;
  double *energie=NULL, energie_sd=0.0;
  int i, nbTries=0;

  /* Allocation de la memoire aux parametres simules */
  simu=(pSimParameters)malloc(sizeof(SimParameters));
  assert(simu!=NULL);

  /* Demarre de 1 */
  number_arg+=1;

  /* Intialisation des parametres de simulation */
  Data_simParameters(a,simu,files_path[2],"standard_deviation", number_arg, debug);

  /* Initialisation des parametres des reactions */
  for(i=0;i<a->nb_parameters;i++){
    simu->y[i]=fluxRatio[i];
  }

  /* Nombre d'essaie */
  nbTries=Xml_getNbTriesMod(a->conf);

  /* Enregistrement des energies*/
  energie=(double*)malloc(nbTries*sizeof(double));
  for(i=0;i<nbTries;i++){
    energie[i]=0.0;
    energie[i]=Sd_compute_simulation(a, simu);
  }

  /* Calcul de la variance */
  if(nbTries>1) energie_sd=gsl_stats_sd(energie,1,nbTries);
  else energie_sd=0.0;

  /* Enregistrement des resultats */
  result_tab[0]=(double)number_arg;
  result_tab[1]=energie_sd;
  for(i=0;i<nbTries;i++){
    result_tab[i+2]=energie[i];
  }

  /* Fermeture du fichier */
  if(energie!=NULL) free(energie);

  /* Desallocation memoire */
  Data_desallocSim(simu);
}



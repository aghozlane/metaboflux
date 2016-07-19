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
 * \file gsl_recuit.c
 * \brief Compute the simulated annealing
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <sys/types.h>
#include <unistd.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_siman.h>
#include <gsl/gsl_ieee_utils.h>
#include <sbml/SBMLTypes.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "xml_parameter.h"
#include "especes.h"
#include "equations.h"
#include "simulation.h"
#include "data_parameters.h"
#include "gsl_recuit.h"

/* parametre global Recuit */
pListParameters all;
pSimParameters sim;
pScore resultemp;



/**
double Recuit_energyFunction(void *xp)
 * \author Amine Ghozlane
 * \brief  Simulate the petri net and compute the energy
 * \param  xp Short table of reaction parameters
 * \return Energy value
 */
double Recuit_energyFunction(void *xp)
{
  /* Fonction d'energie */
  double *x = (double *) xp, energy=0.0;
  int i;

  /* Initialisation du tableau */
  Data_scoreInit(resultemp);
  Data_scoreInit(sim->out);

  /* Mise a jour du tableau complet */
  Data_updateTab(all, sim, x);

  /* Initialisation des equations */
  sim->pile=Data_equationsAlloc(all, sim->pile);
  Data_equationsInit(all, sim->pile);

  /* Simulation */
  SBML_compute_simulation_mean(sim->debugFile, sim->out, resultemp, all->model, sim->y, sim->r, all->banned, all->nb_banned, all->nb_triesSa);

  /* Calcul du score */
  for(i=0;i<all->nb_equations;i++){
    energy+=Equations_calcul(sim->pile[i], sim->out->name, sim->out->quantite, sim->out->taille);
    /*fprintf(stderr,"Energy %f\n",energy);
    if(sim->pile[i]!=NULL){
      fprintf(stderr,"yoo\n");
      free(sim->pile[i]);
    }*/
  }
  energy+=Equations_finalQuantite(sim->out->nb_species,sim->out->species, sim->out->species_amount, sim->out->species_weight,
      sim->out->name, sim->out->quantite,sim->out->taille);
  energy=sqrt(energy);

  /* Debug file */
  if(sim->debugFile!=NULL) SBML_debugPrint(sim->debugFile, sim->out->tailleSpecies, sim->out->taille, sim->out->quantite, energy);

  /* Reinitialisation des equation */
  if(sim->pile!=NULL) free(sim->pile);

  return energy;
}

/**
 * \fn double Recuit_metricDistance(void *xp, void *yp)
 * \author Amine Ghozlane
 * \brief  Compute the distance between the two table
 * \param  xp Past short table of reaction parameters
 * \param  yp New short table of reaction parameters
 * \return Distance between the two table
 */
double Recuit_metricDistance(void *xp, void *yp)
{
  /* Distance entre les variables*/
  double *route1 = (double *) xp, *route2 = (double *) yp;
  double distance = 0.0;
  int i;

  for (i = 0; i < all->interest_parameters; ++i) {
    distance += fabs(route1[i]-route2[i]);
  }
  return distance;
}

/**
 * \fn void Recuit_takeStep(const gsl_rng * r, void *xp, double step_size)
 * \author Amine Ghozlane
 * \brief  Take a step through the solution space TSP
 * \param  r Random number generator
 * \param  xp Short table of reaction parameters
 * \param  step_size Size of step
 */
void Recuit_takeStep(const gsl_rng * r, void *xp, double step_size)
{
  /* Prend un pas à travers l'espace des solutions TSP */
  double *old_x = (double *) xp;
  double *new_x=(double*)malloc(all->interest_parameters*sizeof(double));
  int i, act=0;

  /* Selection des pas dans l'espace des valeurs que peuvent prendre les parametres */
  for(i=0;i<all->nb_couples;i++){
    Recuit_verifParameters_2(new_x,old_x,r,act,(act+all->parameters[i]), step_size);
    act+=all->parameters[i];
  }
  /* Copie du nouveau tableau de ratio */
  memcpy(xp, new_x, act*sizeof(*new_x));
}

/**
 * \fn void Recuit_printPosition(void *xp)
 * \author Amine Ghozlane
 * \brief  Print the table of reaction parameters
 * \param  xp Short table of reaction parameters
 */
void Recuit_printPosition(void *xp)
{
  /* Affichage des valeurs prises */
  int i;
  double *route = (double *) xp;
  printf("  [");
  /* Mise a jour du tableau long */
  Data_updateTab(all, sim, route);

  /* Affiche les ratios */
  for(i=0;i<all->nb_parameters;i++){
    printf(" %.2f ", sim->y[i]);
  }
  printf("]  ");
}

/**
 * \fn FILE * Recuit_redirectionFlux(xmlConfig_t *conf, FILE *f, char **files_path, int number)
 * \author Amine Ghozlane
 * \brief  Redirect stdout flux
 * \param  conf Struct xmlConfig_t
 * \param  f File name
 * \param  files_path List of paths
 * \param  number Index of out repertory
 * \return Out flux
 */
FILE * Recuit_redirectionFlux(xmlConfig_t *conf, FILE *f, char **files_path, int number)
{
  /* Redirection du flux stdout */
  char sortie[200];     /* Allocation du message */
  sprintf(sortie,"%s%s_simulated_annealing_%d.txt",files_path[2],Xml_getString(conf),number);
  /* Redirection des flux stdout */
  f= freopen(sortie,"wt",stdout) ;
  if(f==NULL){
    fprintf(stderr,"Error of flux redirection to \"%s\"\n",sortie);
    exit(EXIT_FAILURE);
  }
  return f;
}

/**
 * \fn void Recuit_verifParameters(double *x_initial, const gsl_rng * r, int debut, int max)
 * \author Amine Ghozlane
 * \brief  Choice of parameters and verification of their value
 * \param  x_initial table of reaction parameters
 * \param  r Random number generator
 * \param  debut Beginning
 * \param  max End
 */
void Recuit_verifParameters(double *x_initial, const gsl_rng * r, int debut, int max)
{
  /* Choix des parametres et verification de leur valeur */
  double temp;
  int temp2,i;

  do {
    temp=0.0; temp2=0;
    for(i=debut;i<max;i++){
      x_initial[i] = gsl_rng_uniform(r)*100.0;
      temp+=x_initial[i];
      if(x_initial[i]<0.0) temp2=1;
    }
  }
  while((temp>100.0)||(temp2==1));
}

/**
 * \fn void Recuit_verifParameters_2(double *new_x, double *old_x, const gsl_rng * r, int debut, int max, double step_size)
 * \author Amine Ghozlane
 * \brief  Choice of parameters and verification of their value
 * \param  new_x table of reaction parameters
 * \param  old_x table of reaction parameters
 * \param  r Random number generator
 * \param  debut Beginning
 * \param  max End
 * \param  step_size Size of the step
 */
void Recuit_verifParameters_2(double *new_x, double *old_x, const gsl_rng * r, int debut, int max, double step_size)
{
  /* Choix des parametres et verification de leur valeur */
  double temp, u = 0.0;
  int temp2,i;

  /* Choix et verification */
  do {
    temp=0.0; temp2=0;
    u=gsl_rng_uniform(r);
    for(i=debut;i<max;i++){
      new_x[i] = u * 2 * step_size - step_size + old_x[i];
      temp+=new_x[i];
      if(new_x[i]<0.0) temp2=1;
    }
  }
  while((temp>100.0)||(temp2==1));
}

/**
 * \fn void Recuit_defParametre(double *x_initial, const gsl_rng * r)
 * \author Amine Ghozlane
 * \brief  Definition of the initial parameters of the simulation
 * \param  x_initial table of reaction parameters
 * \param  r Random number generator
 */
void Recuit_defParametre(double *x_initial, const gsl_rng * r)
{
  /* Definition des parametres initiaux de la simulation */
  int i;
  int act=0;
  /* Selection aleatoire des parametres initiaux */
  for(i=0;i<all->nb_couples;i++){
    Recuit_verifParameters(x_initial,r,act,(act+all->parameters[i]));
    act+=all->parameters[i];
  }
}

/**
 * \fn void Recuit_printParametre(double *x_initial)
 * \author Amine Ghozlane
 * \brief  Print initial parameters
 * \param  x_initial table of reaction parameters
 */
void Recuit_printParametre(double *x_initial)
{
  /* Affichage des parametres initiaux */
  int i;
  printf("******************Parametres initiaux***********************\n");
  printf("Points do we try before stepping :N_TRIES=%d\nIterations for each T :ITERS_FIXED_T=%d\nStep size in random walk :STEP_SIZE=%f\nBoltzmann constant :K=%f\nInitial temperature :T_INITIAL=%f\nDamping factor for temperature MU_T=%f\nMinimal Temperature T_MIN=%e\n",
      Xml_getNbTriesSa(all->conf), Xml_getNbIters(all->conf), Xml_getStepSize(all->conf),
      Xml_getBoltzmann(all->conf), Xml_getTinitial(all->conf), Xml_getMuT(all->conf), Xml_getTmin(all->conf));
  printf("Initial state :");
  for(i=0;i<all->interest_parameters;i++) printf("%.2f\t",x_initial[i]);
  printf("\n");
  printf("*************************************************************\n");
}


/**
 * \fn void Recuit_compute_recuit(char **files_path, int debug, int number, pListParameters allone, gsl_siman_params_t params)
 * \author Amine Ghozlane
 * \brief  Compute the simulated annealing
 * \param  files_path List of paths
 * \param  debug Debug flag
 * \param  number Number of the simulation
 * \param  allone Global parameters : struct ListParameters
 * \param  params Gsl parameters
 */
void Recuit_compute_recuit(char **files_path, int debug, int number, pListParameters allone, gsl_siman_params_t params)
{
  /* Realise le recuit simule */
  FILE *f=NULL;
  double *x_initial=NULL;
  sim=NULL;

  resultemp=NULL;

  /* Debut d'application */
  /* Les parametres sont ainsi accessible aux fonctions gsl */
  all=allone;
  /* Tableau des parametres des equations */
  x_initial=(double *)malloc((all->interest_parameters)*sizeof(double));
  assert(x_initial!=NULL);

  /* Allocation de la memoire aux parametres simules */
  sim=(pSimParameters)malloc(1*sizeof(SimParameters));
  assert(sim!=NULL);

  /* Intialisation des parametres de simulation */
  Data_simParameters(all, sim, files_path[2], "simulated_annealing", (number+1), debug);

  /* Initialisation de la structure de score temporaire */
  resultemp=Data_scoreAlloc(all);

  /* Redirection du flux stdout */
  f=Recuit_redirectionFlux(all->conf, f, files_path, (number+1));

  /* Definition des parametres initiaux */
  Recuit_defParametre(x_initial,sim->r);

  /* Affichage des parametres (des reactions) de la simulation */
  Recuit_printParametre(x_initial);

  /* Resolution */
  gsl_siman_solve(sim->r, x_initial, Recuit_energyFunction, Recuit_takeStep, Recuit_metricDistance, Recuit_printPosition,NULL, NULL, NULL,all->interest_parameters*sizeof(double), params);

  /* Fin de redirection du flux stdout */
  fclose(f);

  /* Desallocation de la structure de score temporaire */
  Data_scoreFree(resultemp);

  /* Desallocation memoire de la structure Sim*/
  Data_desallocSim(sim);

  /* Desallocation de la structure des valeurs */
  if(x_initial!=NULL) free(x_initial);
}

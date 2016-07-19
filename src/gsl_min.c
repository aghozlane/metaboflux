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
 * \file gsl_min.c
 * \brief Compute minimization of scenarii
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 9 novembre 2009
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_multimin.h>
#include <gsl/gsl_ieee_utils.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_siman.h>
#include <sbml/SBMLTypes.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "especes.h"
#include "equations.h"
#include "simulation.h"
#include "xml_parameter.h"
#include "data_parameters.h"
#include "gsl_min.h"

/**********************/
/* Variables globales */
/**********************/
pListParameters a;
pSimParameters simu, simu_tampon;
pScore result_temp;
double *energie_temp;

/**
 * \fn int Min_copieTab2(double *x, int current, int debut, int fin)
 * \author Amine Ghozlane
 * \brief  Copy table of reaction parameters
 * \param  x Short table of reaction parameters
 * \param  current Line
 * \param  debut Beginning
 * \param  fin End
 * \return Number of copied element
 */

int Min_copieTab2(double *x, int current, int debut, int fin)
{
  /* Copie de tableau */
  int i;
  for(i=debut;i<fin;i++){
    x[i]=simu->y[current];
    current++;
  }
  current++;
  return current;
}

/**
 * \fn int Min_copieTab3(gsl_multimin_fminimizer *s, int current, int debut, int fin)
 * \author Amine Ghozlane
 * \brief  Copy short table of parameter into the larger table
 * \param  s Minimizer parameter
 * \param  current Line
 * \param  debut Beginning
 * \param  fin End
 * \return Number of copied element
 */
int Min_copieTab3(gsl_multimin_fminimizer *s, int current, int debut, int fin)
{
  /* Copie de tableau */
  int i;
  double temp=0.0;
  for(i=debut;i<fin;i++){
    simu->y[current]=gsl_vector_get (s->x, i);
    temp+=simu->y[current];
    current++;
  }
  simu->y[current]=100.0-temp;
  current++;
  return current;
}

/**
 * \fn void Min_verifValue(gsl_vector *v, double *x, int debut, int max)
 * \author Amine Ghozlane
 * \brief  Checking reaction ratio parameters
 * \param  v Vector of reaction parameters
 * \param  x Short table of reaction parameters
 * \param  debut Beginning
 * \param  max End
 */
void Min_verifValue(gsl_vector *v, double *x, int debut, int max)
{
  /* Verification des valeurs des parametres */
  double temp=0.0;
  int i;

  /* Selection des pas dans l'espace des valeurs que peuvent prendre les parametres */
  do {
    temp=0.0;
    for(i=debut;i<max;i++){
      x[i]=gsl_vector_get(v, i);
      while(x[i]<0.0){
        x[i]=x[i]+1.0;
        gsl_vector_set (v, i, x[i]);
      }
      temp+=x[i];
    }
    if(temp>100.0){
      for(i=debut;i<max;i++){
        x[i]=x[i]-1.0;
        gsl_vector_set (v, i, x[i]);
      }
    }
  }
  while(temp>100.0);
}

/**
 * \fn double Min_my_f (const gsl_vector *v, void *params)
 * \author Amine Ghozlane
 * \brief  Enter in the program for standard deviation
 * \param  v Vector of reaction parameters
 * \param  params Unused parameter define by GSL
 * \return Energy value
 */
double Min_my_f (const gsl_vector *v, void *params)
{
  /* Fonction de calcul d'energie */
  /* Le formalisme de gsl nous oblige a recuperer ici les parametres */
  /*double *p = (double *)params;*/
  double *x, energie=0.0;
  int i, act=0, choice=0;

  x=(double*)malloc(a->nb_parameters*sizeof(double));

  for(i=0;i<a->nb_couples;i++){
    Min_verifValue((gsl_vector *)v,x,act,(act+a->parameters[i]));
    act+=a->parameters[i];
  }

  /* Determine si l'entete doit etre imprime */
  if(simu->out->name[0]==NULL) choice=1;

  /* Initialisation du tableau */
  Data_scoreInit(result_temp);
  Data_scoreInit(simu->out);

  /* Mise a jour du tableau complet */
  Data_updateTab(a, simu, x);

  /* Initialisation des equations */
  simu->pile=Data_equationsAlloc(a,simu->pile);
  Data_equationsInit(a,simu->pile);

  /* Simulation */
  SBML_compute_simulation_mean(simu->debugFile, simu->out, result_temp, a->model, simu->y, simu->r, a->banned, a->nb_banned, Xml_getNbTriesSa(a->conf));

  /* Calcul de l'energie */
  for(i=0;i<a->nb_equations;i++){
    energie+=Equations_calcul(simu->pile[i], simu->out->name, simu->out->quantite, simu->out->taille);
    /*if(simu->pile[i]!=NULL) free(simu->pile[i]);*/
  }
  energie+=Equations_finalQuantite(simu->out->nb_species,simu->out->species, simu->out->species_amount,simu->out->species_weight,
        simu->out->name, simu->out->quantite,simu->out->taille);
  energie=sqrt(energie);

  if(energie<(*energie_temp)||(*energie_temp)<0.0) Min_getTampon(energie_temp,energie);

  /* Debug file */
  if(simu->debugFile!=NULL) SBML_debugPrint(simu->debugFile, simu->out->tailleSpecies, simu->out->taille, simu->out->quantite, energie);

  /* Reinitialisation des equation */
  if(simu->pile!=NULL) free(simu->pile);
  if(x!=NULL) free(x);
  return energie;
}

/**
 * \fn void Min_getTampon(double *energie_temp, double energie)
 * \author Amine Ghozlane
 * \brief  Enter in the program for standard deviation
 * \param  energie_temp Current energy
 * \param  energie Best energy
 */
void Min_getTampon(double *energie_temp, double energie)
{
  /* Met en tampon la meilleure energie */
  int i;
  /* Copie de l'energie dans le tampon */
  (*energie_temp)=energie;
  /* Copie de la quantite des metabolites dans le tampon */
  for(i=0;i<simu->out->taille; i++){
      simu_tampon->out->quantite[i]=simu->out->quantite[i];
  }
  /* Copie de la valeur des ratio reactions */
  for (i = 0; i < a->nb_parameters; ++i){
      simu_tampon->y[i]=simu->y[i];
  }
}

/**
 * \fn void Min_score_print_mean(double *result_tab)
 * \author Amine Ghozlane
 * \brief  Save the minimization result
 * \param  result_tab Result table
 */
void Min_score_print_mean(double *result_tab)
{
  /* Sortie du resultat des simulations */
  double simulated_quantite=0.0;
  int i,j=0;

  /* Affiche l'energie du systeme */
  result_tab[j]=(*energie_temp);
  j++;

  /* Affiche la quantite des especes etudiees */
  for(i=0;i<simu->out->nb_species;i++){
    if( simu->out->species_amount[i]!=(-1) ){
      simulated_quantite=Equations_findSpecies(simu->out->species[i],simu->out->name,simu_tampon->out->quantite,simu->out->taille);
      result_tab[j]=simulated_quantite;
      j++;
    }
    simulated_quantite=0.0;
  }

  /* Impression des quantites de toutes les especes */
  for(i=0;i<simu_tampon->out->tailleSpecies;i++){
    result_tab[j]=simu_tampon->out->quantite[i];
    j++;
  }

  /* Impression des ratio des reactions etudiees */
  for (i = 0; i < a->nb_parameters; ++i){
      result_tab[j]=simu_tampon->y[i];
      j++;
  }
 }

/**
 * \fn void Min_compute_minimization(pListParameters allone, double *fluxRatio, double *result_tab, char **files_path, int number_arg, int debug)
 * \author Amine Ghozlane
 * \brief  Compute the minimization
 * \param  allone struct ListParameters
 * \param  fluxRatio Ratio parameters
 * \param  result_tab Result table
 * \param  files_path List of paths
 * \param  number_arg Number of simulation
 * \param  debug Debug flag
 */
void Min_compute_minimization(pListParameters allone, double *fluxRatio, double *result_tab, char **files_path, int number_arg, int debug)
{
  /* Minimize les solutions */
  size_t iter = 0;
  int status,i,act=0,current=0;
  double size, *h=NULL;
  const gsl_multimin_fminimizer_type *T=NULL;
  gsl_multimin_fminimizer *s=NULL;
  energie_temp=(double*)malloc(1*sizeof(double));
  assert(energie_temp!=NULL);
  (*energie_temp)=-1.0;
  /* scale factors */
  gsl_vector *x=NULL, *step_size=NULL;
  gsl_multimin_function my_func;

  /* Initialisation des variables globales */
  simu=NULL,simu_tampon=NULL;
  a=allone;
  result_temp=NULL;

  /* Allocation de la memoire aux parametres simules */
  simu=(pSimParameters)malloc(sizeof(SimParameters));
  simu_tampon=(pSimParameters)malloc(sizeof(SimParameters));
  assert(simu!=NULL);
  assert(simu_tampon!=NULL);

  /* Initilisation a 1 */
  number_arg+=1;

  /* Intialisation des parametres de simulation */
  Data_simParameters(a,simu,files_path[2],"minimization",number_arg, debug);
  Data_simParameters(a,simu_tampon,files_path[2],"minimization",number_arg, debug);

  /* Initialisation des valeurs de y */
  for(i=0;i<a->nb_parameters;i++){
    simu->y[i]=fluxRatio[i];
    simu_tampon->y[i]=fluxRatio[i];
  }

  /* Allocation et initialisation du tableau des parametres */
  h=(double*)malloc(a->interest_parameters*sizeof(double));
  assert(h);
  for(i=0;i<a->nb_couples;i++){
    current=Min_copieTab2(h, current, act, (act+a->parameters[i]));
    act+=a->parameters[i];
  }

  /* Initialisation de la structure de score temporaire */
  result_temp=Data_scoreAlloc(a);

  /* Starting point, x = (85.0,7.5...) */
  x = gsl_vector_alloc (a->interest_parameters);
  assert(x!=NULL);
  for(i=0;i<a->interest_parameters;i++){
    gsl_vector_set (x, i, h[i]);
  }

  /* Set initial step sizes to 1 */
  step_size = gsl_vector_alloc (a->interest_parameters);
  gsl_vector_set_all (step_size, 1.0);

  /* Initialize method */
  my_func.n = a->interest_parameters;
  my_func.f = &Min_my_f;
  my_func.params = NULL;

  T = gsl_multimin_fminimizer_nmsimplex;
  s = gsl_multimin_fminimizer_alloc (T, a->interest_parameters);
  gsl_multimin_fminimizer_set (s, &my_func, x, step_size);

  /* Iterate */
  do{
    iter++;
    status = gsl_multimin_fminimizer_iterate (s);
    if (status) break;

    size = gsl_multimin_fminimizer_size (s);
    status = gsl_multimin_test_size (size, 1e-2);

    /* Convergence de l'algorithme */
    if (status == GSL_SUCCESS){
      Min_score_print_mean(result_tab);
    }
  }
  while (status == GSL_CONTINUE && iter < 1000);

  /* Desallocation des parametres GSL */
  gsl_vector_free (x);
  gsl_vector_free(step_size);
  gsl_multimin_fminimizer_free (s);

  /* Desallocation de la structure de score temporaire */
  Data_scoreFree(result_temp);

  /* Desallocation memoire */
  Data_desallocSim(simu);
  Data_desallocSim(simu_tampon);

  /* Liberation de la memoire du tableau */
  if(!h) free(h);
  free(energie_temp);
}

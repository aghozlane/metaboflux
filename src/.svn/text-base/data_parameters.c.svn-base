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
 * \file data_parameters.c
 * \brief Load data parameters
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 10 novembre 2010
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

/* XML */
/**
 * \fn xmlConfig_t *Data_initXML(char *xml_file)
 * \author Amine Ghozlane
 * \brief  Load parameter file
 * \param  xml_file Name of the parameter file
 * \return Model of the parameter file =:Struct xmlConfig_t
 */
xmlConfig_t *Data_initXML(char *xml_file)
{
  /* Initialise le fichier de parametre */
  xmlConfig_t *conf=NULL;

  /* Chargement du fichier de parametre */
  conf = Xml_loadConfig(xml_file);
  if(!xml_file){
    fprintf(stderr,"Error opening the parameter file : %s\n",xml_file);
    exit(EXIT_FAILURE);
  }

  return conf;
}

/* SBML */
/**
 * \fn Model_t *Data_initSBML(char *sbml_file)
 * \author Amine Ghozlane
 * \brief  Load SBML file
 * \param  sbml_file Name of the SBML file
 * \return Model of the SBML file
 */
Model_t *Data_initSBML(char *sbml_file)
{
  /* Initialise le fichier SBML */
  Model_t *mod=NULL;
  SBMLDocument_t *file=NULL;
  int errors =0/*,level=0, version=0*/;

  /* Chargement du fichier sbml  */
  file=readSBML(sbml_file);
  errors = SBMLDocument_getNumErrors(file);

  /* Obtention de la structure du reseau */
  mod=SBMLDocument_getModel(file);
  if(!mod){
    fprintf(stderr, "Error opening the  sbml file : %s !\n", sbml_file);
    exit(EXIT_FAILURE);
  }

  /* Error File */
  if (errors > 0) {
    fprintf(stderr,"Encountered the following SBML error(s):\n");
    SBMLDocument_printErrors(file, stdout);
    fprintf(stderr,"Simulation skipped.  Please correct the problems above first.\n");
    exit(EXIT_FAILURE);
  }
  /* Obtention des details consernant le level et la version du fichier SBML */
  /*level = SBMLDocument_getLevel(file);
  version = SBMLDocument_getVersion(file);
  printf("SBML Level : %d, version : %d\n", level, version);*/

  return mod;
}

/* ALEATOIRE */
/*void Data_newRng(gsl_rng *r)
{*/
  /* Preparation pour la prochaine utilisation de l'aleatoire */
/*  FILE *aleatoire=NULL;
  double alea=0.0;

  aleatoire=fopen("aleatoire.txt","wt");
  alea=gsl_rng_uniform(r);
  fprintf(aleatoire,"aleatoire=%lf\n",pow(alea,alea));
  fclose(aleatoire);
}*/


/**
 * \fn gsl_rng *Data_rngAlloc(gsl_rng *r)
 * \author Amine Ghozlane
 * \brief  Allocation of the random number generator
 * \param  r Random number generator
 * \return Random number generator
 */
gsl_rng *Data_rngAlloc(gsl_rng *r)
{
  /* Allocation du generateur de chiffre aleatoire */
  /*FILE *aleatoire=NULL;*/
  /*double alea=0.0;*/
  const gsl_rng_type *T=NULL;

  /* Initialisation de la variable aleatoire */
  gsl_rng_env_setup();
  /*if (!getenv("GSL_RNG_SEED")&&(aleatoire=fopen("aleatoire.txt","rt"))==NULL){*/
  gsl_rng_default_seed = time(NULL)+(double)getpid();
  /*}
   else {
     fscanf(aleatoire,"aleatoire=%lf\n",&alea);
     gsl_rng_default_seed = time(NULL)+alea*100.0+(double)getpid();
     fclose(aleatoire);
   }*/
  gsl_rng_default_seed = time(NULL)+(double)getpid();

  T = gsl_rng_default;
  r = gsl_rng_alloc(T);
  return r;
}


/* EQUATIONS */
/**
 * \fn Equations **Data_equationsAlloc(pListParameters current, Equations **pile)
 * \author Amine Ghozlane
 * \brief  Allocate the struct Equations
 * \param  current Current parameters : struct ListParameters
 * \param  pile Pile des equations : struct Equations
 * \return Allocated struct Equations
 */
Equations **Data_equationsAlloc(pListParameters current, Equations **pile)
{
  /* Allocation de la structure pEquations */
  pile=(Equations**)malloc(current->nb_equations*sizeof(pEquations));
  assert(pile!=NULL);
  return pile;
}

/**
 * \fn void Data_equationsInit(pListParameters current, Equations **pile)
 * \author Amine Ghozlane
 * \brief  Initialize equations
 * \param  current Current parameters : struct ListParameters
 * \param  pile Pile des equations : struct Equations
 */
void Data_equationsInit(pListParameters current, Equations **pile)
{
  /* Initialisation des equations */
  int i;

  /* Allocation des divers elements de l'arbre */
  for(i=0;i<current->nb_equations;i++){
    pile[i] = Equations_pileFormation(current->equation[i], current->noeud[i]);
    assert(pile[i]!=NULL);
  }
}

/**
 * \fn void Data_allocEquations(pListParameters a)
 * \author Amine Ghozlane
 * \brief  Allocate the different elements related to he struct Equations
 * \param  a Global parameters : struct ListParameters
 */
void Data_allocEquations(pListParameters a)
{
  /* Alloue les differents elements lies aux equations */
  int i;
  a->nb_equations=Xml_getNbEquations(a->conf);

  if(a->nb_equations>0){
      a->noeud=(int*)malloc(a->nb_equations*sizeof(int));
      /* Indique le nombre d'equation par noeud*/
      for(i=0;i<a->nb_equations;i++){
          a->noeud[i]=Xml_getKineticLawNodes(a->conf, i);
          if(a->noeud[i]<=0){
              fprintf(stderr,"Error : The equation has no element\n");
              exit(EXIT_FAILURE);
          }
      }

      /* Texte des equations */
      a->equation=(char****)malloc(a->nb_equations*sizeof(char***));
      for(i=0;i<a->nb_equations;i++){
          a->equation[i]=Xml_getKineticLaw(a->conf, i, a->noeud[i]);
      }
  }
}


/* SCORE */

/**
 * \fn void Data_allocScore(pScore out, xmlConfig_t *conf, Model_t *model)
 * \author Amine Ghozlane
 * \brief  Copy the partial table x in the full table y
 * \param  out Struct Score
 * \param  conf Struct xmlConfig_t
 * \param  model Model of the SBML file
 */
void Data_allocScore(pScore out, xmlConfig_t *conf, Model_t *model)
{
  /* Alloue les differents elements de la structure pScore */
  int i;
  out->name=NULL;
  out->quantite=NULL;
  out->species=NULL;
  out->species_amount=NULL;

  out->tailleReactions=(int)Model_getNumReactions(model);
  out->tailleSpecies=(int)Model_getNumSpecies(model);
  out->taille=out->tailleSpecies+out->tailleReactions;
  out->name=(char**)malloc(out->taille*sizeof(char*));
  assert(out->name!=NULL);
  out->quantite=(double*)malloc(out->taille*sizeof(double));
  assert(out->quantite!=NULL);
  for(i=0;i<out->taille;i++){
    out->name[i]=NULL;
    out->quantite[i]=0.0;
  }
  out->reaction=Xml_getReactionsNames(conf);
  out->nb_reaction=Xml_getNbReaction(conf);
  out->species=Xml_getSpecies(conf);
  out->nb_species=Xml_getNbSpecies(conf);
  out->species_amount=Xml_getallSpeciesFinalAmount(conf, out->species, out->nb_species);
  out->species_weight=Xml_getallSpeciesWeight(conf, out->species, out->nb_species);

}

/**
 * \fn pScore Data_scoreAlloc(pListParameters a)
 * \author Amine Ghozlane
 * \brief  Allocation of the struct Score
 * \param  a Global parameters : struct ListParameters
 * \return Allocated struct Score
 */
pScore Data_scoreAlloc(pListParameters a)
{
  /* Allocation de la structure de score */
  pScore result=NULL;
  result=(pScore)malloc(1*sizeof(Score));
  assert(result!=NULL);
  Data_allocScore(result,a->conf,a->model);
  return result;
}

/**
 * \fn void Data_scoreInit(pScore out)
 * \author Amine Ghozlane
 * \brief  Initialize the struct Score
 * \param  out Empty struct Score
 */
void Data_scoreInit(pScore out)
{
  /* Initialise la structure score */
  int i;

  /* Initialisation */
  for(i=0;i<out->taille;i++){
    out->quantite[i]=0.0;
  }
}

/**
 * \fn void Data_scoreFree(pScore out)
 * \author Amine Ghozlane
 * \brief  Free the struct Score
 * \param  out Struct score
 */
void Data_scoreFree(pScore out)
{
  /* Libere la memoire alloue a la structure de score */
  int i;
  /* Libere la memoire alloue au nom des molecules */
  for(i=0;i<out->taille;i++){
      if(out->name[i]!=NULL) free(out->name[i]);
  }
  if(out->name!=NULL) free(out->name);

  /* Libere la memoire alloue a la quantite des molecules */
  if(out->quantite!=NULL) free(out->quantite);

  /* Libere la memoire alloue au reaction*/
  for(i=0;i<(out->nb_reaction);i++){
    if(out->reaction[i]!=NULL) free(out->reaction[i]);
  }
  if(out->reaction!=NULL) free(out->reaction);

  /* La memoire de species n'a pas ete alloue et n'est donc pas a liberer */

  /* Libere la memoire des tableaux de quantite et de poids*/
  if(out->species_amount!=NULL) free(out->species_amount);
  if(out->species_weight!=NULL) free(out->species_weight);

  if(out!=NULL) free(out);
}

/* TABLEAU */
/**
 * \fn int Data_copieTab(pSimParameters simulated, double *x, int a, int debut, int fin)
 * \author Amine Ghozlane
 * \brief  Copy the partial table x in the full table y
 * \param  simulated Simulation parameters : struct SimParameters
 * \param  x Short table of reaction parameters
 * \param  a Line
 * \param  debut Beginning
 * \param  fin End
 * \return Number of copied element
 */
int Data_copieTab(pSimParameters simulated, double *x, int a, int debut, int fin)
{
  /* Copie des deux tableaux */
  int i;
  double temp=0.0;
  for(i=debut;i<fin;i++){
    simulated->y[a]=x[i];
    temp+=simulated->y[a];
    a++;
  }

  simulated->y[a]=100.0-temp;
  a++;
  return a;
}

/**
 * \fn void Data_updateTab(pListParameters current, pSimParameters simulated, double *x)
 * \author Amine Ghozlane
 * \brief  Copy table
 * \param  current Current parameters : struct ListParameters
 * \param  simulated Simulation parameters : struct SimParameters
 * \param  x Short table of reaction parameters
 */
void Data_updateTab(pListParameters current, pSimParameters simulated, double *x)
{
  /* Copie les donnees du tableau dans une version donnant la valeur parametre pour les 3 */
  int i,act=0,a=0;

  /* Copie des donnees de parametres*/
  for(i=0;i<current->nb_couples;i++){
    a=Data_copieTab(simulated, x, a, act, (act+current->parameters[i]));
    act+=current->parameters[i];
  }
}

/* LISTPARAMETERS */
/**
 * void Data_allocSimParameters(double *y, pListParameters a)
 * \author Amine Ghozlane
 * \brief  Allocation of tables needed for simulations
 * \param  y table of reaction parameters
 * \param  a Global parameters : struct ListParameters
 */
void Data_allocSimParameters(double *y, pListParameters a)
{
  /* Allocation des tableaux necessaires aux simulations */
  int i;

  /* Initialisation du tableau des valeurs */
  for(i=0;i<a->nb_parameters;i++){
    y[i]=0.0;
  }
}

/**
 * \fn void Data_allocParameters(pListParameters a)
 * \author Amine Ghozlane
 * \brief  Copy the partial table x in the full table y
 * \param  a Global parameters : struct ListParameters
 */
void Data_allocParameters(pListParameters a)
{
  /* Alloue les differents elements de parametre de la simulation */
  int i;
  a->nb_parameters=Xml_getNbParameters(a->conf);
  a->nb_couples=Xml_getNbCouples(a->conf);
  a->interest_parameters=a->nb_parameters-a->nb_couples;
  a->banned=Xml_getBanned(a->conf);
  a->nb_banned=Xml_getNbBannedSpecies(a->conf);
  a->nb_triesMod=Xml_getNbTriesMod(a->conf);
  a->nb_triesSa=Xml_getNbTriesSa(a->conf);


  if(a->nb_parameters==0 || a->nb_couples==0){
    fprintf(stderr,"Error the number of parameters or couples is equal to 0\n");
    exit(EXIT_FAILURE);
  }

  a->parameters=(int*)malloc(a->nb_couples*sizeof(int));
  assert(a->parameters!=NULL);

  /* Initialisation des combinaisons des couples */
  for(i=0;i<a->nb_couples;i++){
    a->parameters[i]=Xml_getNbReactioninNoeud(a->conf,i)-1;
  }
}

/**
 * \fn void Data_parameters(pListParameters a, char **files_path)
 * \author Amine Ghozlane
 * \brief  Record parameters of the simulation
 * \param  a Global parameters : struct ListParameters
 * \param  files_path List of paths
 */
void Data_parameters(pListParameters a, char **files_path)
{
  /* Enregistrement des parametres de la simulation */
  /* Parametre de la simulation */
  a->model=Data_initSBML(files_path[0]); /* Fichier SBML */
  a->conf =Data_initXML(files_path[1]); /* Fichier de parametre */


  /* Parametres */
  Data_allocParameters(a);

  /* Equations */
  Data_allocEquations(a);
}

/**
 * \fn void Data_desallocation(pListParameters a)
 * \author Amine Ghozlane
 * \brief  Free memory allocated to the various objects of the program
 * \param  a Global parameters : struct ListParameters
 */
void Data_desallocation(pListParameters a)
{
  /* Libere la memoire alloue aux differents objets du programme */
  int i,j;

  /* Desallocation du tableau du nombre de parametre */
  if(a->parameters!=NULL) free(a->parameters);

  /* Desallocation du texte des equations */
  for(i=0;i<a->nb_equations;i++){
    for(j=0;j<a->noeud[i];j++){
      if(a->equation[i][0][j]!=NULL) free(a->equation[i][0][j]);
      if(a->equation[i][1][j]!=NULL) free(a->equation[i][1][j]);
    }
    /* Liberation de la memoire allouee a equation */
    if(a->equation[i]!=NULL) free(a->equation[i]);
  }
  if(a->equation!=NULL) free(a->equation);
  if(a->noeud!=NULL) free(a->noeud);
  if(a->banned!=NULL) free(a->banned);

  /* XML */
  Xml_freeConfig(a->conf);

  /* SBML */
  Model_free(a->model);

  /*  Liberation de la memoire de la structure */
  if(a!=NULL) free(a);
}

/* SIMPARAMETERS */
/**
 * \fn void Data_simParameters(pListParameters a, pSimParameters sim, char *out, char *texte, int number, int debug)
 * \author Amine Ghozlane
 * \brief  Recording parameters specific to each simulation
 * \param  a Global parameters : struct ListParameters
 * \param  sim Simulation parameters : struct SimParameters
 * \param  out Output repertory
 * \param  texte File name
 * \param  number Number of the debug file
 * \param  debug Determine debug
 */
void Data_simParameters(pListParameters a, pSimParameters sim, char *out, char *texte, int number, int debug)
{
  /* Enregistrement des parametres specifiques a chaque simulation */
  char sortie2[200];      /* Allocation des messages */

  /* Initialisation */
  sim->out=NULL;
  sim->r=NULL;
  sim->y=NULL;
  sim->pile=NULL;
  sim->debugFile=NULL;

  /* Score */
  sim->out=(pScore)malloc(1*sizeof(Score));
  assert(sim->out!=NULL);
  Data_allocScore(sim->out,a->conf,a->model);

  /* Generateur de chiffre aleatoire */
  sim->r=Data_rngAlloc(sim->r);

  /* Parametres des reactions */
  sim->y=(double*)malloc(a->nb_parameters*sizeof(double));
  assert(sim->y!=NULL);
  Data_allocSimParameters(sim->y, a);

  /* Allocation de la memoire au fichier de sortie Debug */
  if(debug){
    sprintf(sortie2,"%sdebug_%s_%d.txt",out,texte,number);
    /* Creation du fichier */
    sim->debugFile= fopen(sortie2,"wt");
    if(sim->debugFile==NULL){
      fprintf(stderr,"Error opening Debug file : %s\n",sortie2);
      exit(EXIT_FAILURE);
    }
  }
}

/**
 * \fn void Data_desallocSim(pSimParameters sim)
 * \author Amine Ghozlane
 * \brief  Free struct parameters specific to each simulation
 * \param  sim Simulation parameters : struct SimParameters
 */
void Data_desallocSim(pSimParameters sim)
{
  /* Desallocation des parametres specifiques a chaque simulation */
  int i;

  /* Preparation pour les autres simulations */
  /*Data_newRng(sim->r);*/

  /* Debug File */
  if(sim->debugFile!=NULL) fclose(sim->debugFile);

  /* Desallocation des parametres des reactions */
  if(sim->y!=NULL) free(sim->y);

  /* Desallocation des elements du score */
  for(i=0;i<(sim->out->taille);i++){
    if(sim->out->name[i]!=NULL) free(sim->out->name[i]);
  }
  if(sim->out->name!=NULL) free(sim->out->name);
  if(sim->out->quantite!=NULL) free(sim->out->quantite);
  for(i=0;i<(sim->out->nb_reaction);i++){
    if(sim->out->reaction[i]!=NULL) free(sim->out->reaction[i]);
  }
  if(sim->out->reaction!=NULL) free(sim->out->reaction);
  if(sim->out->species!=NULL) free(sim->out->species);
  if(sim->out->species_amount!=NULL) free(sim->out->species_amount);
  if(sim->out->species_weight!=NULL) free(sim->out->species_weight);
  if(sim->out!=NULL) free(sim->out);

  /* Desallocation du generateur de chiffre aleatoire */
  if(sim->r!=NULL) gsl_rng_free(sim->r);

  /* Desallocation de sim */
  if(sim!=NULL) free(sim);
}

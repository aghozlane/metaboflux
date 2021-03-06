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
 * \file simulation.c
 * \brief Simulate a petri net
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 27 octobre 2009
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <sbml/SBMLTypes.h>
#include "especes.h"
#include "simulation.h"


/*TODO Verification des BoundaryConditions,Local parameter values, Global parameter values */

/**
 * \fn void SBML_initEspeceAmounts(Model_t *mod, pEspeces molecules, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct Especes
 * \param  mod Model of the SBML file
 * \param  molecules Struct Especes
 * \param  nbEspeces Number of molecules
 */
void SBML_initEspeceAmounts(Model_t *mod, pEspeces molecules, int nbEspeces)
{
  /* Initialisation de l'etat des especes */
  int i;
  Species_t *esp=NULL;

  /* Initialisation des quantites des especes*/
  for (i = 0; i < nbEspeces; i++) {
      esp = Model_getSpecies(mod, i);
      Especes_save(molecules, i, Species_getInitialAmount(esp),
          Species_getId(esp));
  }
}

/**
 * \fn int SBML_findReaction(char **reaction, const char *react, int nb_reaction)
 * \author Amine Ghozlane
 * \brief  Determine if the reaction is study
 * \param  reaction List of reactions
 * \param  react Reaction of interest
 * \param  nb_reaction Number of reactions
 * \return Number of the molecules if it's study
 */
int SBML_findReaction(char **reaction, const char *react, int nb_reaction)
{
  /* Determine si la reation indique est etudiee */
  int i;
  for(i=0;i<nb_reaction;i++){
      /* La reaction est etudiee, on retourne son numero */
      if(!strcmp(reaction[i], react)) return i;
  }
  /* La reaction n'est pas etudiee */
  return OK;
}

/**
 * \fn double SBML_evalExpression(const char *formule)
 * \author Amine Ghozlane
 * \brief  Get the reaction ratio define in the sbml
 * \param  formule Formule SBML
 * \return Return double value of the constraint
 */
double SBML_evalExpression(const char *formule)
{
  /* Caste la formule en double */
  return atof(formule);
}

/**
 * \fn void SBML_setReactions(Model_t *mod, pEspeces molecules, pScore result, double *reactions_ratio, int nbReactions, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct Especes
 * \param  mod Model of the SBML file
 * \param  molecules Struct Especes
 * \param  result Struct Score
 * \param  reactions_ratio List of computed reaction ratio
 * \param  nbReactions Number of reaction
 * \param  nbEspeces Number of molecules
 */
void SBML_setReactions(Model_t *mod, pEspeces molecules, pScore result, double *reactions_ratio, int nbReactions, int nbEspeces)
{
  /* Initialisation les reactions auquelles participent chaque espece */
  int ref = 0, i, j, resultat=OK;
  SpeciesReference_t *reactif=NULL;
  Species_t *especeId=NULL;
  Reaction_t *react=NULL;
  const char *kf=NULL;
  /*const ASTNode_t *km;*/
  KineticLaw_t *kl=NULL;

  /* Recherche les reactions ou apparaissent chaque espece */
  for (i = 0; i < nbReactions; i++) {
      react = Model_getReaction(mod, i);
      /* Recherche si la reaction est etudiee */
      resultat = SBML_findReaction(result->reaction,Reaction_getId(react), result->nb_reaction);
      /* Etude des reactifs */
      for (j = 0; j < (int)Reaction_getNumReactants(react); j++) {
          /* Recuperation d'un reactif */
          reactif = Reaction_getReactant(react, j);
          /* Recuperation du nom de l'espece */
          especeId = Model_getSpeciesById(mod, SpeciesReference_getSpecies(reactif));
          /* Recherche la reference de cette molecule dans la struture molecule */
          ref = Especes_find(molecules, Species_getId(especeId), nbEspeces);
          /* Recuperation de la loi cinetique sur la */
          kl = Reaction_getKineticLaw(react);
          /* Reaction dont on recherche le bon ratio */
          if(resultat!=OK){
              Especes_allocReactions(molecules, ref, react, reactions_ratio[resultat]);
          }
          /* Si la loi cinetique est au format : formule on recupere le ratio en question */
          else if (KineticLaw_isSetFormula(kl)) {
              kf = KineticLaw_getFormula(kl);
              Especes_allocReactions(molecules, ref, react, SBML_evalExpression(kf));
          } else { /* Sinon on n'utilise pas ces informations */
              /*km = KineticLaw_getMath(kl);*/
              fprintf(stderr,"SBML equation are taken into account, use instead the parameter file\n");
              /*exit(EXIT_FAILURE);*/
          }
      }
  }
}

/**
 * \fn int SBML_checkQuantite(Model_t *mod, Reaction_t *react, int nbEspeces, pEspeces molecules)
 * \author Amine Ghozlane
 * \brief  Determine the number of reaction for one molecule
 * \param  mod Model of the SBML file
 * \param  react Reaction id
 * \param  nbEspeces Number of molecules
 * \param  molecules Struct Especes
 * \return Number of reaction for one molecule
 */
int SBML_checkQuantite(Model_t *mod, Reaction_t *react, int nbEspeces, pEspeces molecules)
{
  /* Determine le nombre de reaction possible a partir de la quantite des reactifs */
  int  ref = 0, i;
  double quantite = 0.0, minStep = 0.0, temp = 0.0;
  SpeciesReference_t *reactif;
  Species_t *especeId;
  /*TODO Il est possible de reprogrammer ca plus proprement */
  /* Recupere la quantite de la premiere molecule */
  reactif = Reaction_getReactant(react, 0);
  especeId = Model_getSpeciesById(mod, SpeciesReference_getSpecies(reactif));
  ref = Especes_find(molecules, Species_getId(especeId), nbEspeces);
  /* Cas ou la quantite de la molecule est egale a zero */
  if ((quantite = Especes_getQuantite(molecules, ref)) <= 0.0) return END;
  /* Calcul du nombre de pas minimum qu'il sera possible d'effectuer */
  /* C'est le nombre minimum d'etat de tous les reactifs qui determine le nombre de pas */
  minStep = floor(quantite / SpeciesReference_getStoichiometry(reactif));
  /* Cas ou le nombre de pas est egal a 0 */
  if(minStep==0.0) return END;

  /* On calcule pour les autres reactifs */
  for (i = 1; i < (int)Reaction_getNumReactants(react); i++) {
      /* Recupere la quantite d'une molecule */
      reactif = Reaction_getReactant(react, i);
      especeId = Model_getSpeciesById(mod, SpeciesReference_getSpecies(reactif));
      ref = Especes_find(molecules, Species_getId(especeId), nbEspeces);
      quantite= Especes_getQuantite(molecules, ref);
      /* La quantite est egale a 0 */
      if(quantite<=0.0) return END;
      /* On calcule le nombre de pas minimum qu'il sera possible d'effectuer */
      temp = floor(Especes_getQuantite(molecules, ref)/SpeciesReference_getStoichiometry(reactif));
      /* Cas ou le nombre de pas est egal a 0 */
      if(temp==0.0) return END;
      /* Si le nouveau nombre est inferieur au precedent, on change la valeur de minStep */
      if (minStep > temp) minStep = temp;
  }
  /*fprintf(stderr,"minStep returned %d\n",(int)minStep);*/
  return (int)minStep;
}

/**
 * \fn Reaction_t * SBML_reactChoice(pEspeces molecules, const gsl_rng * r, int ref)
 * \author Amine Ghozlane
 * \brief  Determine randomly the reaction to achieve for several nodes reactions
 * \param  molecules Struct Especes
 * \param  r Random number generator
 * \param  ref Number reference of one molecule
 * \return Id of the selected reaction
 */
Reaction_t * SBML_reactChoice(pEspeces molecules, const gsl_rng * r, int ref)
{
  /* Determine aleatoirement la reaction a realiser pour les noeuds de plusieurs reactions */
  pReaction temp = NULL;
  pReaction Q = molecules[ref].system;
  double value = gsl_rng_uniform(r) * 100.0;
  double choice = 0.0;

  /* Si la valeur choisie est inferieure au ratio alloue a la premiere reaction du noeud */
  if(value<Q->ratio) temp=Q;

  /* Cas ou il faut ajouter les ratio des autres reactions du noeud */
  else{
      do {
          choice += Q->ratio;
          Q = Q->suivant;
          temp=Q;
      }while (Q->suivant != NULL && value <= (choice + Q->suivant->ratio) && value > choice);
  }

  /* Temp n'indique aucune reaction */
  if(temp==NULL){
      fprintf(stderr,"There is something wrong with the ratio data\n");
      exit(EXIT_FAILURE);
  }
  /* retourne la reaction choisie */
  return (temp->link);
}

/**
 * \fn void SBML_reaction(Model_t *mod, pEspeces molecules, Reaction_t *react, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Simulation of a discrete transision
 * \param  mod Model of the SBML file
 * \param  molecules Struct Especes
 * \param  react Reaction id
 * \param  nbEspeces Number of molecules
 */
void SBML_reaction(Model_t *mod, pEspeces molecules, Reaction_t *react, int nbEspeces)
{
  /* Simulation d'une transision discrete */
  SpeciesReference_t *reactif;
  Species_t *especeId;
  int i, ref = 0;

  /*boucle pour retirer des reactifs*/
  for (i = 0; i < (int)Reaction_getNumReactants(react); i++) {
      /* Indentification du reactif */
      reactif = Reaction_getReactant(react, i);
      especeId = Model_getSpeciesById(mod, SpeciesReference_getSpecies(reactif));
      ref = Especes_find(molecules, Species_getId(especeId), nbEspeces);
      /* Modification de sa quantite */
      Especes_setQuantite(molecules, ref, (Especes_getQuantite(molecules, ref)- SpeciesReference_getStoichiometry(reactif)));
  }

  /*boucle pour ajouter des produits */
  for (i = 0; i < (int)Reaction_getNumProducts(react); i++) {
      /* Indentification du reactif */
      reactif = Reaction_getProduct(react, i);
      especeId = Model_getSpeciesById(mod, SpeciesReference_getSpecies(reactif));
      ref = Especes_find(molecules, Species_getId(especeId), nbEspeces);
      /* Modification de sa quantite */
      Especes_setQuantite(molecules, ref, (Especes_getQuantite(molecules, ref)+ SpeciesReference_getStoichiometry(reactif)));
  }
}

/**
 * \fn void SBML_allocTest(pTestReaction T, int nbReactions)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct pTestReaction
 * \param  T Empty struct TestReaction
 * \param  nbReactions Number of reactions
 */
void SBML_allocTest(pTestReaction T, int nbReactions)
{
  /* Alloue la memoire necessaire a la struture de test des reactions */
  int i;

  /* Initialisation des tableaux des reactions */
  T->tabReactions = (Reaction_t **) malloc(nbReactions * sizeof(Reaction_t *));
  T->minStepTab = (int*) malloc(nbReactions * sizeof(int));

  assert(T->tabReactions!=NULL);
  assert(T->minStepTab!=NULL);

  /* Initialisation */
  for (i = 0; i < nbReactions; i++){
      T->tabReactions[i] = NULL;
      T->minStepTab[i]=0;
  }
}

/**
 * \fn void SBML_freeTest(pTestReaction T)
 * \author Amine Ghozlane
 * \brief  Free memory of the struct TestReaction
 * \param T Struct TestReaction gives data on reaction
 */
void SBML_freeTest(pTestReaction T)
{
  /* Libere la memoire allouee a la struture de test des reactions */
  if(T->tabReactions!=NULL) free(T->tabReactions);
  if(T->minStepTab!=NULL) free(T->minStepTab);
}

/**
 * \fn int SBML_EstimationReaction(Model_t *mod, pTestReaction T, pEspeces molecules, int ref, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct Especes
 * \param  mod Model of the SBML file
 * \param  T Struct TestReaction gives data on reaction
 * \param  molecules Struct Especes
 * \param  ref Number reference of one molecule
 * \param  nbEspeces Number of molecules
 * \return Estimated number of feasible step by reaction
 */
int SBML_EstimationReaction(Model_t *mod, pTestReaction T, pEspeces molecules, int ref, int nbEspeces)
{
  pReaction Q = molecules[ref].system;
  int i=0, min=0, curr=0;

  /* Compte le nombre de reactions rattachees a une espece */
  while (Q!= NULL) {
      T->tabReactions[i]=Q->link;
      T->minStepTab[i]=SBML_checkQuantite(mod, Q->link, nbEspeces, molecules);
      if(i==0) min=T->minStepTab[i];
      curr=T->minStepTab[i];
      if(T->minStepTab[i]<=0)     return END;
      else if(curr<min) min=curr;
      Q = Q->suivant;
      i++;
  }
  /* Nombre d'etape realisable par reaction */
  return min;
}

/**
 * \fn int SBML_simulate(Model_t *mod, pEspeces molecules, const gsl_rng * r, pTestReaction T, char **banned, int nbBanned, int nbEspeces, int ref)
 * \author Amine Ghozlane
 * \brief  Simulate one step of petri net
 * \param  mod Model of the SBML file
 * \param  molecules Struct Especes
 * \param  r Random number generator
 * \param  T Struct TestReaction gives data on reaction
 * \param  banned List of banned compound
 * \param  nbBanned Number of banned compound
 * \param  nbEspeces Number of molecules
 * \param  ref Number reference of one molecule
 * \return Condition of stop/pursue
 */
int SBML_simulate(Model_t *mod, pEspeces molecules, const gsl_rng * r, pTestReaction T, char **banned, int nbBanned, int nbEspeces, int ref)
{
  Reaction_t *react=NULL;
  int i, minStep = 0, valid=0, nbReactions = 0;

  /* Probleme ATP, ADP, NADH, NAD+ */
  for(i=0;i<nbBanned;i++){
      if(!strcmp(molecules[ref].id,banned[i])) return END;
  }
  /* Decompte du nombre de reaction */
  /*nbReactions = Especes_getNbreactions(molecules, ref);*/
  nbReactions = molecules[ref].nbReactions;
  /* Variation selon le cas. */
  switch (nbReactions) {
  /* Cas ou aucune reaction n'est possible */
  case 0:
    return END;
    break;
  /* Cas ou une seule reaction est possible */
  case 1:
    react = molecules[ref].system->link;
    if ((minStep = SBML_checkQuantite(mod, react, nbEspeces, molecules))<= END)
      return END;
    /* Tant qu'il reste des molecules, on realise la reaction */
    while (minStep > 0) {
        SBML_reaction(mod, molecules,  react, nbEspeces);
        minStep--;
    }
    break;
  /* Cas ou  plusieurs reactions sont possibles*/
  default:
    /* Allocation de la memoire au tableau des reactions */
    SBML_allocTest(T, nbReactions);
    /* Estimation du nombre de reaction realisable */
    valid = SBML_EstimationReaction(mod, T, molecules, ref, nbEspeces);
    /* Il n'y a plus de reactions realisables */
    if(valid<=END){
        SBML_freeTest(T);
        return END;
    }
    /* On realise les reactions */
    while(Especes_getQuantite(molecules, ref)> 0.0 && valid>END) {
        react = SBML_reactChoice(molecules, r, ref);
        SBML_reaction(mod, molecules,  react, nbEspeces);
        valid--;
    }
    /* Liberation de la memoire allouee au tableau des reactions */
    SBML_freeTest(T);
    break;
  }
  return PURSUE;
}

/**
 * \fn void SBML_score(Model_t *mod, pEspeces molecules, pScore result, double *reactions_ratio, int nbReactions, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct Especes
 * \param  mod Model of the SBML file
 * \param  molecules Struct Especes
 * \param  result Struct Score
 * \param  reactions_ratio List of computed reaction ratio
 * \param  nbReactions Number of reactions
 * \param  nbEspeces Number of molecules
 */
void SBML_score(Model_t *mod, pEspeces molecules, pScore result, double *reactions_ratio, int nbReactions, int nbEspeces)
{
  /* Fonction de score */
  int  i, resultat=OK;
  Reaction_t *react=NULL;
  const char *kf=NULL;
  KineticLaw_t *kl=NULL;

  /* Enregistre le score des especes */
  Especes_scoreSpecies(molecules, nbEspeces, result->name, result->quantite);

  /* Enregistre le ratio des reactions */
  for (i = 0; i < (nbReactions); i++) {
      react = Model_getReaction(mod, i);
      if(result->name[nbEspeces+i]==NULL){
          result->name[nbEspeces+i]=(char*)malloc(((int)strlen((char *)Reaction_getId(react))+1)*sizeof(char));
          assert(result->name[nbEspeces+i]!=NULL);
          strcpy(result->name[nbEspeces+i],(char *)Reaction_getId(react));
      }

      /* Recherche si la reaction est etudiee*/
      resultat = SBML_findReaction(result->reaction, result->name[nbEspeces+i], result->nb_reaction);
      /* Recuperation de la loi cinetique sur la */
      kl = Reaction_getKineticLaw(react);
      /* Reaction dont on recherche le bon ratio*/
      if(resultat!=OK) result->quantite[nbEspeces+i]=reactions_ratio[resultat];
      else if(KineticLaw_isSetFormula(kl)){
          kf = KineticLaw_getFormula(kl);
          result->quantite[nbEspeces+i]=SBML_evalExpression(kf);
      }
      else {  /*Sinon on n'utilise pas ces informations*/
          fprintf(stderr,"SBML equation are taken into account, use instead the parameter file\n");
          exit(EXIT_FAILURE);
      }
  }
}

/**
 * \fn void SBML_debugPrintHead(FILE *debugFile, int taille, char **name)
 * \author Amine Ghozlane
 * \brief  Print the head of the debug file
 * \param  debugFile File use for debug
 * \param  taille Number of molecules/reactions
 * \param  name List of molecules/reactions
 */
void SBML_debugPrintHead(FILE *debugFile, int taille, char **name)
{
  /* Affichage de l'entete du fichier debug */
  int i;
  fprintf(debugFile,"Energy\t");
  for(i=0;i<taille;i++){
      fprintf(debugFile,"%s\t",name[i]);
  }
  fprintf(debugFile,"\n");
}

/**
 * \fn void SBML_debugPrint(FILE *debugFile,int tailleSpecies, int taille, double *quantite, double result)
 * \author Amine Ghozlane
 * \brief  Print the debuf file
 * \param  debugFile File use for debug
 * \param  tailleSpecies Number of molecules
 * \param  taille Number of molecules/reactions
 * \param  quantite Quantity of molecules/reactions
 * \param  result Energy value
 */
void SBML_debugPrint(FILE *debugFile,int tailleSpecies, int taille, double *quantite, double result)
{
  /* Affiche tous les resultats de toutes les simulations */
  int i;

  /* Affichage de l'energie */
  if(result!=-1.0) fprintf(debugFile,"%.3f\t",result);
  else fprintf(debugFile,"NAN\t");
  /* Affichage des quantites et ratios */
  for(i=0;i<tailleSpecies;i++){
      fprintf(debugFile,"%.0f\t",quantite[i]);
  }
  for(i=tailleSpecies;i<taille;i++){
      fprintf(debugFile,"%.2f\t",quantite[i]);
  }
  fprintf(debugFile,"\n");
}

/**
 * \fn void SBML_compute_simulation(pScore result, Model_t *mod, double *reactions_ratio, gsl_rng * r, char **banned, int nbBanned)
 * \author Amine Ghozlane
 * \brief  Simulation of metabolic network
 * \param  result Struct Score
 * \param  mod Model of the SBML file
 * \param  reactions_ratio List of computed reaction ratio
 * \param  r Random number generator
 * \param  banned List of banned compound
 * \param  nbBanned Number of banned compound
 */
void SBML_compute_simulation(pScore result, Model_t *mod, double *reactions_ratio, gsl_rng * r, char **banned, int nbBanned)
{
  /* Simulation du reseau metabolique  */
  int i, temp = 1;
  pEspeces molecules=NULL;
  pTestReaction TR=NULL;

  /* Allocation memoire */
  TR=(pTestReaction)malloc(1*sizeof(TestReaction));
  assert(TR!=NULL);

  /* File information */
  molecules = Especes_alloc(result->tailleSpecies);

  /* Initialisation de la quantite des especes */
  SBML_initEspeceAmounts(mod, molecules, result->tailleSpecies);

  /* Initialisation des reactions et des ratios*/
  SBML_setReactions(mod, molecules, result, reactions_ratio, result->tailleReactions, result->tailleSpecies);

  /* Simulation des reactions */
  while (temp > END) {
      temp = 0;
      for (i = 0; i < result->tailleSpecies; i++) {
          temp +=SBML_simulate(mod, molecules, r, TR, banned, nbBanned, result->tailleSpecies, i);
      }
  }

  /*Score */
  SBML_score(mod, molecules, result, reactions_ratio, result->tailleReactions, result->tailleSpecies);

  /* Liberation de la memoire de la structure Especes */
  Especes_free(molecules, result->tailleSpecies);
  if(TR!=NULL) free(TR);
}

/**
 * \fn void SBML_score_add(pScore result, pScore result_temp, FILE *debugFile)
 * \author Amine Ghozlane
 * \brief  Add scores
 * \param  result Struct Score used for all the simulation
 * \param  result_temp Struct Score used at each simulation step
 * \param  debugFile File use for debug
 */
void SBML_score_add(pScore result, pScore result_temp, FILE *debugFile)
{
  /* Addition des scores */
  int i, choice=0;

  /* Determine si l'entete doit etre imprime */
  if(result->name[0]==NULL) choice=1;

  /* Affichage de l'entete du fichier debug */
  if(debugFile!=NULL && choice) SBML_debugPrintHead(debugFile, result_temp->taille, result_temp->name);

  /* Copie des resultats */
  for(i=0;i<result->taille;i++){
      if(result->name[i]==NULL){
          result->name[i]=(char*)malloc(((int)strlen(result_temp->name[i])+1)*sizeof(char));
          assert(result->name[i]!=NULL);
          strcpy(result->name[i], result_temp->name[i]);
      }
      result->quantite[i]+=result_temp->quantite[i];
  }
  /* Affichage des donnees debug */
  if(debugFile!=NULL) SBML_debugPrint(debugFile,result->tailleSpecies, result->taille, result_temp->quantite, -1.0);
}

/**
 * \fn void SBML_score_mean(pScore result, int n)
 * \author Amine Ghozlane
 * \brief  Mean quantities for score
 * \param  result Struct Score
 * \param  n Number of simulation step
 */
void SBML_score_mean(pScore result, int n)
{
  /* Moyenne des resultats */
  int i;
  for(i=0;i<result->taille;i++){
      /*result->quantite[i]/=(double)n;*/
      result->quantite[i]=floor(result->quantite[i]/(double)n);
  }
}

/**
 * \fn void SBML_compute_simulation_mean(FILE *debugFile, pScore result, pScore result_temp, Model_t *mod, double *reactions_ratio, gsl_rng * r, char **banned, int nbBanned, int nb_simulation)
 * \author Amine Ghozlane
 * \brief  X time simulation of metabolic network
 * \param  debugFile File use for debug
 * \param  result Struct Score used for all the simulation
 * \param  result_temp Struct Score used at each simulation step
 * \param  mod Model of the SBML file
 * \param  reactions_ratio List of computed reaction ratio
 * \param  r Random number generator
 * \param  banned List of banned compound
 * \param  nbBanned Number of banned compound
 * \param  nb_simulation Number of simulation step
 */
void SBML_compute_simulation_mean(FILE *debugFile, pScore result, pScore result_temp, Model_t *mod, double *reactions_ratio, gsl_rng * r, char **banned, int nbBanned, int nb_simulation)
{
  /* Simulation du reseau metabolique  */
  int i, j,nbReactions = 0, nbEspeces = 0, temp = 1, tempo=0;
  pEspeces molecules=NULL;
  pTestReaction TR=NULL;

  /* Allocation memoire */
  TR=(pTestReaction)malloc(1*sizeof(TestReaction));
  assert(TR!=NULL);

  /* File information */
  nbReactions = (int) Model_getNumReactions(mod);
  nbEspeces = (int)Model_getNumSpecies(mod);
  molecules = Especes_alloc(nbEspeces);

  /* Initialisation de la quantite des especes */
  SBML_initEspeceAmounts(mod, molecules, nbEspeces);

  /* Initialisation des reactions et des ratios*/
  SBML_setReactions(mod, molecules, result_temp, reactions_ratio, nbReactions, nbEspeces);

  /* SIMULATION */
  for(j=0;j<nb_simulation;j++){
      /* Simulation des reactions */
      while (temp > END) {
          temp = 0;
          for (i = 0; i < nbEspeces; i++) {
              tempo= SBML_simulate(mod, molecules, r, TR,  banned, nbBanned, nbEspeces, i);
              temp +=tempo;
          }
      }
      temp=1;
      tempo=0;

      /*Score */
      SBML_score(mod, molecules, result_temp, reactions_ratio, nbReactions, nbEspeces);
      SBML_score_add(result,result_temp,debugFile);
      SBML_initEspeceAmounts(mod, molecules, nbEspeces);
  }
  SBML_score_mean(result,nb_simulation);

  /* Liberation de la memoire de la structure Especes */
  Especes_free(molecules, nbEspeces);
  if(TR!=NULL) free(TR);
}

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
 * \file especes.c
 * \brief Modelize a molecule
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 27 octobre 2009
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sbml/SBMLTypes.h>
#include "especes.h"

/**
 * \fn pEspeces Especes_alloc(int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct Especes
 * \param  nbEspeces Number of molecules
 * \return Allocated struct Espece
 */
pEspeces Especes_alloc(int nbEspeces)
{
  int i;
  pEspeces molecules = (pEspeces) malloc(nbEspeces * sizeof(Especes));
  assert(molecules!=NULL);

  /* Initialisation de la quantite */
  for (i = 0; i < nbEspeces; i++) {
    molecules[i].quantite = 0.0;
    molecules[i].system = NULL;
    molecules[i].nbReactions=0;
  }
  /* Alloue et initialise la memoire a la structure espece */
  return molecules;
}

/**
 * \fn void Especes_allocReactions(pEspeces molecules, int Espece, Reaction_t *local, double reactRatio)
 * \author Amine Ghozlane
 * \brief  Alloc memory and initialize the struct Reaction for each molecule
 * \param  molecules List of molecules
 * \param  Espece Number of the selected molecule
 * \param  local Reaction sbml reference
 * \param  reactRatio Ratio of the reaction
 */
void Especes_allocReactions(pEspeces molecules, int Espece, Reaction_t *local, double reactRatio)
{
  pReaction Q = molecules[Espece].system;
  pReaction new = (pReaction) malloc(1 * sizeof(Reaction));

  /* Initialisation de la nouvelle structure */
  new->link = local;
  new->ratio = reactRatio;
  new->suivant = NULL;
  /* Ajout d'une nouvelle reaction */
  molecules[Espece].nbReactions+=1;
  /* Deplacement le long de la liste chainee */
  if (Q == NULL) {
    /* Cas n°1 : premiere molecule de la chaine */
    molecules[Espece].system = new;
  } else {
    /* Cas n°2 :  deuxieme ou plus molecule de la chaine */
    while (Q->suivant != NULL) {
      Q = Q->suivant;
    }
    Q->suivant = new;
  }
}

/**
 * \fn void Especes_free(pEspeces molecules, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Free the struct pEspeces
 * \param  molecules List of molecules
 * \param  nbEspeces Number of molecules
 */
void Especes_free(pEspeces molecules, int nbEspeces)
{
  int i;
  for (i = 0; i < nbEspeces; i++) {
    Especes_freeReactions(molecules[i]);
  }
  if(molecules!=NULL) free(molecules);
}

/**
 * \fn void Especes_freeReactions(Especes molecules)
 * \author Amine Ghozlane
 * \brief  Free the struct Reaction
 * \param  molecules List of molecules
 */
void Especes_freeReactions(Especes molecules)
{
  pReaction Q=NULL;
  pReaction P=NULL;
  if(molecules.system!=NULL) Q=molecules.system;
  if (Q != NULL) {
    while (Q!= NULL) {
      P = Q->suivant;
      if(Q!=NULL)free(Q);
      Q=NULL;
      Q = P;
      P=NULL;
    }
  }
}

/**
 * \fn void Especes_save(pEspeces molecules, int i, double quant, const char *dye)
 * \author Amine Ghozlane
 * \brief  Save data on one molecule
 * \param  molecules List of molecules
 * \param  i Number of the selected molecule
 * \param  quant Quantity of the molecule
 * \param  dye ID of the molecule
 */
void Especes_save(pEspeces molecules, int i, double quant, const char *dye)
{
  molecules[i].quantite = quant;
  molecules[i].id = dye;
}

/**
 * \fn void Especes_scoreSpecies(pEspeces molecules, int nbEspeces, char **name, double *quantite)
 * \author Amine Ghozlane
 * \brief  Save the quantite of all molecules for scoring.
 * \param  molecules List of molecules
 * \param  nbEspeces Number of molecules
 * \param  name ID of the selected molecule
 * \param  quantite Table of molecule Quantity
 */
void Especes_scoreSpecies(pEspeces molecules, int nbEspeces, char **name, double *quantite)
{
  int i;
  /* Enregistre la quantite de toutes les molecules */
  for (i = 0; i < nbEspeces; i++) {
    if(name[i]==NULL){
      name[i]=(char*)malloc(((int)strlen(molecules[i].id)+1)*sizeof(char));
      assert(name[i]!=NULL);
      strcpy(name[i], molecules[i].id);
    }
    quantite[i]=molecules[i].quantite;
  }
}

/**
 * \fn void Especes_print(pEspeces molecules, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Print data on each molecule
 * \param  molecules List of molecules
 * \param  nbEspeces Number of molecules
 */
void Especes_print(pEspeces molecules, int nbEspeces)
{
  int i;
  pReaction Q = NULL;

  for (i = 0; i < nbEspeces; i++) {
    fprintf(stderr,"Molecule : %s, Quantity : %.0f\n", molecules[i].id, molecules[i].quantite);
    Q = molecules[i].system;
    if (Q != NULL) {
      /* Compte le nombre de reactions rattachees a une espece */
      while (Q != NULL) {
        fprintf(stderr,"reaction %s : ratio %.2f%%\n",Reaction_getId(Q->link),Q->ratio);
        Q = Q->suivant;
      }
      fprintf(stderr,"\n");
    }
  }
}

/**
 * \fn void Especes_print_2(pEspeces molecules, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Print Id and quantity on each molecule
 * \param  molecules List of molecules
 * \param  nbEspeces Number of molecules
 */
void Especes_print_2(pEspeces molecules, int nbEspeces)
{
  int i;

  for (i = 0; i < nbEspeces; i++) {
    fprintf(stderr,"Molecule : %s, Quantity : %.0f\n", molecules[i].id, molecules[i].quantite);
  }
}

/**
 * \fn int Especes_find(pEspeces molecules, const char *especeId, int nbEspeces)
 * \author Amine Ghozlane
 * \brief  Find a molecule thank to it ID
 * \param  molecules List of molecules
 * \param  especeId  SBML ID of the molecule
 * \param  nbEspeces Number of molecules
 * \return Number of a selected molecule
 */
int Especes_find(pEspeces molecules, const char *especeId, int nbEspeces)
{
  int i;

  for (i = 0; i < nbEspeces; i++) {
    if (!strcmp(especeId, molecules[i].id)) {
      return i;
    }
  }
  /* Cas ou la molecule n'est pas trouvee */
  if (i == nbEspeces) {
    fprintf(stderr, "Molecule \"%s\" not found\n",especeId);
    exit(EXIT_FAILURE);
  }
  return 0;
}

/**
 * \fn void Especes_setQuantite(pEspeces molecules, int Especes, double quant)
 * \author Amine Ghozlane
 * \brief  Change the quantity of a molecule
 * \param  molecules List of molecules
 * \param  Especes Number of a selected molecule
 * \param  quant Quantity of the molecule
 */
void Especes_setQuantite(pEspeces molecules, int Especes, double quant)
{
  molecules[Especes].quantite = quant;
}

/**
 * \fn double Especes_getQuantite(pEspeces molecules, int Especes)
 * \author Amine Ghozlane
 * \brief  Get the quanty of one molecule by its number
 * \param  molecules List of molecules
 * \param  Especes Number of a selected molecule
 * \return Quantity of a selected molecule
 */
double Especes_getQuantite(pEspeces molecules, int Especes)
{
  return molecules[Especes].quantite;
}

/**
 * \fn int Especes_getNbreactions(pEspeces molecules, int ref)
 * \author Amine Ghozlane
 * \brief  Get number of reaction where one molecule is used (like a reactif)
 * \param  molecules List of molecules
 * \param  ref Number of a selected molecule
 * \return Number of reactions
 */
int Especes_getNbreactions(pEspeces molecules, int ref)
{
  int nbReactions=0;
  pReaction Q = molecules[ref].system;

  /* L'espece ne participe en tant que reactif a aucune reaction */
  if (Q == NULL) {
    return nbReactions;
  } else {
    /* Compte le nombre de reactions rattachees a une espece */
    while (Q!= NULL) {
      nbReactions++;
      Q = Q->suivant;
    }
  }
  return nbReactions;
}

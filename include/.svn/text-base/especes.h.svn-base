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
 * \file especes.h
 * \brief Modelize a molecule
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 27 octobre 2009
 */
#ifndef ESPECES_H_
#define ESPECES_H_

/* Especes */
/**
 * \struct Reaction
 * \brief Reactions data.
 */
typedef struct Reaction
{
  Reaction_t *link; /*!< Reaction id */
  double ratio; /* Reaction ratio */
  struct Reaction *suivant; /*!< Next reaction implicated for the molecule */
} Reaction, *pReaction;

/**
 * \struct Especes
 * \brief Molecules data
 */
typedef struct Especes {
  double quantite;      /*!< Molecule quantity */
  const char *id;       /*!< Molecule id */
  pReaction system;     /*!< Reaction where the molecule is implicated */
  int nbReactions;      /*!< Number of reactions where the molecule is involved*/
} Especes, *pEspeces;

/* Allocation memoire de la structure Espece et initialisation */
pEspeces Especes_alloc(int);

/* Allocation memoire de la structure Reaction et initialisation */
void Especes_allocReactions(pEspeces, int, Reaction_t *, double);

/* Libere la memoire alloue aux molecules */
void Especes_free(pEspeces, int);

/* Libere la memoire alloue aux reactions */
void Especes_freeReactions(Especes);

/* Enregistrement des donnees */
void Especes_save(pEspeces, int, double, const char *);

/* Enregistre le score des especes */
void Especes_scoreSpecies(pEspeces, int, char **, double *);

/* Affichage des infos et ratio des especes */
void Especes_print(pEspeces, int);

/* Affichage des infos des especes */
void Especes_print_2(pEspeces molecules, int nbEspeces);

/* Trouve une espece a partir de son ID */
int Especes_find(pEspeces, const char *, int);

/* Change la quantite d'une espece*/
void Especes_setQuantite(pEspeces, int, double);

/* Indique la quantite d'une espece*/
double Especes_getQuantite(pEspeces, int);

/* Determine le nombre de reaction auxquelles participe une espece (sous forme de reactif) */
int Especes_getNbreactions(pEspeces molecules, int ref);

#endif /* ESPECES_H_ */

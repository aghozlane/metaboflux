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
 * \file xml_parameter.h
 * \brief Xml reader for parametre.xml
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 15 janvier 2010
 */
#ifndef _XML_PARAMETER_H_
#define _XML_PARAMETER_H_

/**
* \struct xmlConfig_t
* \brief Structure containing all information from parameter.xml
*/
typedef struct {
    char *fichier; /*!< Name of the parameter file */
    xmlDocPtr doc; /*!< Xml doc */
    xmlNodePtr racine; /*!< Xml root */
    xmlXPathContextPtr ctxt; /*!< Xml ctxt */
} xmlConfig_t;

/* Initialisation et chargement du fichier XML en memoire */
xmlConfig_t *Xml_loadConfig(char *);

/* Liberation de la memoire pour la structure xmlConfig_t */
void Xml_freeConfig(xmlConfig_t *);

/* Recherche des valeurs entieres */
int Xml_getNumber(xmlConfig_t *, const char *);

/* Recherche des valeurs double */
double Xml_getDoubleNumber(xmlConfig_t *, const char *);

/* Recherche du texte */
char* Xml_getString(xmlConfig_t *);

/* Retourne le nombre de simulation a effectuer */
int Xml_getNbSimulations(xmlConfig_t *);

/* Retourne le nombre d'essai a effectuer pour les modelisations */
int Xml_getNbTriesMod(xmlConfig_t *);

/* Retourne le nombre d'essai a effectuer pour le recuit et la minimisation */
int Xml_getNbTriesSa(xmlConfig_t *);

/* Retourne le nombre d'Iteration a effectuer */
int Xml_getNbIters(xmlConfig_t *);

/* Retourne le nombre de groupe */
int Xml_getNbGroup(xmlConfig_t *);

/* Retourne la taille du pas */
double Xml_getStepSize(xmlConfig_t *);

/* Retourne la valeur de la constante de boltzmann */
double Xml_getBoltzmann(xmlConfig_t *);

/* Retourne la temperature initiale */
double Xml_getTinitial(xmlConfig_t *);

/* Retourne la variation de temperature */
double Xml_getMuT(xmlConfig_t *);

/* Retourne la temperature minimum */
double Xml_getTmin(xmlConfig_t *);

/* Retourne des decomptes sur l'objet xml */
int Xml_getCount(xmlConfig_t *, const char *);

/* Retourne le nombre de couples de parametres en jeu */
int Xml_getNbCouples(xmlConfig_t *);

/* Retourne le nombre de couples de parametres en jeu */
int Xml_getNbParameters(xmlConfig_t *);

/* Retourne le nombre de reaction dans un noeud */
int Xml_getNbReactioninNoeud(xmlConfig_t *, int);

/* Retourne le nombre de reaction*/
int Xml_getNbReaction(xmlConfig_t *);

/* Retourne le nombre de especes en jeu */
int Xml_getNbSpecies(xmlConfig_t *);

/* Retourne le nombre d'equation en jeu */
int Xml_getNbEquations(xmlConfig_t *);

/* Retourne le nombre de molecules bannies */
int Xml_getNbBannedSpecies(xmlConfig_t *);

/* Retourne la liste des reactions d'un noeud */
char **Xml_getReactionsNamesinNoeud(xmlConfig_t *, int);

/* Retourne la liste des reactions */
char **Xml_getReactionsNames(xmlConfig_t *);

/* Retourne la liste des especes */
char **Xml_getSpecies(xmlConfig_t *);

/* Retourne la liste des especes bannies */
char **Xml_getBanned(xmlConfig_t *conf);

/* Retourne la quantite final de l'espece */
int Xml_getSpeciesFinalAmount(xmlConfig_t *, char *);

/* Retourne le tableau contenant les quantites de toutes especes */
int *Xml_getallSpeciesFinalAmount(xmlConfig_t *, char **, int);

/* Retourne le poids de l'espece */
int Xml_getSpeciesWeight(xmlConfig_t *, char *);

/* Retourne le tableau contenant les poids de toutes especes */
int *Xml_getallSpeciesWeight(xmlConfig_t *, char **, int);

/* Alloue la memoire necessaire aux listes des equations */
char **Xml_allocEquation(char**, int);

/* Libere la memoire necessaire a la liste des equations */
/*void Xml_freeEquation(char***, int);*/

/* Retourne le nombre d'expressions mathematiques */
int Xml_getKineticLawNodes(xmlConfig_t *, int);

/* Retourn les expressions mathematiques */
char ***Xml_getKineticLaw(xmlConfig_t *, int, int);

#endif /* XML_PARAMETER_H_ */

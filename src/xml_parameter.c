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
 * \file xml_parameter.c
 * \brief Xml reader for parametre.xml
 * \author {Amine Ghozlane}
 * \version 2.0
 * \date 15 janvier 2010
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include "xml_parameter.h"

/**
 * \fn xmlConfig_t *Xml_loadConfig(char *fichier)
 * \author Amine Ghozlane
 * \brief  Initialize and load the parameter.xml
 * \param  fichier Xml file name
 * \return Struct xmlConfig_t
 */
xmlConfig_t *Xml_loadConfig(char *fichier)
{
  /* Initialisation et chargement du fichier XML en memoire */
  xmlConfig_t *conf=NULL;

  conf=(xmlConfig_t *)malloc(1*sizeof(xmlConfig_t));
  assert(conf!=NULL);

  /*Initialisation*/
  conf->fichier =NULL;
  conf->doc =NULL;
  conf->racine = NULL;
  conf->ctxt = NULL;

  conf->fichier = (char*)malloc(((int)strlen(fichier)+1)*sizeof(char));
  strcpy(conf->fichier,fichier);

  /* Creation de l'arbre DOM a partir du fichier XML */
  xmlKeepBlanksDefault(0);
  if ((conf->doc = xmlParseFile((const char*)conf->fichier)) == NULL) {
      fprintf(stderr,"Error creating of dom tree from \"%s\"\n",conf->fichier);
      Xml_freeConfig(conf);
      exit(EXIT_FAILURE);
      return NULL;
  }

  /* Recuperation de la racine */
  conf->racine = xmlDocGetRootElement(conf->doc);
  if (conf->racine != NULL && xmlStrcasecmp(conf->racine->name, xmlCharStrdup("parameters"))) {
      fprintf(stderr,"The file root is incorrect \"%s\", \"parameters\" is expected insteaded\n",conf->racine->name);
      Xml_freeConfig(conf);
      exit(EXIT_FAILURE);
      return NULL;
  }

  /* Initialisation de l'environnement XPath */
  xmlXPathInit();

  /* Creation d'un contexte pour les requetes XPath*/
  conf->ctxt = xmlXPathNewContext(conf->doc);
  if (conf->ctxt == NULL) {
      fprintf(stderr,"Error creating XPath context\n");
      Xml_freeConfig(conf);
      exit(EXIT_FAILURE);
      return NULL;
  }
  return conf;
}

/**
 * \fn void Xml_freeConfig(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Free the Struct xmlConfig_t
 * \param  conf Struct xmlConfig_t
 */
void Xml_freeConfig(xmlConfig_t *conf)
{
  /* Liberation de la memoire pour la structure xmlConfig_t */
  if(conf->ctxt!=NULL) xmlXPathFreeContext(conf->ctxt);
  if(conf->doc!=NULL) xmlFreeDoc(conf->doc);
  if(conf->fichier!=NULL) free(conf->fichier);
  if(conf!=NULL) free(conf);
  xmlCleanupParser();
}

/**
 * \fn int Xml_getNumber(xmlConfig_t *conf, const char *requete)
 * \author Amine Ghozlane
 * \brief  Get integer value
 * \param  conf Struct xmlConfig_t
 * \param  requete Xpath query
 * \return Read integer value in the xml
 */
int Xml_getNumber(xmlConfig_t *conf, const char *requete)
{
  /* Recherche des valeurs entieres */
  xmlXPathObjectPtr xpathRes;
  int result=0;

  /* Evaluation de l'expression XPath*/
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(requete), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"%s\"\n",requete);
      exit(EXIT_FAILURE);
  }

  /* Manipulation du resultat */
  if (xpathRes->type == XPATH_NUMBER) {
      result=(int)xmlXPathCastToNumber(xpathRes);
  }
  else{
      fprintf(stderr,"The program cannot get the number of simulations\n");
      exit(EXIT_FAILURE);
  }

  /* Libere la memoire alloue a l'objet xpath */
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);

  return result;
}

/**
 * \fn double Xml_getDoubleNumber(xmlConfig_t *conf, const char *requete)
 * \author Amine Ghozlane
 * \brief  Get double value
 * \param  conf Struct xmlConfig_t
 * \param  requete Xpath query
 * \return Read double value in the xml
 */
double Xml_getDoubleNumber(xmlConfig_t *conf, const char *requete)
{
  /* Recherche des valeurs double */
  xmlXPathObjectPtr xpathRes;
  double result=0;

  /* Evaluation de l'expression XPath*/
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(requete), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"%s\"\n",requete);
      exit(EXIT_FAILURE);
  }

  /* Manipulation du resultat */
  if (xpathRes->type == XPATH_NUMBER) {
      result=(double)xmlXPathCastToNumber(xpathRes);
  }
  else{
      fprintf(stderr,"The program cannot get the number asked by the XPath expression : \"%s\"\n",requete);
      exit(EXIT_FAILURE);
  }

  /* Libere la memoire alloue a l'objet xpath */
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);

  return result;
}

/**
 * \fn char* Xml_getString(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get name value
 * \param  conf Struct xmlConfig_t
 * \return Read name value in the xml
 */
char* Xml_getString(xmlConfig_t *conf)
    {
  /* Recherche du texte */
  xmlXPathObjectPtr xpathRes;
  char* result=NULL;

  /* Evaluation de l'expression XPath*/
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup("string(/parameters/name)"), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"string(/parameters/name)\"\n");
      exit(EXIT_FAILURE);
  }

  /* Manipulation du resultat */
  if (xpathRes->type == XPATH_STRING) {
      result=(char *)xmlXPathCastToString(xpathRes);
  }
  else{
      fprintf(stderr,"The program cannot get the name of the simulations\n");
      exit(EXIT_FAILURE);
  }

  /* Libere la memoire alloue a l'objet xpath */
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);

  return result;
    }

/**
 * \fn int Xml_getNbSimulations(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get number of simulation
 * \param  conf Struct xmlConfig_t
 * \return Number of simulation
 */
int Xml_getNbSimulations(xmlConfig_t *conf)
{
  /* Retourne le nombre de simulation a effectuer */
  return Xml_getNumber(conf,"number(/parameters/nbSimulations)");
}

/**
 * \fn int Xml_getNbTriesMod(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get number of tries for modelling
 * \param  conf Struct xmlConfig_t
 * \return Number of tries
 */
int Xml_getNbTriesMod(xmlConfig_t *conf)
{
  /* Retourne le nombre d'essai a effectuer */
  return Xml_getNumber(conf,"number(/parameters/nbtriesMod)");
}

/**
 * \fn int Xml_getNbTriesSa(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get number of tries for the simulated annealing and minimization
 * \param  conf Struct xmlConfig_t
 * \return Number of tries
 */
int Xml_getNbTriesSa(xmlConfig_t *conf)
{
  /* Retourne le nombre d'essai a effectuer */
  return Xml_getNumber(conf,"number(/parameters/nbtriesSa)");
}

/**
 * \fn int Xml_getNbIters(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get number of iteration
 * \param  conf Struct xmlConfig_t
 * \return Number of iteration
 */
int Xml_getNbIters(xmlConfig_t *conf)
{
  /* Retourne le nombre d'Iteration a effectuer */
  return Xml_getNumber(conf,"number(/parameters/nbiters)");
}

/**
 * \fn int Xml_getNbGroup(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get number of group
 * \param  conf Struct xmlConfig_t
 * \return Number of iteration
 */
int Xml_getNbGroup(xmlConfig_t *conf)
{
  /* Retourne le nombre d'Iteration a effectuer */
  return Xml_getNumber(conf,"number(/parameters/nbGroup)");
}

/**
 * \fn double Xml_getStepSize(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the step size
 * \param  conf Struct xmlConfig_t
 * \return Step size
 */
double Xml_getStepSize(xmlConfig_t *conf)
{
  /* Retourne la taille du pas */
  return Xml_getDoubleNumber(conf,"number(/parameters/stepsize)");
}

/**
 * \fn double Xml_getBoltzmann(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the Boltzmann value
 * \param  conf Struct xmlConfig_t
 * \return Boltzmann value
 */
double Xml_getBoltzmann(xmlConfig_t *conf)
{
  /* Retourne la valeur de la constante de boltzmann */
  return Xml_getDoubleNumber(conf,"number(/parameters/boltzmann)");
}

/**
 * \fn double Xml_getTinitial(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the initial temperature
 * \param  conf Struct xmlConfig_t
 * \return Initial temperature
 */
double Xml_getTinitial(xmlConfig_t *conf)
{
  /* Retourne la temperature initiale */
  return Xml_getDoubleNumber(conf,"number(/parameters/Tinitial)");
}

/**
 * \fn double Xml_getMuT(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the variation of temperature
 * \param  conf Struct xmlConfig_t
 * \return Variation of temperature
 */
double Xml_getMuT(xmlConfig_t *conf)
{
  /* Retourne la variation de temperature */
  return Xml_getDoubleNumber(conf,"number(/parameters/muT)");
}

/**
 * \fn double Xml_getTmin(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the minimum temperature
 * \param  conf Struct xmlConfig_t
 * \return Minimum temperature
 */
double Xml_getTmin(xmlConfig_t *conf)
{
  /* Retourne la temperature minimum */
  return Xml_getDoubleNumber(conf,"number(/parameters/Tmin)");
}

/**
 * \fn int Xml_getCount(xmlConfig_t *conf, const char *requete)
 * \author Amine Ghozlane
 * \brief  Get count
 * \param  conf Struct xmlConfig_t
 * \param  requete Xpath query
 * \return Count
 */
int Xml_getCount(xmlConfig_t *conf, const char *requete)
{
  xmlXPathObjectPtr xpathRes=NULL;
  int result=0;

  /* Evaluation de l'expression XPath*/
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(requete), conf->ctxt);

  /* Erreur expression xpath */
  if (xpathRes==NULL) {
      fprintf(stderr, "Error of the XPath expression : \"%s\"\n",requete);
      exit(EXIT_FAILURE);
  }

  /* Manipulation du resultat */
  if (xpathRes->type == XPATH_NUMBER) {
      result=(int)xmlXPathCastToNumber(xpathRes);
  }
  else{
      fprintf(stderr,"The program cannot get the count\n");
      exit(EXIT_FAILURE);
  }

  /* Libere la memoire alloue a l'objet xpath */
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);

  /* Retourne des decomptes sur l'objet xml */
  return result;

}

/**
 * \fn int Xml_getNbCouples(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the number of couples
 * \param  conf Struct xmlConfig_t
 * \return Number of couples
 */
int Xml_getNbCouples(xmlConfig_t *conf)
{
  /* Retourne le nombre de couples de parametres en jeu */
  return Xml_getCount(conf,"count(/parameters/listOfReactions/noeud)");
}

/**
 * \fn int Xml_getNbParameters(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the number of parameters
 * \param  conf Struct xmlConfig_t
 * \return Number of parameters
 */
int Xml_getNbParameters(xmlConfig_t *conf)
{
  /* Retourne le nombre de couples de parametres en jeu */
  return Xml_getCount(conf,"count(/parameters/listOfReactions/noeud/reaction)");
}

/**
 * \fn int Xml_getNbReactioninNoeud(xmlConfig_t *conf, int noeud)
 * \author Amine Ghozlane
 * \brief  Count the number of reaction in one node
 * \param  conf Struct xmlConfig_t
 * \param  noeud Number of the node
 * \return Number of reaction in one node
 */
int Xml_getNbReactioninNoeud(xmlConfig_t *conf, int noeud)
{
  char *ask=NULL;
  int result=0, temp=0;
  temp=asprintf(&ask,"count(/parameters/listOfReactions/noeud[@reference=\"%d\"]/reaction)",noeud);
  if(temp==-1){
      fprintf(stderr,"There is something wrong with asprintf\n");
      exit(EXIT_FAILURE);
  }

  result=Xml_getCount(conf,ask);
  if(ask!=NULL) free(ask);

  /* Retourne le nombre de reaction dans un noeud */
  return result;
}

/**
 * \fn int Xml_getNbReaction(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the number of reactions
 * \param  conf Struct xmlConfig_t
 * \return Number of reactions
 */
int Xml_getNbReaction(xmlConfig_t *conf)
{
  /* Retourne le nombre de reaction dans un noeud */
  return Xml_getCount(conf,"count(/parameters/listOfReactions/noeud/reaction)");
}

/**
 * \fn int Xml_getNbSpecies(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the number of species
 * \param  conf Struct xmlConfig_t
 * \return Number of species
 */
int Xml_getNbSpecies(xmlConfig_t *conf)
{
  /* Retourne le nombre de especes en jeu */
  return Xml_getCount(conf,"count(/parameters/listOfSpecies/species)");
}

/**
 * \fn int Xml_getNbEquations(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the number of equations
 * \param  conf Struct xmlConfig_t
 * \return Number of equations
 */
int Xml_getNbEquations(xmlConfig_t *conf)
{
  /* Retourne le nombre d'equation en jeu */
  return Xml_getCount(conf,"count(/parameters/listOfKineticlaw/math)");
}

/**
 * \fn int Xml_getNbBannedSpecies(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get the number of banned species
 * \param  conf Struct xmlConfig_t
 * \return Number of banned species
 */
int Xml_getNbBannedSpecies(xmlConfig_t *conf)
{
  /* Retourne le nombre de molecules bannies */
  return Xml_getCount(conf,"count(/parameters/listOfBannedCompounds/compounds)");
}

/**
 * \fn char **Xml_getReactionsNamesinNoeud(xmlConfig_t *conf, int numero)
 * \author Amine Ghozlane
 * \brief  Get list of reactions
 * \param  conf Struct xmlConfig_t
 * \param  numero Number of the node
 * \return List of reactions
 */
char **Xml_getReactionsNamesinNoeud(xmlConfig_t *conf, int numero)
{
  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL, cur_node = NULL;
  char **result=(char**)malloc(Xml_getNbReactioninNoeud(conf,numero)*sizeof(char*));
  char *reactions = NULL;
  int i=0;

  /* Evaluation de l'expression XPath */
  asprintf(&reactions,"/parameters/listOfReactions/noeud[@reference=\"%d\"]",numero);
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(reactions), conf->ctxt);
  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfReactions/noeud[@reference=\"%d\"\"\n",numero);
      exit(EXIT_FAILURE);
  }

  /* Traitement du resultat */
  if (xpathRes->type == XML_ELEMENT_NODE){
      arf=xpathRes->nodesetval->nodeTab[0];
      for (cur_node = arf->children; cur_node; cur_node = cur_node->next) {
          if (cur_node->type == XML_ELEMENT_NODE) {
              result[i]=(char*)malloc((strlen((char*)xmlGetNoNsProp(cur_node,xmlCharStrdup("id")))+1)*sizeof(char));
              strcpy(result[i],(char*)xmlGetNoNsProp(cur_node,xmlCharStrdup("id")));
              i++;
          }
      }
  }
  if(reactions!=NULL) free(reactions);
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);
  /* Retourne la liste des reactions d'un noeud */
  return result;
}

/**
 * \fn char **Xml_getReactionsNames(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get name of reactions
 * \param  conf Struct xmlConfig_t
 * \return Name of reactions
 */
char **Xml_getReactionsNames(xmlConfig_t *conf)
{
  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL, cur_node = NULL;
  int nbreaction=Xml_getNbCouples(conf),i=0,j;
  char **result=(char**)malloc(Xml_getNbReaction(conf)*sizeof(char*));
  char *reactions = NULL;

  for(j=0;j<nbreaction;j++){
      /* Evaluation de l'expression XPath */
      asprintf(&reactions,"/parameters/listOfReactions/noeud[@reference=\"%d\"]",j);
      xpathRes = xmlXPathEvalExpression(xmlCharStrdup(reactions), conf->ctxt);
      /* Erreur expression xpath */
      if (!xpathRes) {
          fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfReactions/noeud[@reference=\"%d\"\"\n",j);
          exit(EXIT_FAILURE);
      }

      /* Traitement du resultat */
      if (xpathRes->type == XML_ELEMENT_NODE){
          arf=xpathRes->nodesetval->nodeTab[0];
          for (cur_node = arf->children; cur_node; cur_node = cur_node->next) {
              if (cur_node->type == XML_ELEMENT_NODE) {
                  result[i]=(char*)malloc((strlen((char*)xmlGetNoNsProp(cur_node,xmlCharStrdup("id")))+1)*sizeof(char));
                  strcpy(result[i],(char*)xmlGetNoNsProp(cur_node,xmlCharStrdup("id")));
                  i++;
              }
          }
      }
  }
  if(reactions!=NULL) free(reactions);
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);

  /* Retourne la liste des reactions d'un noeud */
  return result;
}

/**
 * \fn char **Xml_getReactionsNamesinNoeud(xmlConfig_t *conf, int numero)
 * \author Amine Ghozlane
 * \brief  Get list of species
 * \param  conf Struct xmlConfig_t
 * \return List of species
 */
char **Xml_getSpecies(xmlConfig_t *conf)
{
  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL, cur_node = NULL;
  char **result=(char**)malloc(Xml_getNbSpecies(conf)*sizeof(char*));
  int i=0;

  /* Evaluation de l'expression XPath */
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup("/parameters/listOfSpecies"), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfSpecies\"\n");
      exit(EXIT_FAILURE);
  }

  /* Traitement du resultat */
  if (xpathRes->type == XML_ELEMENT_NODE){
      arf=xpathRes->nodesetval->nodeTab[0];
      for (cur_node = arf->children; cur_node; cur_node = cur_node->next) {
          if (cur_node->type == XML_ELEMENT_NODE) {
              result[i]=(char*)xmlGetNoNsProp(cur_node,xmlCharStrdup("id"));
              i++;
          }
      }
  }

  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);
  /* Retourne la liste des especes */
  return result;
}

/**
 * \fn char **Xml_getBanned(xmlConfig_t *conf)
 * \author Amine Ghozlane
 * \brief  Get list of banned species
 * \param  conf Struct xmlConfig_t
 * \return List of banned species
 */
char **Xml_getBanned(xmlConfig_t *conf)
{
  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL, cur_node = NULL;
  char **result=(char**)malloc(Xml_getNbBannedSpecies(conf)*sizeof(char*));
  int i=0;

  /* Evaluation de l'expression XPath */
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup("/parameters/listOfBannedCompounds"), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfBannedCompounds\"\n");
      exit(EXIT_FAILURE);
  }

  /* Traitement du resultat */
  if (xpathRes->type == XML_ELEMENT_NODE){
      arf=xpathRes->nodesetval->nodeTab[0];
      for (cur_node = arf->children; cur_node; cur_node = cur_node->next) {
          if (cur_node->type == XML_ELEMENT_NODE) {
              result[i]=(char*)xmlGetNoNsProp(cur_node,xmlCharStrdup("id"));
              i++;
          }
      }
  }

  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);
  /* Retourne la liste des especes bannies */
  return result;
}


/**
 * \fn int Xml_getSpeciesFinalAmount(xmlConfig_t *conf, char *species)
 * \author Amine Ghozlane
 * \brief  Get the final amount of one species
 * \param  conf Struct xmlConfig_t
 * \param  species Name of one species
 * \return Final amount of one species
 */
int Xml_getSpeciesFinalAmount(xmlConfig_t *conf, char *species)
{

  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL;
  char *ask=NULL,*reponse=NULL;

  int result=0;

  /* Evaluation de l'expression XPath */
  asprintf(&ask,"/parameters/listOfSpecies/species[@id=\"%s\"]",species);
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(ask), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfSpecies/species[@id=\"%s\"]\"\n",species);
      exit(EXIT_FAILURE);
  }

  /* Traitement du resultat */
  if (xpathRes->type == XML_ELEMENT_NODE){
      arf=xpathRes->nodesetval->nodeTab[0];
      reponse=(char*)xmlGetNoNsProp(arf,xmlCharStrdup("finalAmount"));
      if(reponse!=NULL) result=atoi(reponse);
      else result=-1;
  }

  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);
  if(ask!=NULL) free(ask);

  /* Retourne la quantite final de l'espece */
  return result;
}

/**
 * \fn int *Xml_getallSpeciesFinalAmount(xmlConfig_t *conf, char **species, int taille)
 * \author Amine Ghozlane
 * \brief  Get Table of species amount
 * \param  conf Struct xmlConfig_t
 * \param  species Table of species
 * \param  taille Number of species
 * \return Table of species amount
 */
int *Xml_getallSpeciesFinalAmount(xmlConfig_t *conf, char **species, int taille)
{
  int *result,i;
  result=(int*)malloc(taille*sizeof(int));
  for(i=0;i<taille;i++){
      result[i]= Xml_getSpeciesFinalAmount(conf, species[i]);
  }
  /* Retourne le tableau contenant les quantites de toutes especes */
  return result;
}

/**
 * \fn int Xml_getSpeciesWeight(xmlConfig_t *conf, char *species)
 * \author Amine Ghozlane
 * \brief  Get Species weight
 * \param  conf Struct xmlConfig_t
 * \param  species Name of one species
 * \return Species weight
 */
int Xml_getSpeciesWeight(xmlConfig_t *conf, char *species)
{

  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL;
  char *ask=NULL,*reponse=NULL;

  int result=0;

  /* Evaluation de l'expression XPath */
  asprintf(&ask,"/parameters/listOfSpecies/species[@id=\"%s\"]",species);
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(ask), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfSpecies/species[@id=\"%s\"]\"\n",species);
      exit(EXIT_FAILURE);
  }

  /* Traitement du resultat */
  if (xpathRes->type == XML_ELEMENT_NODE){
      arf=xpathRes->nodesetval->nodeTab[0];
      reponse=(char*)xmlGetNoNsProp(arf,xmlCharStrdup("weight"));
      if(reponse!=NULL) result=atoi(reponse);
      else result=-1;
  }

  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);
  if(ask!=NULL) free(ask);

  /* Retourne le poids statistique de la molecule */
  return result;
}

/**
 * \fn int *Xml_getallSpeciesWeight(xmlConfig_t *conf, char **species, int taille)
 * \author Amine Ghozlane
 * \brief  Get list of species weight
 * \param  conf Struct xmlConfig_t
 * \param  species Table of species
 * \param  taille Number of species
 * \return List of species weight
 */
int *Xml_getallSpeciesWeight(xmlConfig_t *conf, char **species, int taille)
{
  int *result,i;
  result=(int*)malloc(taille*sizeof(int));
  for(i=0;i<taille;i++){
      result[i]= Xml_getSpeciesWeight(conf, species[i]);
  }
  /* Retourne le poids statistique de toutes les molecules */
  return result;
}

/**
 * \fn char **Xml_allocEquation(char**equation, int nb_noeud)
 * \author Amine Ghozlane
 * \brief  Allocate memory for the list of equation
 * \param  equation list of equation
 * \param  nb_noeud Number of the node
 * \return List of reactions
 */
char **Xml_allocEquation(char **equation, int nb_noeud)
{
  int i;

  /* Allocation memoire */
  equation=(char**)malloc(nb_noeud*sizeof(char*));
  assert(equation!=NULL);

  /* initialisation */
  for(i=0;i<nb_noeud;i++){
      equation[i]=NULL;
  }
  /* Alloue la memoire necessaire aux listes des equations */
  return equation;
}

/**
 * \fn int Xml_getKineticLawNodes(xmlConfig_t *conf, int num_equation)
 * \author Amine Ghozlane
 * \brief  Get the kinetic law for one node
 * \param  conf Struct xmlConfig_t
 * \param  num_equation Number of the node
 * \return Ninetic law for one node
 */
int Xml_getKineticLawNodes(xmlConfig_t *conf, int num_equation)
{
  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL, cur_node=NULL;
  char *ask=NULL;
  int nb_noeud=0;

  /* Evaluation de l'expression XPath */
  asprintf(&ask,"/parameters/listOfKineticlaw/math[@display=\"block_%d\"]/mrow",num_equation);
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(ask), conf->ctxt);

  /* Erreur expression xpath */
  if (!xpathRes) {
      fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfKineticlaw/math[@display=\"block_%d\"]/mrow\"\n",num_equation);
      exit(EXIT_FAILURE);
  }
  /* Traitement du resultat */
  if (xpathRes->type == XML_ELEMENT_NODE){
      arf=xpathRes->nodesetval->nodeTab[0];
      /* Decompte du nombre de noeud */
      for (cur_node = arf->children; cur_node; cur_node = cur_node->next) {
          nb_noeud++;
      }
  }

  if(ask!=NULL) free(ask);
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);

  /* Retourne le nombre d'expressions mathematiques */
  return nb_noeud;
}

/**
 * \fn char ***Xml_getKineticLaw(xmlConfig_t *conf, int num_equation, int nb_noeud)
 * \author Amine Ghozlane
 * \brief  Get list of kinetic laws
 * \param  conf Struct xmlConfig_t
 * \param  num_equation Number of equation
 * \param  nb_noeud Number nodes
 * \return List of kinetic laws
 */
char ***Xml_getKineticLaw(xmlConfig_t *conf, int num_equation, int nb_noeud)
{
  xmlXPathObjectPtr xpathRes = NULL;
  xmlNodePtr arf = NULL, cur_node=NULL;
  char ***all=NULL, *ask=NULL;
  int i=0,j;

  /* Allocation de la matrice des resultats  */
  all=(char***)malloc(2*sizeof(char**));
  assert(all!=NULL);
  for(j=0;j<2;j++){
      /* type = 0  et name = 1 */
      all[j]=Xml_allocEquation(all[j],nb_noeud);
  }
  /* Evaluation de l'expression XPath */
  asprintf(&ask,"/parameters/listOfKineticlaw/math[@display=\"block_%d\"]/mrow",num_equation);
  xpathRes = xmlXPathEvalExpression(xmlCharStrdup(ask), conf->ctxt);

  /* Erreur expression xpath */
  if (xpathRes==NULL) {
      fprintf(stderr, "Error of the XPath expression : \"/parameters/listOfKineticlaw/math[@display=\"block_%d\"]/mrow\"\n",num_equation);
      exit(EXIT_FAILURE);
  }

  /* Traitement du resultat */
  if (xpathRes->type == XML_ELEMENT_NODE){
      arf=xpathRes->nodesetval->nodeTab[0];

      /* Enregistrement des noeuds */
      for (cur_node = arf->children; cur_node; cur_node = cur_node->next) {
          if(arf!=NULL){
              all[0][i]=(char*)malloc(((int)strlen((char*)cur_node->name)+1)*sizeof(char));
              strcpy(all[0][i], ((char*)cur_node->name));

              all[1][i]=(char*)malloc(((int)strlen(((char*)xmlXPathCastNodeToString(cur_node)))+1)*sizeof(char));
              strcpy(all[1][i], ((char*)xmlXPathCastNodeToString(cur_node)));

              i++;
          }
      }
  }
  if(xpathRes!=NULL) xmlXPathFreeObject(xpathRes);
  if(ask!=NULL)free(ask);

  /* Retourne les expressions mathematiques */
  return all;
}

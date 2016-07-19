#!/usr/bin/python
# -*- coding: utf-8 -*-
###############################################################################
# This file is part of MetaBoFlux (http://www.cbib.u-bordeaux2.fr/metaboflux/)#
# Copyright (C) 2010 Amine Ghozlane from LaBRI and University of Bordeaux 1   #
#                                                                             #
# MetaBoFlux is free software: you can redistribute it and/or modify          #
# it under the terms of the GNU Lesser General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or           #
# (at your option) any later version.                                         #
#                                                                             #
# MetaBoFlux is distributed in the hope that it will be useful,               #
# but WITHOUT ANY WARRANTY; without even the implied warranty of              #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               #
# GNU General Public License for more details.                                #
#                                                                             #
# You should have received a copy of the GNU Lesser General Public License    #
# along with this program.  If not, see <http://www.gnu.org/licenses/>.       #
###############################################################################
"""
 @brief: Get files
 @author: Amine Ghozlane
 @version: 2.0 
"""
import sys, getopt, os

#==============================================================
#Ouverture des fichiers
#==============================================================
#Detail l'usage de Metaboflux
def usage(info):
    """
     Give information to use Metaboflux
     @param info: Error texte
     @return: Use of Metaboflux
    """
    text = ""
    if(info):
        text += info
    temp = "Option\t\tCase\t\tFilename\tType\tDescription\n"
    text += temp
    for i in range(len(temp) + 40):
        text += '-'
    text += '\n'
    text += "-s\t\tno\t\tno\t\tno\tSimulated annealing\n"
    text += "-m\t\tno\t\tno\t\tno\tMinimization\n"
    text += "-r\t\tno\t\tno\t\tno\tStatistic representation\n"
    text += "-t\t\t-s -t\t\tno\t\tno\tNumber of thread to launch\n"
    text += "-p\t\t-s -t\t\tno\t\tOut\tConnection port\n"
    text += "--debug\t\t-s -m\t\tno\t\tOutput\tDebug file output\n"
    text += "--sbml\t\t-s -m -r\tsbml.xml\tInput\tSBML file\n"
    text += "--par\t\t-s -m -r\tparameter.xml\tInput\tParameter file at MetaBoflux format\n"
    text += "--sim_out\t-s -m\t\tRepertory/\tIn/Out\tSimulation out repertory\n"
    text += "--min_out\t-m -r\t\tRepertory/\tIn/Out\tMinimization out file\n"
    text += "--stat_out\t-r\t\tRepertory/\tOutput\tStatistic out repertory\n"
    return text

#Determine les fichiers fournis en arguments
def getfiles():
    """
    Determine the files provided as arguments
    @return: Choosen options
    """
    #Sans argument
    if len(sys.argv) <= 1:
        sys.exit("Do './MetaBoFlux.py -h' for a usage summary")
    #test
    try:
        (opts, args) = getopt.getopt(sys.argv[1:], "smrht:p:", ["sbml=", "par=", "help", "sim_out=", "min_out=", "stat_out=", "debug"])
    except getopt.GetoptError, err:
        # print help information and exit:
        print str(err) # will print something like "option -a not recognized"
        sys.exit(usage(None))
    
    sbml = None
    par = None
    sim_out = None
    min_out = None
    stat_out = None
    simulation = False
    minimization = False
    stat = False
    thread = None
    port = 0
    debug = 0
    temp = "Metaboflux chosen activity : "
    #Identification of options
    for (o, a) in opts:
        if o == "-s":
            simulation = True
            temp += 'Simulated annealing\t'
        elif o == "-m":
            minimization = True
            temp += 'Minimization\t'
        elif o == "-r":
            stat = True
            temp += 'Statistics\t'
        elif o == "-t":
            try:
             thread = int(a)
            except:
                sys.exit(usage("Option \"t\" must be followed by the number of thread\n"))
        elif o == "-p":
            try:
                port = int(a)
            except:
                sys.exit(usage("Option \"p\" must be followed by the number of port\n"))
        elif o in ("-h", "--help"):
            sys.exit(usage(None))
        elif o in ("--sbml"):
            sbml = a
        elif o in ("--par"):
            par = a
        elif o in ("--sim_out"):
            sim_out = a
        elif o in ("--min_out"):
            min_out = a
        elif o in ("--stat_out"):
            stat_out = a
        elif o in ("--debug"):
            temp += 'Mode debug on\t'
            debug = 1
        else:
            assert False, "unhandled option"
    if(simulation or minimization or stat):
        print temp
    return simulation, minimization, stat, thread, port, sbml, par, sim_out, min_out, stat_out, debug

#Test if necessary document are available
def case(simulation, minimization, stat, sbml, par, sim_out, min_out, stat_out):
  """
  Test if necessary document are available
  @param simulation: Simulation called
  @param minimization: Minimization called
  @param stat: Statistic called
  @param sbml: SBML file
  @param par: Parameter file
  @param sim_out: Simulation repertory
  @param min_out: Minimization repertory
  @param stat_out: Statistic repertory
  @param debug: Debug called
  """
  if(simulation==False and minimization==False and stat==False):
      sys.exit(usage("You must indicate an activity\n"))
      
  #Test du fichier de parametre
  if(par == None):
    sys.exit(usage("parameter.xml is missing\n"))
    
  #Test des fichiers sbml.xml et du repertoire de simulation
  if(simulation or minimization):
     if sbml == None:
         sys.exit(usage("sbml file : \"%s\" must be indicated\n" % (sbml)))
     elif sim_out == None:
         sys.exit(usage("Simulated annealing repertory (sim_out) : \"%s\" must be indicated\n" % (sim_out)))
  #Test du repertoire de minimisation
  if(minimization and min_out == None):
    sys.exit(usage("Minimization output repertory (min_out) : \"%s\" must be indicated\n" % (min_out)))
  #Test du repertoire statistique
  if(stat):
      if(not sbml):
        sys.exit(usage("sbml file : \"%s\" must be indicated\n" % (sbml)))
      if(not stat_out):
          sys.exit(usage("Statistic output repertory (stat_out) and  : \"%s\"  must be indicated\n" % (stat_out)))
      if(not min_out):
          sys.exit(usage("Minimization output repertory (min_out) : \"%s\"  must be indicated\n" % (min_out)))
#Test their format
def data_format(par, sbml, sim_out, min_out, stat_out):
    """
    Check if information are correct
    """
    #Test des fichiers et repertoires
    if(par and not os.path.isfile(par)):
        #test si c'est bien un fichier
        sys.exit(usage("Error with %s, --par required a parameter file\n"%par))
    if(sbml and not os.path.isfile(sbml)):
        #test si c'est bien un fichier
        sys.exit(usage("Error with %s, --par required a sbml file\n"%sbml))
    #Test des repertoires donnes
    if(sim_out and not os.path.isdir(sim_out)):
        sys.exit(usage("Error with %s, --sim_out required a path to a repertory\n"%sim_out))
    if(min_out and not os.path.isdir(min_out)):
        sys.exit(usage("Error with %s, --min_out required a path to a repertory\n"%min_out))
    if(stat_out and not os.path.isdir(stat_out)):
        sys.exit(usage("Error with %s, --stat_out required a path to a repertory\n"%stat_out))

#Definition des fichiers de sortie
def fileName(simulationname, min_out, stat_out):
  """
  Definition of output files
  @param simulationname: Name of the simulation
  @param min_out: Minimization repertory
  @param stat_out: Statistic repertory
  @return: Metaboflux files output 
  """
  valuefilename = ""
  modeloutfilename = ""
  modellogoutfilename = ""
  sdmodeloutfilename = ""
  minioutfilename = ""
  logoutfilename = ""
  sdoutfilename = ""
  groupinfilename = ""
  groupoutfilename = ""
  grouplogoutfilename = ""
  valuegroupfilename = ""
  ratiofilename=""
  
  if(min_out != None):
    valuefilename = min_out + simulationname + "_compilation.txt"
    valuegroupfilename = min_out + simulationname + "_compilation_group.txt"
    modeloutfilename = min_out + simulationname + "_modeling.txt"
    modellogoutfilename = min_out + simulationname + "_modeling_log.txt"
    minioutfilename = min_out + simulationname + "_minimization.txt"
    logoutfilename = min_out + simulationname + "_minimization_log.txt"
    sdoutfilename = min_out + simulationname + "_sd.txt"
    groupoutfilename = min_out + simulationname + "_group_modeling.txt"
    grouplogoutfilename = min_out + simulationname + "_group_log.txt"
    ratiofilename=min_out + simulationname + "_ratio.txt"
    
  if(stat_out != None):
    statoutfilename = stat_out + simulationname
    groupinfilename = stat_out + simulationname + "_mean_proportion.txt"
    
  #simuloutfilename,
  return valuefilename, valuegroupfilename, modeloutfilename, modellogoutfilename,  minioutfilename, logoutfilename, sdoutfilename, groupinfilename, groupoutfilename, grouplogoutfilename,ratiofilename

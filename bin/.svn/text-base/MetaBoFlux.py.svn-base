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
import sys
import os
import xml.dom.minidom
#from time import gmtime
sys.path.append(os.path.join(os.path.dirname(__file__),"..%slib%s"%(os.sep,os.sep)))
from libXml import *
from libGetfiles import *
from libAnalysis import *
from libCompute import *


#==============================================================
#Programme principal
#==============================================================
def main():
    """
    @brief: Programme  de gestion du coeur
    @author: Amine Ghozlane
    """
    #test de temps d'execution
    #a=gmtime()
    
    s = None
    #Lecture du fichier de parametre
    sa, min, stat, thread, port, sbml, par, sim_out, min_out, stat_out, debug = getfiles()
    case(sa, min, stat, sbml, par, sim_out, min_out, stat_out)
        
    #Lecture des donnees issues du fichier de parametre xml
    print "Read parameters"
    try:
        dom = xml.dom.minidom.parse(par)
    except:
         sys.exit("Read problem with %s"%par)   
    simulationname, nb_simulation, nb_group, parameters, outputparameters, nb_parameters, nb_outputparameters, parameters_initialQuantity = extractParameters(dom)
    #Definition des fichiers de sortie
    valuefilename, valuegroupfilename, modeloutfilename, modellogoutfilename,  minioutfilename, logoutfilename, sdoutfilename, groupinfilename, groupoutfilename, grouplogoutfilename,ratiofilename = fileName(simulationname, min_out, stat_out)
    #Determination du nombre de thread a lancer et Initialisateur du serveur de calcul
    if(thread == None):
        thread = (detectCPUs() + 1)
      
    #Simulation
    if(sa):
      #Connexion a l'interface
      if(port): s = connectInterface(port)
      #Calcul du recuit simule
      print "Parameter estimation by simulated annealing ...."
      if(s): s.send("Simulated annealing")
      simulated_annealing(thread, sbml, par, sim_out, debug, port)
      print "End of estimation" 
      #Deconnexion de l'interface
      if(s): disconnectInterface(s)
     
    #minimization
    if(min):
      #Connexion a l'interface
      if(port): s = connectInterface(port)      
      #Extraction des donnees de simulation
      extractValue(valuefilename, sim_out, simulationname, nb_simulation, nb_parameters)      
      #Creation du ratio file
      refreshRatioFile(valuefilename,ratiofilename,False,0,0)
      
      #Extraction des donnes de modelisation et minimisation
      print "Minimization ...."
      if(s): s.send("Minimization")
      preparationOutfile(minioutfilename, outputparameters, parameters, nb_parameters, parameters_initialQuantity, False)
      nbspecies = preparationOutfile3(logoutfilename, sbml, parameters, False)
      minimization(minioutfilename, logoutfilename,ratiofilename, nb_simulation, thread, sbml, par, min_out, debug, port)
      print "End of minimization"
      
      #Reorganisation des donnees de minimisation
      print "Minimization log : result classification ...."
      tab = classificationValues(logoutfilename, nbspecies, nb_parameters,False)
      preparationOutfile3(logoutfilename, sbml, parameters, False)
      saveclassifiedValues(tab, logoutfilename,False)
      print "End of Classification"      
      
      #Reorganisation des donnees de minimisation
      print "Minimization : result classification ...."
      tab = classificationValues(minioutfilename, nb_outputparameters, nb_parameters, False)
      preparationOutfile(minioutfilename, outputparameters, parameters, nb_parameters, parameters_initialQuantity, False)
      saveclassifiedValues(tab, minioutfilename,False)
      print "End of Classification"
      
      #Creation du ratio file
      refreshRatioFile(minioutfilename,ratiofilename,True,nb_outputparameters,nb_parameters)

      #Extraction des donnees de recuit et simulation par simple modelisation
      print "Modeling test ...."
      if(s): s.send("Modeling")
      preparationOutfile3(modellogoutfilename, sbml, parameters,False)
      preparationOutfile(modeloutfilename, outputparameters, parameters, nb_parameters, parameters_initialQuantity,False)
      modeling(modeloutfilename, modellogoutfilename, ratiofilename, nb_simulation, thread, sbml, par, min_out, debug, port, False)
      print "End of modeling"
      
      #Reorganisation des donnees de modelisation
      print "Modeling : result classification...."
      tab = classificationValues(modeloutfilename, nb_outputparameters, nb_parameters,False)
      preparationOutfile(modeloutfilename, outputparameters, parameters, nb_parameters, parameters_initialQuantity,False)
      saveclassifiedValues(tab, modeloutfilename,False)
      print "End of classification"
      
            #Reorganisation des donnees de modelisation
      print "Modeling log: result classification...."
      tab = classificationValues(modellogoutfilename, nbspecies, nb_parameters,False)
      preparationOutfile3(modellogoutfilename, sbml, parameters, False)
      saveclassifiedValues(tab, modellogoutfilename,False)
      print "End of classification"
      
      #Estimation de l'ecart type
      print "Estimation of the standard deviation ...."
      if(s): s.send("Standard deviation estimation")
      preparationOutfile2(sdoutfilename)
      standard_deviation(sdoutfilename, ratiofilename, nb_simulation, thread, sbml, par, min_out, debug, port)
      print "End of Estimation"      
      
      #Deconnexion de l'interface
      if(s): disconnectInterface(s)
    
    #Statistiques
    if(stat):
      #Connexion a l'interface
      if(port): s = connectInterface(port)
      
      #Extraction des donnees de recuit et simulation par simple modelisation
      print("Statistics")
      statistics(par, modeloutfilename, sdoutfilename, stat_out)
      print("End of Statistics")
      
      #Modelisation des groupes
      if(s): s.send("Group analysis")
      print("Group analysis")
      preparationOutfile(groupoutfilename, outputparameters, parameters, nb_parameters, parameters_initialQuantity, True)
      nbspecies = preparationOutfile3(grouplogoutfilename, sbml, parameters,True)
      groupValues(groupinfilename, nb_parameters, valuegroupfilename)
      #Creation du ratio file
      refreshRatioFile(valuegroupfilename,ratiofilename,False,0,0)
      #Modelisation des solutions des groupes
      modeling(groupoutfilename, grouplogoutfilename,ratiofilename, nb_group, thread, sbml, par, min_out, debug, port, True)
      print "End of Group Analysis"
      
      #Reorganisation des donnees de modelisation des groupes
      print "GroupLog : result classification ...."
      tab = classificationValues(grouplogoutfilename, nbspecies, nb_parameters,True)
      preparationOutfile3(grouplogoutfilename, sbml, parameters, True)
      saveclassifiedValues(tab, grouplogoutfilename,True)
      print "End of Classification"  
      
    #test de temps d'execution
    #b=gmtime()
    #start=a.tm_sec+a.tm_min*60+a.tm_hour*3600
    #end=b.tm_sec+b.tm_min*60+b.tm_hour*3600
    #inputfile = open("/home/amine/workspace/MetaboFlux/metaboflux/timetest.txt", "a")
    #inputfile.write("%f\n"%(end-start))
    #inputfile.close()
if __name__ == "__main__":
    main()

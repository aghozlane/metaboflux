#! /usr/bin/Rscript --vanilla 
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

library(gplots)
library(XML)

#==============================================================
#Traitement des Arguments
#==============================================================
#Lecture de l'argument en entree du programme
input=function(){
	x=commandArgs(trail=T)
	if(length(x)!=4){
		print("Usage : ./MetaBoFlux_stat.R parameter.xml minimization_file.txt sd_file.txt out_repertory/")
		q("yes",status=1)
	}
	return(x)
}

#==============================================================
#Fichier xml
#==============================================================
#Retourne la valeur d'un noeud
getValue=function(conf, xpath){
  return (sapply(getNodeSet(doc=conf,path=xpath),xmlValue))
}

#Retourne un comptage
getCount=function(conf, xpath){
  return (length(getNodeSet(doc=conf,path=xpath)))
}

#Retourne les attributs
getAttribute=function(conf, xpath){
  return (sapply(getNodeSet(doc=conf,path=xpath),xmlAttrs))
}

#Retourne le nom
getName=function(conf){
  return (getValue(conf, "/parameters/name"))
}

#Retourne le nombre de groupe a realiser
getGroup=function(conf){
  return (as.integer(getValue(conf, "/parameters/nbGroup")))
}

#Nombre de simulation a considerer
getGrouplimit=function(conf){
  return (as.integer(getValue(conf, "/parameters/groupSize")))
}

#Retourne le nombre 
getNbSimulations=function(conf){
  return (as.integer(getValue(conf, "/parameters/nbSimulations")))
}

#Retourne le nombre d'espece
getInterestEnergy=function(conf){
   return (as.integer(getValue(conf, "/parameters/interestEnergy")))
}

#Retourne le nombre de graph
getNbGraph=function(conf){
  return (getCount(conf,"/parameters/representations/graph"))
}

#Retourne le nombre de transition
getNbTransition=function(conf){
  return (getCount(conf, "/parameters/listOfReactions/noeud/reaction"))
}

#Retourne le nombre de couples
getNbCouples=function(conf){
  return (getCount(conf, "/parameters/listOfReactions/noeud"))
}

#Retourne le nombre d'espece
getNbSpecies=function(conf){
  return (getCount(conf, "/parameters/listOfSpecies/species"))
}

#Retourne les transitions en jeu
getTransition=function(conf){
  return (getAttribute(conf, "/parameters/listOfReactions/noeud/reaction"))
}

#Retourne les especes en jeu
getSpecies=function(conf){
  return (getAttribute(conf, "/parameters/listOfSpecies/species"))
}

#Retourne les parametres
getParameters=function(conf,nb_couples){
  parameters=c()
  for(i in seq(0,nb_couples-1)){
    node=paste("/parameters/listOfReactions/noeud[@reference=",toString(i),"]/reaction",sep="")
    parameters=cbind(parameters,getCount(conf,node))
  }
  return(parameters)
}

getRepresentation=function(conf,nb_graph){
  parameters=c()
  for(i in seq(0,nb_graph-1)){
    node=paste("/parameters/representations/graph[@reference=",toString(i),"]",sep="")
    parameters=cbind(parameters,getAttribute(conf, node))
  }
  return(parameters)
}

#==============================================================
#Traitement statistique
#==============================================================
#Determine la deviation standard
sd_stat=function(sd_data,representation,output){
  png(output,height=as.integer(representation[6]),width=as.integer(representation[7]))
  barplot(unique(sd_data[,1-3])[,2],main=representation[2],names.arg=unique(sd_data[,1]),sub=representation[3],col="black",xlab=representation[4],ylab=representation[5],col.sub = "red")
  dev.off()
}

#Determine l'etat des balances
check_stat=function(donnees,debut, fin,Interest_energy,nb_simulations,representation,output){
	#text=paste("Perform by Glycoflux")
	#Selection les "nb" scenarios de meilleure energie
	ratio12=donnees[1:(Interest_energy+1),]

	#Donnees intermediaires
	energie=ratio12[,1]
	energie[1]="Expected"
	A=gl((Interest_energy+1),1,(Interest_energy+1),labels=paste(energie,"\n",seq(0,(length(energie)-1))))

	#Gestion de la palette de couleur
	if(fin>=2){
		palette(rainbow(fin))
	}
	#Affichage
	png(output,height=as.integer(representation[6]),width=as.integer(representation[7]))
	split.screen(fig =rbind(c(0, 1, 0.40, 1),c(0, 1, 0, 0.5)))
	screen(2)
	plot(c(1,5),c(1,5),ty="n",xaxt="n",yaxt="n",xlab="",ylab="",col="white",col.lab ="white",fg="white")
	names=colnames(ratio12)
	legend(3,3,legend=names[debut:(fin+1)],fill=c(1:(fin)),xjust=0.5,yjust=0.5,
	 border="white",ncol=3,pt.lwd="n")
	#mtext(text,side=3)
	screen(1)
	#print(rbind(levels(A),seq())
	barplot(t(ratio12[,debut:(fin+1)]),beside=T,col=c(1:(fin)),names.arg=levels(A),xlab=representation[4],ylab=representation[5])
	
	#main=simulation_name
	title(main=representation[2], sub=representation[3], col.sub = "red")
	close.screen(all = T) 
	dev.off()
}

#Determine les groupements
group_define=function(donnees,debut,fin,group,output,output1,representation,representation1){
	#Determination de la distance euclidienne
	distances=dist(donnees[,debut:(fin+1)])
	hierar_tree=hclust( distances, method="complete")

	#Regroupement des lignes
	png(output,height=as.integer(representation[6]),width=as.integer(representation[7]))
	plot(hierar_tree,main=representation[2],sub=representation[3],xlab=representation[4],ylab=representation[5])
	x=rect.hclust(hierar_tree,group,border="red")
	dev.off()
	
	#Regroupement des energies
	png(output1,height=as.integer(representation1[6]),width=as.integer(representation1[7]))
	plot(hierar_tree,main=representation1[2],sub=representation1[3],xlab=representation1[4],ylab=representation1[5],labels=format(donnees[,1],digits=3))
	rect.hclust(hierar_tree,group,border="red")
	dev.off()

	#retourne les groupes
	return (x)
}

#Analyse du tableau de donnees
tab_analysis=function( data,  x, debut, fin, i){
	temp=c()
	for(a in seq(0,(fin-1))){
		temp=cbind(temp,mean(data[x[[i]],(debut+a)]))
	}
	return ( temp)
}

#Determine les statistiques des groupes	
group_analysis=function(data,x,nb_parametre,nb_couples,parameters,group,nb_especes,total,output){
	tab=c()
	name_group=c()
	moyenne=c()

	#Analyse des statistiques
	for(i in seq(1,group)){
		temp=c()
		debut=nb_especes+2
		for(j in seq(1,nb_couples)){
			temp=cbind(temp,tab_analysis(data,  x, debut, parameters[j], i))
			debut=debut+parameters[j]			
		}
		tab=rbind(tab,temp)
		name_group=cbind(name_group,paste("H",toString(i),sep=""))
		moyenne=rbind(moyenne,mean(data[x[[i]],1]))
	}

	#Ecriture des donnees
	colnames(tab)=colnames(data[,(nb_especes+2):(total+1)])
	rownames(tab)=name_group
	colnames(moyenne)="Energy"

	#ecriture des valeurs de groupes
	output=paste(sortie,name,"_mean_proportion.txt",sep="")
	temp=cbind(moyenne,tab)
	write.table(format(temp,digits=3), file =output,append = FALSE, sep="\t", quote=F)
	
	#retourne les moyennes
	return(tab)
}

#Calcul de la limite superieure
tab_limit_add=function(ratio14, data,  x, debut2, debut, fin, i){
	temp=c()
	for(a in seq(0,(fin-1))){
		value=ratio14[i,(debut2+a)]+sd(data[x[[i]],(debut+a)])
		if(is.na(value)){
		    value=ratio14[i,(debut2+a)]
		}
		temp=cbind(temp,value)
	}
	return (temp)
}

#Calcul de la limite inferieure
tab_limit_sub=function(ratio14, data,  x, debut2, debut, fin, i){
	temp=c()
	for(a in seq(0,(fin-1))){
		value=ratio14[i,(debut2+a)]-sd(data[x[[i]],(debut+a)])
		if(is.na(value)){
		    value=ratio14[i,(debut2+a)]
		}
		temp=cbind(temp,value)
	}
	return (temp)
}


#Determine les limites pour les parametres
group_limit=function(data,x,ratio14,nb_parametre,nb_couples,parameters,group,nb_especes,representation,output){
	limsup=c()
	liminf=c()
	name_group=c()
	for(i in seq(1,group)){
		temp=c()
		temp_2=c()
		begin=nb_especes+2
		begin2=1
		for(j in seq(1,nb_couples)){
			temp=cbind(temp,tab_limit_add(ratio14, data,  x, begin2, begin, parameters[j], i))
			temp_2=cbind(temp_2,tab_limit_sub(ratio14, data,  x, begin2, begin, parameters[j], i))
			begin=begin+parameters[j]
			begin2=begin2+parameters[j]
		}
		limsup=rbind(limsup,temp)
		liminf=rbind(liminf,temp_2)
		name_group=cbind(name_group,paste("H",toString(i),"\nE_mean : ",format(mean(data[x[[i]],1]),digits=3),sep=""))
	}
	
	rownames(ratio14)=name_group
	if(length(ratio14[1,])>=2){
		palette(rainbow(length(ratio14[1,])))
	}

	#Affichage des donnees de groupes
	png(output,height=as.integer(representation[6]),width=as.integer(representation[7]))
	split.screen(fig =rbind(c(0, 1, 0.40, 1),c(0, 1, 0, 0.5)))
	
	#screen legend
	screen(2)
	plot(c(1,5),c(1,5),ty="n",xaxt="n",yaxt="n",xlab="",ylab="",col="white",col.lab ="white",fg="white")
	legend(3,3,legend=colnames(ratio14),fill=c(1:length(ratio14)),xjust=0.5,yjust=0.5,border="white",ncol=3,pt.lwd="n")

	#screen barplot
	screen(1)
	couleurs=c(1:length(ratio14[1,]))
	barplot2(t(ratio14),beside=T,col=couleurs,plot.ci = TRUE,ci.l = t(liminf), ci.u = t(limsup),xlab=representation[4],ylab=representation[5])
	
	#main=simulation_name
	title(main=representation[2], sub=representation[3], col.sub = "red")
	close.screen(all = T) 
	dev.off()
}

#==============================================================
#Programme principal
#==============================================================

#Lecture de l'argument en entree du programme
x=input()

#Lecture du fichier de parametre
conf=xmlParseDoc(x[1])

#Initialisation des parametres
entree=x[2]
sd_file=x[3]
sortie=x[4]
#width=getWidth(conf)
#height=getHeight(conf)
name=getName(conf)
group=getGroup(conf)
limit=getGrouplimit(conf)
nb_transition=getNbTransition(conf)
nb_couples=getNbCouples(conf)
nb_species=getNbSpecies(conf)
transition=getTransition(conf)
species=getSpecies(conf)
nb_simulations=getNbSimulations(conf)
total=nb_transition+nb_species
parameters=getParameters(conf,nb_couples)
interest_energy=getInterestEnergy(conf)
nb_graph=getNbGraph(conf)

representation=getRepresentation(conf,nb_graph)

#print(paste("nbsimulation=",nb_simulations))
if(interest_energy>nb_simulations){
	    interest_energy=nb_simulations
}

#Lecture du fichier de donnees
donnees=as.matrix(read.table(entree,header=T))
sd_data=as.matrix(read.table(sd_file,header=T))
sd_data=sd_data[sort(sd_data[,1],index.return=T)$ix,]

print("Estimation of the Standard deviation")
output=paste(sortie,name,"_standard_deviation.png",sep="")
sd_stat(sd_data,representation[,1],output)

#Determine l'etat des balances
print("Metabolites studied ...")
output=paste(sortie,name,"_product.png",sep="")
check_stat(donnees,2,nb_species,interest_energy,nb_simulations,representation[,2],output)

#Selection des donnes d'interet
if(limit>nb_simulations){
  limit=nb_simulations
}
donnees=donnees[2:(1+limit),]

#Correction du nombre de groupes
if(group>=nb_simulations){
  group=nb_simulations-1
}

#Correction du nombre de groupes
if(group>=limit){
  group=limit-1
}
 
if(nb_simulations>=3){
  #Determine les groupements
  print("Groupment evaluation ...")
  output=paste(sortie,name,"_hclust_complete.png",sep="")
  output1=paste(sortie,name,"_hclust_complete_energy.png",sep="")
  debut=nb_species+2
  groupes=group_define(donnees,debut,total,group,output,output1,representation[,3],representation[,4])

  #Determine les statistiques des groupes
  print("Statistics of groups ...")
  output=paste(sortie,name,"_mean_proportion.txt",sep="")
  statgroup=group_analysis(donnees,groupes,nb_parametre,nb_couples,parameters,group,nb_species,total,output)

  #Determine les limites
  print("Groups limit ...")
  output=paste(sortie,name,"_reaction.png",sep="")
  main_title=paste(name," : Reactions ratio representation")
  sub_title=paste(toString(interest_energy)," bests energies - ",toString(nb_simulations)," simulations")
  ytext="Reactions ratio (%)"
  group_limit(donnees,groupes,statgroup,nb_parametre,nb_couples,parameters,group,nb_species,representation[,5],output)
}
#liberation de la structure du fichier xml
free(conf)


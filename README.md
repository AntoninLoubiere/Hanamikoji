# Compétition Hanamikoji façon Prologin

### Introduction

Chaque participant conçoit un programme capable de jouer au jeu *Hanamikoji*, ensuite un tournoi est organisé entre ces programmes.

Pour exécuter votre code, en local il faudra télécharger stechec (cf [Utilisation](#utilisation)) sinon vous pouvez le site (cf [Utilisation Serveur](#utilisation-serveur)).

## Le jeu, Hanamikoji

### Description générale

Le jeu est composé de 7 *Geisha* et de 21 cartes représentant chacune une Geisha. Il y a 3 Geisha de valeur 2, 2 Geisha de valeur 3, 1 Geisha de valeur 4 et 1 Geisha de valeur 5. La valeur d'une Geisha est aussi le nombre de cartes de cette Geisha disponible (2 cartes pour la Geisha 0, 2 pour la 1, 5 pour la 6…).

Le jeu se joue en au plus 3 manches (cf [conditions de victoires](#conditions-de-victoires)) durant lesquels chaque joueur jouera une et une seule fois chacune des 4 quatre actions (cf [actions](#actions)), qui permettent de valider des cartes Geisha. À la fin de chaque manche, pour chaque Geisha, si un joueur a strictement plus de cartes Geisha validé que l'adversaire, il prend possession de celle-ci (même si l'adversaire la possédait déjà depuis la manche précédente; au départ les Geisha ne sont possédés par aucun des joueurs).

### Actions

##### Valider une carte
Vous valider secrètement une carte en main

##### Défausser une carte
Vous défausser secrètement deux de vos cartes en main, elles sont désormais exclus de la manche.

##### Choix trois cartes
Vous choisissez trois cartes de votre main que vous présentez à l'adversaire, celui-ci en choisi une qu'il valide, tandis que vous validez les deux autres.

##### Choix paquets
Vous présentez deux paquets de deux cartes de votre main à l'adversaire, il valide le paquet de son choix tandis que vous validez le paquet restant.

### Déroulement d'une manche

Les cartes sont mélangés, une carte est écarté de la manche, chaque joueur reçoit 6 cartes. Au début de leur tour, les joueurs piochent une carte.

À la fin de la manche, la possession des Geisha est mise à jour : pour chaque Geisha, si un joueur a validé strictement plus de cartes de cette Geisha que l'autre alors il prends possession de celle-ci.

Les [conditions de victoires](#conditions-de-victoires) sont vérifiées, s'il n'y a pas de gagnant, on passe à la manche suivante (les possessions des Geisha ne sont pas réinitialisées).

### Conditions de victoires

Un joueur est gagnant dans l'ordre :
 - Si la somme des valeurs des geisha qu'il possède est supérieur à 11
 - S'il possède au moins quatre carte Geisha
 - À la fin des trois manches, la partie s'arrête, le gagnant est celui totalisant le plus grand score (somme des valeurs des geisha qu'il possède), le cas échéant il y a égalité.

## Utilisation

### Installation

Télécharger [*stechec2*](https://gitlab.com/prologin/tech/tools/stechec2/)
```
git clone https://gitlab.com/prologin/tech/tools/stechec2.git
cd stechec2
```

Téléchargez le code d'Hanamikoji
```
git clone https://github.com/AntoninLoubiere/Hanamikoji.git games/hanamikoji
```

Installez les paquets requis par stechec2: cf https://gitlab.com/prologin/tech/tools/stechec2/-/blob/main/README.rst#requirements

Configurer le projet :
```
./waf.py configure --with-games=hanamikoji --prefix=/usr
```
(En cas d'erreur, `python commande inconnue`, remplacer ./waf.py par python3 waf.py)

Construisez et installez :
```
./waf.py build install
```
(Un accès administrateur `sudo` sera peut être nécessaire).

### Génération du patron de code

Pour générer le patron pour le champion :

```
stechec2-generator player hanamikoji <dossier>
```
(remplacez `<dossier>` par `players` par exemple).


### Lancement d'un match
Compilez le code (même pour python !), en exécutant dans le dossier correspondant `make`.

Créez un fichier de configuration du type
```Yaml
rules: /usr/lib/libhanamikoji.so
map: ./map.txt
verbose: 3  # Niveau d'informations de débogage affichées dans la console
clients:
  - ./champion.so
  - ./champion.so # Chemins vers les exécutables des champions, ils peuvent être différents
names:
  - Player 1
  - Player 2
dump: dump.json # Rapport du match pour le replay.
```

Lancez le match avec
```
stechec2-run config.json
```

## Utilisation Serveur

Pour utiliser le serveur, créez un compte, puis télécharger les patrons de code (le fichier [players-template.zip](https://github.com/AntoninLoubiere/Hanamikoji/raw/main/players-template.zip)), il vous suffit de compléter le fichier `Champion` du [langage de votre choix](#langages-supportés).

### Téléverser un champion sur le serveur

Si vous avez `make` d'installer, il vous suffit de lancer la commande `make` dans le dossier pour obtenir l'archive. Sinon, il suffit créer une archive avec tous les fichiers du dossier (les fichiers Makefile et Makefile-\<langage> ne sont pas nécessaire par contre), si le serveur ne détecte pas le langage, vous pouvez ajouter un fichier texte nommé `_lang` avec le nom du langage dedans (i.e. le nom du dossier de l'archive).
Une fois l'archive obtenue, vous pouvez la téléverser sur le serveur sur la page ajouter un champion.

### Lancer un match

Une fois que les champions ont été téléversés et que le serveur a terminé de compilé votre code, il suffit de se rendre sur la page lancer un match.

### Langages supportés
Langages supportés :
 - c
 - c++
 - ocaml
 - python

Pour
 - java
 - js
 - php
 - rust

veuillez nous demander.

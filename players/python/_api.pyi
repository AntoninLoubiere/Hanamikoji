# This file was generated by stechec2-generator. DO NOT EDIT.

from typing import List

from api import action, action_jouee, error, joueur

def id_joueur() -> joueur:
    """
    Renvoie l'identifiant du joueur
    """
    ...

def id_adversaire() -> joueur:
    """
    Renvoie l'identifiant de l'adversaire
    """
    ...

def manche() -> int:
    """
    Renvoie le numéro de la manche
    """
    ...

def tour() -> int:
    """
    Renvoie le numéro de la manche
    """
    ...

def tour_precedant() -> action_jouee:
    """
    Renvoie l'action jouée par l'adversaire
    """
    ...

def nb_carte_validee(j: joueur, g: int) -> int:
    """
    Renvoie le nombre de carte validée par le joueur pour la geisha
    """
    ...

def possession_geisha(g: int) -> joueur:
    """
    Renvoie qui possède la geisha
    """
    ...

def est_jouee_action(j: joueur, a: action) -> bool:
    """
    Renvoie si l'action a déjà été jouée par le joueur
    """
    ...

def nb_cartes(j: joueur) -> int:
    """
    Renvoie le nombre de carte que chaque joueur a
    """
    ...

def cartes_en_main() -> List[int]:
    """
    Renvoie les cartes que vous avez
    """
    ...

def action_valider(c: int) -> error:
    """
    Jouer l'action valider une carte
    """
    ...

def action_defausser(c1: int, c2: int) -> error:
    """
    Jouer l'action défausser deux cartes
    """
    ...

def action_choix_trois(c1: int, c2: int, c3: int) -> error:
    """
    Jouer l'action choisir entre trois cartes
    """
    ...

def action_choix_paquets(p1c1: int, p1c2: int, p2c1: int, p2c2: int) -> error:
    """
    Jouer l'action choisir entre deux paquets de deux cartes
    """
    ...

def afficher_action(v: action) -> None:
    """
    Affiche le contenu d'une valeur de type action
    """
    ...

def afficher_error(v: error) -> None:
    """
    Affiche le contenu d'une valeur de type error
    """
    ...

def afficher_joueur(v: joueur) -> None:
    """
    Affiche le contenu d'une valeur de type joueur
    """
    ...

def afficher_action_jouee(v: action_jouee) -> None:
    """
    Affiche le contenu d'une valeur de type action_jouee
    """
    ...

from api import *


# Fonction appelée au début du jeu
def init_jeu():
    print("Init")


# Fonction appelée au début du tour
def jouer_tour():
    print("Jouer tour")
    afficher_joueur(id_joueur())
    cartes = cartes_en_main()
    print(cartes)
    afficher_error(action_valider(0))


# Fonction appelée lors du choix entre les trois cartes lors de l'action de
# l'adversaire
def repondre_action_choix_trois(c1, c2, c3):
    print("Repondre action choix 3")


# Fonction appelée lors du choix entre deux paquet lors de l'action de
# l'adversaire
def repondre_action_choix_paquets(p1c1, p1c2, p2c1, p2c2):
    print("Repondre action choix paquets")


# Fonction appelée à la fin du jeu
def fin_jeu():
    print("Fin jeu")
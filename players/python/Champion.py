from api import *

MOI = 0
# Fonction appelée au début du jeu
def init_jeu():
    global MOI
    MOI = id_joueur()


# Fonction appelée au début du tour
def jouer_tour():
    print(carte_pioche())
    cartes = cartes_en_main()
    if not est_jouee_action(MOI, action.VALIDER):
        action_valider(cartes[0])
    elif not est_jouee_action(MOI, action.DEFAUSSER):
        action_defausser(cartes[0], cartes[1])
    # elif not est_jouee_action(MOI, action.CHOIX_TROIS):
    #     action_choix_trois(cartes[0], cartes[1], cartes[2])
    elif not est_jouee_action(MOI, action.CHOIX_PAQUETS):
        action_choix_paquets(cartes[0], cartes[1], cartes[2], cartes[3])
    else:
        print("???, je sais pas quoi faire")
        # exit(0)
    print("Joueur tour")


# Fonction appelée lors du choix entre les trois cartes lors de l'action de
# l'adversaire (cf tour_precedent)
def repondre_action_choix_trois():
    print("Repondre action 3")
    print(carte_pioche())
    repondre_choix_trois(2)



# Fonction appelée lors du choix entre deux paquet lors de l'action de
# l'adversaire (cf tour_precedent)
def repondre_action_choix_paquets():
    print("Repondre paquet")
    print(carte_pioche())
    repondre_choix_paquets(1)


# Fonction appelée à la fin du jeu
def fin_jeu():
    print("Fin jeu")

# 🧩 Takuzu Game

Bienvenue sur le projet **Takuzu Game** ! Ce jeu de logique solo, aussi connu sous les noms de Binairo, Tohu-Wa-Vohu ou Unruly, vous propose de remplir une grille avec des zéros et des uns selon des règles précises. 

## 🚀 Fonctionnalités principales
- Grille 6x6 (V1) ou rectangulaire (V2)
- Règles classiques et avancées (topologie torique, unicité des lignes/colonnes)
- Interface graphique avec SDL2
- Système d'annulation/rejouer, aide intégrée, détection de victoire/défaite

## 📜 Règles du jeu
- Chaque ligne et colonne doit contenir autant de zéros que de uns
- Pas plus de deux chiffres identiques consécutifs
- Certaines cases sont immuables (indices)
- En version avancée : grille rectangulaire, topologie torique, unicité des lignes/colonnes

## 🖥️ Installation & Lancement
> **Note :** Les instructions ci-dessous sont prévues pour Linux (Ubuntu/Debian). L'installation sur Windows ou macOS est possible mais nécessite des adaptations et n'est pas officiellement supportée.
### Prérequis
- Linux (Ubuntu recommandé)
- C99
- Dépendances : SDL2, SDL2_image, SDL2_ttf, cmake, gcc

### Installation rapide
```bash
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev git gcc cmake
# Clonez le projet puis compilez :
git clone git@github.com:RaymCharles/Takuzu_Game.git
cd takuzu-10a
mkdir build && cd build
cmake ..
make
./game_sdl
```

### Jouer en terminal
```bash
./game
```
Suivez les instructions pour placer vos coups (ex : `b 0 3` pour placer un 1 en ligne 0 colonne 3).

## 🕹️ Interface graphique
- Cliquez sur une case pour changer son état
- Boutons à droite : annuler, rejouer, recommencer, quitter
- Messages d'aide et de victoire/défaite intégrés

## 📂 Structure du projet
- `game.c` : logique principale du jeu
- `game_ext.c` : extensions (V2)
- `game_test.c` : tests unitaires

## 👨‍💻 Auteurs
Développé par Charles Raymondiere, Baptiste Ruberte et Salomé Rodriguez dans le cadre d'un projet universitaire.

## 💡 Remerciements
Merci à nos enseignants Aurélien Esnard, Nicolas Bonichon et Alexis Bandet pour leur accompagnement !

## 🛑 Licence
Projet privé, non open-source.

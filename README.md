# cymatic-visualizer

Un visualiseur musical écrit en **C** avec **raylib**, basé sur les **figures de Chladni** : des milliers de grains de sable sur une plaque qui vibre, et la musique décide de la façon dont elle vibre.

> **Statut : projet terminé mais considéré comme un échec.** Le programme fonctionne, mais le rendu visé n'a pas été atteint. Ce qui a marché, ce qui n'a pas marché et ce que j'en retiens est détaillé dans [LESSON.md](LESSON.md).

## Principe

Sur une plaque qui vibre, le sable quitte les zones qui bougent et s'accumule sur les **lignes nodales**, là où la plaque reste immobile. Chaque mode de vibration (m, n) donne une figure différente.

Le programme fait ça en temps réel :

1. À chaque frame, une fenêtre de 2048 échantillons du morceau est passée dans une **FFT**.
2. Le spectre est découpé en **4 bandes** (sub/basses, bas-médiums, médiums, aigus). Dans chaque bande, la fréquence la plus forte est convertie en un mode (m, n), pris dans une table triée par m² + n² (à peu près la fréquence à laquelle une vraie plaque vibre dans ce mode).
3. La plaque vibre selon la **superposition des 4 modes**, chacun pondéré par l'énergie de sa bande.
4. Chaque grain calcule l'amplitude autour de lui, part vers la zone la plus calme, et bouge d'autant plus que ça vibre là où il est. Un bruit aléatoire empêche les grains de rester bloqués.

## Fonctionnalités

- Simulation de 25 000 grains (direction par différences finies, bruit aléatoire)
- FFT Cooley-Tukey récursive écrite à la main, validée par des tests (`test.c`)
- Analyse audio synchronisée avec la lecture (copie mono du morceau)
- Analyse sur 4 bandes de fréquences, superposition pondérée de 4 modes de Chladni
- Conversion fréquence → mode (m, n) sur une échelle logarithmique
- Rendu raylib avec compteur de FPS

## Dépendances

- Un compilateur C compatible C17 (clang par défaut dans le Makefile)
- [raylib](https://www.raylib.com/)
- `make`

## Compilation

```bash
mkdir -p bin
make
```

## Utilisation

```bash
./test [-H hauteur] [-L largeur] [-G nombre_de_grains]
```

Le morceau joué est défini dans `main.c` (`music/flat.mp3` par défaut). Place un fichier audio de ton choix dans `music/`.

| Touche | Effet |
|--------|-------|
| `Espace` | Relancer la musique depuis le début |
| `P` | Pause / reprise |
| `↑` / `↓` | Monter / baisser le volume |

### Paramètres modifiables

| Paramètre | Fichier | Rôle |
|-----------|---------|------|
| `GRAIN_NB` | `main.c` | Nombre de grains |
| `HEIGHT` / `LENGHT` | `main.c` | Taille de la fenêtre (carrée par défaut, comme la plaque de la formule) |
| `TRAME` | `main.c` | Taille de la FFT : précision en fréquence contre réactivité |
| `bornes` | `main.c` | Limites des 4 bandes de fréquences (en indices de cases FFT) |
| `delta_T` | `main.c` | Amplitude des déplacements des grains |
| `a` | `physic.c` | Intensité du bruit, donc épaisseur des lignes |
| `N_MAX` | `audio.c` | Valeur max de n, donc nombre et complexité des figures |
| `frequence_min` / `frequence_max` | `audio.c` | Plage de fréquences utilisée pour choisir le mode |

## Limitations connues

- **Pas de stabilisation** : les modes et leurs poids changent à chaque frame, donc les grains ont rarement le temps de dessiner une figure nette. Ils forment surtout des points plutôt que des lignes.
- **Détection de note fragile** : le pic d'une bande est souvent une harmonique plutôt que la note jouée.
- **Pas de gestion des bords** : un grain qui sort de la fenêtre ne revient pas.
- **L'option `-M`** (choix du morceau) ne fonctionne pas : le chemin est modifié dans une copie locale.
- Le dossier `bin/` doit exister avant de compiler.

## Auteur

Projet personnel — Yanis.

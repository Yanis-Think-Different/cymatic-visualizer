PROJET : cymatic-visualizer, visualiseur musical basé sur les figures de Chladni
PÉRIODE : week-end du 10-11 octobre 2026
STATUT : Terminé, considéré comme un échec / Post-Mortem

1. OBJECTIF

Reproduire en temps réel l'expérience des figures de Chladni : du sable sur une plaque qui vibre se range sur les lignes où la plaque ne bouge pas. La musique devait piloter la vibration, et des milliers de grains devaient dessiner des figures nettes qui changent avec le morceau.

Le programme fonctionne, mais le rendu visé n'est pas atteint : les grains forment surtout des points et des amas plutôt que de longues lignes propres.

2. CE QUI FONCTIONNE

- Simulation des grains : direction calculée par différences finies sur 4 voisins, déplacement proportionnel à l'amplitude locale, bruit aléatoire pour éviter les blocages.
- FFT (Cooley-Tukey récursive) écrite à la main et validée par des tests : cas calculés à la main en N = 4 et N = 8, rampe 0..7 comparée à la DFT exacte.
- Analyse synchronisée avec la lecture : copie mono du fichier, fenêtre d'échantillons calée sur le temps de lecture.
- Analyse sur 4 bandes de fréquences, chacune donnant un mode (m, n) pondéré par son énergie, et superposition des 4 modes sur la plaque.
- Harmonisation : table des couples (m, n) triée par m² + n², parcourue en échelle logarithmique selon la fréquence détectée.

3. COMMENT LE PROJET A ÉVOLUÉ

- Version 1 : un seul mode, piloté par la fréquence dominante. Problèmes : une diagonale toujours présente, et les pics de la musique (drops, kicks) sans aucun effet.
- La diagonale venait de la formule elle-même : avec la version « − », l'amplitude est nulle sur la diagonale pour tous les modes, donc les grains finissent forcément par s'y accumuler. Je suis passé à la version « + » et à une fenêtre carrée, cohérente avec une formule de plaque carrée.
- Pour que l'énergie de la musique compte, je suis passé à 4 bandes de fréquences, chacune avec son mode et un poids égal à son énergie.

4. POURQUOI LE RENDU NE MARCHE PAS

- Les modes et leurs poids changent à chaque frame. Les grains n'ont jamais le temps de dessiner une figure : ils finissent là où c'est calme dans plusieurs configurations à la fois, c'est-à-dire en points et en amas. La solution (ne changer de mode que s'il tient quelques frames, ou lisser les poids dans le temps) était identifiée tôt, mais je ne l'ai jamais implémentée.
- J'ai réglé des symptômes au lieu de traiter la cause : delta_T, bruit, opérateurs de la formule, nombre de grains.
- La détection de note est fragile : le pic d'une bande est souvent une harmonique plutôt que la note jouée, et il saute d'une frame à l'autre.

5. BILAN PERSONNEL

- Traduire des maths en code : c'est la partie qui m'a le plus coûté. Comprendre Chladni et la FFT, puis passer du pseudo-code à du C avec pointeurs et strides, m'a demandé beaucoup de temps, mais c'est aussi ce qui m'a le plus appris.
- Ma méthode de debug : c'est mon principal axe de progression. Plusieurs bugs (un cast de pointeur incorrect, une recherche de max qui ne mettait pas le max à jour, deux axes inversés dans le calcul de direction) venaient de détails que j'aurais dû repérer en relisant.

6. UTILISATION DE L'IA

J'ai utilisé une IA (Claude) comme tuteur, avec une règle fixée dès le départ : pas de code ni d'architecture fournis, je voulais concevoir et écrire le projet moi-même.

- Le code est le mien : conception, structures, simulation, FFT, harmonisation, rendu. La seule exception est une double boucle de remplissage de table, déléguée, que je sais réexpliquer ligne par ligne.
- L'IA m'a expliqué la théorie (figures de Chladni, FFT, échantillonnage) et, pendant le debug, m'a indiqué où chercher quand je bloquais. Les corrections, je les ai faites moi-même.
- Ce que j'en retiens : elle m'a fait gagner du temps sur la compréhension, mais trop compter sur elle pour localiser les bugs m'a empêché de muscler ce réflexe. Pour les prochains projets, je cherche seul d'abord, et je ne demande que des pistes.

7. CE QUE JE RETIENS

- Traiter la cause identifiée avant de tourner les boutons.
- Changer une variable à la fois, et noter ce que ça change.
- Tester chaque brique isolément. Je l'ai fait pour la FFT, et c'est la seule partie dont je suis sûr à 100 %.
- Chercher seul 20 minutes avant de demander : relire chaque affectation et chaque cast, dérouler les boucles à la main, comparer le code à sa propre documentation.
- Comprendre les pièges d'une formule avant de coder autour.

8. SI JE REPRENDS LE PROJET

- Stabiliser les modes et lisser les poids dans le temps (priorité absolue).
- Gérer les bords de la plaque.
- Mieux distinguer la note jouée de ses harmoniques.
- Éviter l'entassement des grains (répartition par grille spatiale).

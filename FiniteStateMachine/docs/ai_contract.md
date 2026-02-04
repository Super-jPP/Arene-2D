AI Contract — Finite State Machine
1. Rôle de l’Intelligence Artificielle

L’IA est responsable uniquement de la prise de décision :
choisir un comportement (Idle, Wander, Chase, Attack, Dead)
déclencher des intentions (se déplacer, attaquer)
L’IA ne gère pas :
le rendu (SFML, sprites, animations)
les collisions
l’application des dégâts
les entrées clavier
la suppression des entités
👉 L’IA décide quoi faire, le gameplay décide comment c’est exécuté.

2. Contraintes architecturales (CDC)

La FSM est générique et template
Aucun code spécifique à SFML dans l’IA
Aucun accès direct au moteur de jeu
Les états sont des classes distinctes
Les transitions sont explicites et basées sur des conditions observables

3. Types neutres utilisés par l’IA

3.1 Vec2

Type mathématique indépendant de SFML.
Représente :
une position
une direction
un vecteur de déplacement

Fonctionnalités minimales attendues :
addition / soustraction
multiplication par un scalaire
normalisation
longueur
distance entre deux Vec2
Toute conversion avec sf::Vector2f est faite côté gameplay / rendu.

4. Paramètres utilisés par l’IA (Stats)

Chaque ennemi expose une structure de paramètres, utilisée par l’IA
sans connaître le type concret de l’ennemi.

Paramètres attendus :
moveSpeed : vitesse de déplacement
detectRadius : distance d’aggro
lostRadius : distance de désengagement (anti-flicker)
attackRange : distance d’attaque
attackCooldown : temps minimal entre deux attaques

Ces valeurs peuvent différer entre plusieurs types d’ennemis,
tout en utilisant la même FSM et les mêmes états.

5. Mémoire IA par entité (AI Memory)

Les états IA sont stateless.
Toute donnée persistante doit être stockée dans l’entité contrôlée.
Mémoire IA minimale attendue :
spawnTimer : délai initial avant activation
wanderTimer : durée avant changement de direction
wanderDirection : direction actuelle de déplacement aléatoire
Cette mémoire est propre à chaque ennemi et n’est jamais stockée
dans les classes d’état.

6. API attendue par l’IA (Owner Interface)

L’IA considère l’entité contrôlée comme un Owner générique.
L’Owner doit exposer les méthodes suivantes.

6.1 Accesseurs (lecture)

Position actuelle de l’ennemi
Position actuelle du joueur
Points de vie de l’ennemi
Accès à la structure Stats
Indique si l’attaque est prête (cooldown terminé)
Ces informations sont utilisées uniquement pour
évaluer les conditions de transition.

6.2 Actions (commandes)

Déplacer l’ennemi vers une position cible
Déplacer l’ennemi dans une direction donnée
Demander une attaque (sans appliquer directement les dégâts)
Définir le nom de l’état courant pour l’affichage debug
L’IA ne vérifie pas les collisions et n’applique pas les dégâts.

7. États IA standards (Vampire Survivors-like)

Les états suivants sont implémentés :
Idle
Wander
Chase
Attack
Dead

Chaque état :
définit son comportement propre
déclenche explicitement les transitions
met à jour le nom d’état pour le debug

8. Priorité des transitions

À chaque mise à jour, les transitions sont évaluées
selon l’ordre de priorité suivant :
Dead si les points de vie sont ≤ 0
Attack si la cible est à portée et l’attaque prête
Chase si la cible est détectée
Wander ou Idle par défaut

9. Debug et observabilité

L’IA doit être observable visuellement.
À l’entrée de chaque état :
le nom de l’état courant est transmis à l’Owner

Le gameplay peut utiliser cette information pour :
afficher un texte au-dessus de l’ennemi
changer la couleur ou l’animation
afficher des cercles de détection (optionnel)

10. Objectif de réutilisabilité

La FSM et les états doivent pouvoir être réutilisés :
pour plusieurs types d’ennemis
dans un autre projet
sans modification du code IA
Toute dépendance spécifique au jeu invalide cet objectif.
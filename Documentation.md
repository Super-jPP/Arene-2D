## LOST HAUNTED FOREST

**CONCEPT**

Vous incarner un chasseur qui s'est égaré dans une forêt maudite.
Des hordes rempli de squelettes et de chauve-souris vous attaqueront sans cesse.
Dans ce Horde Survivor, le but est de battre le plus de monstres possible afin d'obtenir le plus grand score. 
Il vous faudra réussir à optimiser vos déplacements et bien gérer la distance de vos attaques pour rester en vie. La fin est proche pour vous… Mais combien de monstres emporterez vous dans la tombe ? 

**FONCTIONNALITES REALISEES**

Nous avons réussi à créer un système qui ajoute deux différents types d'ennemis à l'infini, un rapide mais avec peu de vie, et l'autre plus lent mais avec plus de vie. La différence la plus marquante est surtout dans leur façon d'agir, l'un attaque le joueur avec certaines conditions alors que l'autre chasse le joueur directement sans interruption.

Grâce à la FSM, nous avons implémenter dans le fonctionnement des ennemis un état "idle", un état "wander", un état "chase", un état "attack" et un état "dead".
Tout ces états ont leur propre conditions de changements.

-ETAT IDLE : (squelette)
	- état neutre qui sert au squelette à faire une animation d'asset pour fluidifié le mouvement. Il se lance parfois pendant l'"état wander".

-ETAT WANDER : (squelette)
	-Possède une zone de détection autour de lui. 
	-Si la hitbox du joueur rentre dedans, alors il passe en "état chase".

-ETAT CHASE : (squelette)
	-Possède deux zones autour de lui. 
	-La première zone prend les coordonnées du joueur pour que l'ennemi se rapproche le plus possible de lui. Si le joueur sors de cette zone, alors retour en "état wander".
	-La deuxième est très proche de l'ennemi, et lui permet de passer en "état attack".
	-POUR LA CHAUVE-SOURIS, qu'importe les coordonnées du joueur, elle sera toujours dès le début en "état chase". Elle ne possède donc que la zone de transition vers l'"état attack".

-ETAT ATTACK : (squelette et chauve-souris)
	-Cet état permet à l'ennemi de faire perdre des points de vie au joueur. Il continuera de le faire tant que le joueur ne décide pas de s'écarter de lui. 

-ETAT DEAD : (squelette et chauve-souris)
	-Cet état permet à l'ennemi de disparaitre de l'écran ainsi que de le supprimer de la mémoire pour faire apparaitre d'autres ennemis, car il y a un nombre maximum d'ennemis possible dans le jeu en même temps.


**FONCTIONNALITES NON REALISES**

Nous n'avons pas réussi à implémenter les fonctionnement que nous voulions pour les armes.
On voulait que le joueur puisse récupérer différents bonus qui ajoute des armes passives autour du joueur pour avoir un sentiment de monter de puissance. 
On voulait aussi que les armes deviennent de plus en plus puissantes quand on ramasse plusieurs fois le même type.
Mais par faute de temps nous avons privilégier le fonctionnement des ennemies.
Dans le build jouable, le personnage possède uniquement une épée pour attaquer ses adversaires.


**SCHEMA DE LA FSM**

```mermaid
 Chaser (Bat),
stateDiagram-v2
direction LR

[*] --> Chase: spawn Chaser

Chase --> Attack: inRange && attackReady
Attack --> Chase: recoveryDone

Chase --> Dead: hpZero
Attack --> Dead: hpZero

state Attack {
    [*] --> Windup
    Windup --> Active: windupDone
    Active --> Recovery: activeDone
    Recovery --> [*]: recoveryDone
}
```
**SOURCE ASSET**

 https://legnops.itch.io/red-hood-character
itch.io
Red hood pixel character by Legnops
Free smooth pixel character


https://luizmelo.itch.io/monsters-creatures-fantasy
itch.io
Monsters Creatures Fantasy by LuizMelo


https://flowfrog101.itch.io/free-bat-enemy-spritesheet
itch.io
Free Bat-Enemy Spritesheet by FlowFrog101
Top down animated sprite

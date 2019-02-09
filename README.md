# Constraint_Programming
Project for the constraint programming course at MPRO

## TODO
Forward checking<br> 
Arc consistance<br> 
Possibilité de combiner les méthodes<br> 
Ordre de progression dans le backtrack : choix des variables, choix des valeurs (shuffle, plus grand domaine, etc ...)

Gestion des symétries

Création des instances de coloration

## TO FIX
Pas besoin de checker si l'instantiation partielle est correcte si l'arc consistency est enclenchée ?<br>
Vecteur des valeurs supprimées par Arc consistency construit par copie dans le backtrack récursif
Peut etre changé pour éviter la copie<br>
Tout est bien remis à zero une fois le backtrack fini ?<br>
Bonne taille de vecteur dans chaque vecteur booléen de contraintes ?<br>

## Evolution du temps pour les reines
07/02 : 20 en 1.15s, 25 en 1.476s, 30 en 5.909s, 32 en 31.013s, 40 prend trop de temps
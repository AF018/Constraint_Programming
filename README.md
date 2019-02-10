# Constraint_Programming
Project for the constraint programming course at MPRO

## TODO
Ordre de progression dans le backtrack : choix des variables, choix des valeurs (shuffle, plus grand domaine, etc ...)

Gestion des symétries

## TO FIX
Pas besoin de checker si l'instantiation partielle est correcte si l'arc consistency est enclenchée ?<br>
Faire la vérification des contraintes ou l'arc consistance mais pas les deux<br>

## Evolution du temps pour les reines
07/02 : 20 en 1.15s, 25 en 1.476s, 30 en 5.909s, 32 en 31.013s, 40 prend trop de temps
09/02 : 20 en 0.433s, 25 en 0.507s, 30 en 1.768s, 32 en 9.985s, 35 en 5.038s, 40 en 29.241s, 45 en 100.16s
Correction d'un bug : régression des valeurs, bcp plus mauvais
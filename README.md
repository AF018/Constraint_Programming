# Constraint_Programming
Project for the constraint programming course at MPRO

## TODO
Ordre de progression dans le backtrack : choix des variables, choix des valeurs (shuffle, plus grand domaine, etc ...)
Faire appel a AC une fois une bonne partie des variables fixées

Gestion des symétries

## TO FIX
Faire un shuffle de visit_order_vect au départ pour voir si il n'y a pas de probleme avec la gestion des indices <br>
Il y a un moyen d'effectuer le parcours des contraintes plus efficacement dans AC4 ?
La récursion est peut etre moins efficace qu'un for/while loop

## Evolution du temps pour les reines
07/02 (AC) : 20 en 1.15s, 25 en 1.476s, 30 en 5.909s, 32 en 31.013s, 40 prend trop de temps <br>
09/02 (AC) : 20 en 0.433s, 25 en 0.507s, 30 en 1.768s, 32 en 9.985s, 35 en 5.038s, 40 en 29.241s, 45 en 100.16s <br>
Correction d'un bug : régression des valeurs <br>
10/02 (FC): 20 en 0.02s, 25 en 0.153s, 30 en 0.23s, 32 en 8.127s, 35 en 43.954s, 40 en ? <br>

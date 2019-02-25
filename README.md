# Constraint_Programming
Project for the constraint programming course at MPRO

## TODO
Dans la gestion de symétrie pour la coloration, sélectionner des variables qui sont fortement contraintes entre elles et non des variables aléatoires<br>
Vérifier la gestion des symétries de color<br>
intéressant de lancer le small domain var dès la méthode backtrackSolve ?<br>

## Evolution du temps pour les reines
07/02 (AC) : 20 en 1.15s, 25 en 1.476s, 30 en 5.909s, 32 en 31.013s, 40 prend trop de temps <br>
09/02 (AC) : 20 en 0.433s, 25 en 0.507s, 30 en 1.768s, 32 en 9.985s, 35 en 5.038s, 40 en 29.241s, 45 en 100.16s <br>
Correction d'un bug : régression des valeurs <br>
10/02 (FC) : 20 en 0.02s, 25 en 0.153s, 30 en 0.23s, 32 en 8.127s, 35 en 43.954s, 40 en ? <br>
Ajout d'un shuffle dans l'ordre de visite et les domaines<br>
12/02 (FC+RV) : 20 en moins de 0.001s, 30 en 0.002s, 40 en moins de 0.01s, 100 en moins de 2s la plupart du temps, 200 en moins de 3s la plupart du temps, 300 en moins de 5s la moitié du temps, trop long dans les reste des cas<br>
Visite des plus petits ou plus grands domaines en premier
14/02 (FC+SD+RV) : 20 en 0.001s, 30 en 0.001s, 50 en 0.002s, 100 en 0.004s, 150 en 0.008s, 200 en 0.014s, 250 en 0.024s, 300 en 0.04s, 350 en 0.055s <br>
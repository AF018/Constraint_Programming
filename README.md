# Constraint_Programming
Project for the constraint programming course at MPRO

## TODO
Forward checking<br> 
Arc consistance<br> 
Possibilité de combiner les méthodes<br> 
Ordre de progression dans le backtrack : choix des variables, choix des valeurs

Gestion des symétries

Création des instances de coloration

## TO FIX
Vecteur des valeurs supprimées par Arc consistency construit par copie dans le backtrack récursif
Peut etre changé pour éviter la copie<br>
Map au lieu de unordered_map a cause du hash<br>
Valeurs pas rajoutées dans le même ordre qu'à l'ajout : n'enlever que des valeurs de domaine si variable pas encore instanciée<br>
Tout est bien remis à zero une fois le backtrack fini ?


La raison pour laquelle l'arc consistance ne marche pas:
parcours des contraintes au départ au lieu de parcourir les domaines
il faut parcourir les domaines et vérifier les contraintes a partir des couples a,b
Changer le format des données pour les contraintes ? (booléen?)
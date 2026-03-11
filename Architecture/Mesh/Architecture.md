## Shape
Définie la forme topologique. Elle ne dépend pas encore du maillage. Une forme de dimension D est définie par le nombre de formes de dimension d<D qu'elle contient. 
On y stocke le nombre de formes de dimensions d.

## Entité
Une entité du maillage est définie par sa forme, ses coordonnées et ses relations d'incidences et d'adjacence. Elle ne définit pas le maillage (c'est pourquoi elle ne fait pas partie de la classe maillage) mais permet d'avoir accès aux informations d'une entité précise. Elle possède des attributs qui pointent vers les coordonnées ou les entités incidentes ou adjacentes.

## Maillage
### Topologie

#### Listes des entités
Selon les besoins, toutes ou certaines listes sont crées.
#### Incidences
Les relations d'incidences indiquent quelles entité de dimensions d sont contenues dans une entité de dimension D. Pour chaque entité de dimension D, une liste peut être crée pour chaque entité de dimension d. Cette liste contient les indices des entités de dimensions d contenues dans cette entité.

#### Adjacences
Les relations d'adjacences représentes les entités d'une même dimension qui se "touchent" i.e. qui ont une entité de dimension inférieure en commun. Selon les besoin, on peut définir les relations d'adjacences totales (s'il existe une entité de dimension inférieure en commun, alors elles sont adajcentes) ou plus faibles (s'il existe une uniquement une face en commun et pas juste une arrête ou un sommet -- idem avec juste une arrête ou juste un sommet--). Ces relations faibles peuvent être représentées sous formes de graphes dont les noeuds sont les entités dont on construit les relations d'adjacence et les arrête (du graphe) sont les entités de dimension inférieure en commun.
### Geometrie
### Liste de coordonnées
Cette liste correspond aux coordonnées des sommets du maillage.

### Itérateurs
Un itérateur permet de d'itérer sur les entités du maillage. Elle renvoient un ```Entity_view``` défini selon Entité au dessus.

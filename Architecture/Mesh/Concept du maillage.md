
# Spécification conceptuelle — Mesh générique (Donk)

## 1. Rôle du Mesh (contrat)

Le **Mesh** est une structure :

- **topologique** : entités + relations orientées
    
- **géométrique** : plongement métrique dans ℝ^G
    
- **neutre vis-à-vis des méthodes numériques**
    

Il est conçu pour être utilisé **tel quel** par :

- FE (CG, non conforme),
    
- DG,
    
- FV,
    
- schémas hybrides (FCT, etc.).
    

Le Mesh **ne connaît pas** :

- les DDL,
    
- l’ordre de discrétisation,
    
- les schémas numériques,
    
- l’algèbre linéaire.
    

---

## 2. Entités topologiques minimales

Pour une dimension topologique `D`, le Mesh fournit **toutes les entités** :

|Dimension|Entité|Rôle principal|
|---|---|---|
|0|Sommets|Géométrie, FE|
|1|Arêtes|FCT, P2+, géométrie|
|D−1|Facettes|Flux FV / DG, BC|
|D|Cellules|Volume de contrôle, élément FE|

👉 Aucune hypothèse globale sur le type de cellule  
(tri, quad, tet, hex, prism, … peuvent coexister).

---

## 3. Relations topologiques garanties

### Relations **fondamentales** (définissables pour toute entité)

- cellule → facettes
    
- cellule → arêtes
    
- cellule → sommets
    
- facette → arêtes **ou** sommets
    
- arête → sommets
    
- facette → cellules (0, 1 ou 2)
    
- sommet → cellules
    
- sommet → sommets (adjacence)
    

### Relations **sémantiques**

- entité → frontière / intérieur
    
- incidence **orientée** (signe cellule ↔ facette)
    

👉 Toutes ces relations sont :

- **conceptuellement définies**
    
- **matérialisées à la demande**
    
- **accessibles en O(1) si construites**
    

---

## 4. Orientation et conventions

- Les relations d’incidence sont **orientées**
    
- Une convention globale et déterministe est fixée  
    (ex : face orientée de `C_i` vers `C_j` avec `i < j`)
    
- La topologie fournit :
    
    - un **signe relatif**
        
    - jamais une grandeur métrique
        

---

## 5. Géométrie

Le Mesh fournit :

- coordonnées des sommets
    
- mesures géométriques dérivables :
    
    - longueurs, aires, volumes
        
- normales **géométriques non contextuelles**
    

⚠️ Les normales FV/DG sont :

> des objets **dérivés du couple (cellule, facette)**  
> combinant géométrie + orientation topo

---

## 6. Interdits du Mesh (non négociables)

Le Mesh **ne doit pas contenir** :

1. ❌ nœuds / DDL
    
2. ❌ ordre de discrétisation
    
3. ❌ schémas numériques
    

(ni implicitement, ni par “service pratique”)

---

## 7. Conséquences positives (pour Donk)

Grâce à cette séparation :

- maillages hybrides : ✔️
    
- p-adaptativité locale : ✔️
    
- FE / FV / DG sur le même mesh : ✔️
    
- MPI robuste : ✔️
    
- multi-physique : ✔️
    

Et surtout :

> **le Mesh n’aura pas besoin d’être modifié quand tu inventeras une nouvelle méthode.**
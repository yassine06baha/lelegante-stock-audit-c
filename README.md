# L'Élégante - Stock Audit en C

Outil C pour analyser le stock boutique, détecter les ruptures proches et générer un rapport web consultable dans le navigateur.

## Stack

- C
- SQL
- HTML / CSS
- JavaScript

## Fonctionnalités

- Lecture d'un fichier CSV produit
- Calcul d'un score de priorité de réapprovisionnement
- Génération automatique d'un fichier `public/report.js`
- Visualisation web du stock critique avec filtres par catégorie
- Script SQL pour modéliser les données produit/stock

## Compiler et exécuter

```bash
gcc src/stock_audit.c -o stock_audit
./stock_audit data/products.csv public/report.js
```

Puis ouvrir `public/index.html`.
